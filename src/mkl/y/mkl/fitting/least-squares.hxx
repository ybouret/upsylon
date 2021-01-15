//! \file

#if 0
private:

struct D2_function
{
    sample_api_type            *s_;
    sequential_type            *F_;
    const accessible<ORDINATE> *aorg_;
    const accessible<ORDINATE> *step_;
    addressable<ORDINATE>      *atmp_;
    
    inline ORDINATE operator()(const ORDINATE u)
    {
        assert(s_); assert(aorg_); assert(step_); assert(atmp_);
        tao::muladd(*atmp_, *aorg_, u, *step_);
        return s_->D2(*F_,*atmp_);
    }
    
};

public:
#endif

//______________________________________________________________________________
//
//! generic call
/**
 \param s a sample interface
 \param F a sequential function
 \param G a gradient of F
 \param A array of parameters
 \param U array of flags to use parameters
 \param E array of errors
 */
//______________________________________________________________________________
inline bool fit(sample_api_type        &s,
                sequential_type        &F,
                v_gradient_type        &G,
                addressable<ORDINATE>  &A,
                const accessible<bool> &U,
                addressable<ORDINATE>  &E)
{
    //--------------------------------------------------------------------------
    //
    //
    // initialize
    //
    //
    //--------------------------------------------------------------------------
    Y_STATIC_CHECK(sizeof(bool)<=sizeof(ORDINATE),boolean_too_large);
    static const ORDINATE vtol  = get_vtol();
    static const ORDINATE dtol  = get_dtol();

    //--------------------------------------------------------------------------
    //
    // dimensions
    //
    //--------------------------------------------------------------------------
    const variables &vars = s.vars; // variables for this run
    M      = vars.sweep();          // dimensions
    p      = 0;                     // regularization
    lambda = lam[p];                // matching coefficient

    //--------------------------------------------------------------------------
    //
    // memory setup
    //
    //--------------------------------------------------------------------------
    alpha.make(M,M);
    covar.make(M,M);
    space.acquire(M);
    {
        ORDINATE *tmp = *utmp;
        new ( &used ) flags_type( (bool *)tmp, M );
    }


    
    //--------------------------------------------------------------------------
    //
    // initialize values
    //
    //--------------------------------------------------------------------------
    tao::set(aorg,A);     // setup aorg
    tao::ld(aerr,-s.one); // setup aerr
    tao::ld(used,false);  // setup used
    tao::set(used,U);     // fill used
    vars.set(E,aerr);     // fill error values with -1
    s.setup(aorg);
    
    Y_GLS_PRINTLN("-------- <initialized: p=" << p << ", lambda=" << lambda << "> --------");
    if(verbose)
    {
        display_variables::values(std::cerr, "\t(--) ", s.vars, aorg, ", used=", used, NULL);
    }
    
    
    //--------------------------------------------------------------------------
    //
    //
    // cycle
    //
    //
    //--------------------------------------------------------------------------
    size_t cycle      = 0;      // cycle indication
    bool   decreasing = true;   // is lambda decreasing?
CYCLE:
    ++cycle;
    ORDINATE D2_org = s.D2(alpha,beta,F,G,aorg,used);
    Y_GLS_PRINTLN("-------- <run@cycle=" << cycle << "> -------- " );
    Y_GLS_PRINTLN("D2_org  = " << D2_org);
    
    //--------------------------------------------------------------------------
    //
    // compute curvature with lamba increase, then compute the step.
    // The step has some numerical noise when small
    //
    //--------------------------------------------------------------------------
COMPUTE_STEP:
    if(!compute_step(decreasing))
    {
        //----------------------------------------------------------------------
        // here, a singular curvature is met
        //----------------------------------------------------------------------
        return false;
    }
    //Y_GLS_PRINTLN("          " << D2_org << " | step=" << step);

    
    
    //--------------------------------------------------------------------------
    //
    // At this point, the curvature is not singular, so
    // compute the descent slope
    //
    //--------------------------------------------------------------------------
    const ORDINATE sigma_over_two = tao::dot<ORDINATE>::of(beta,step);
    const ORDINATE sigma          = sigma_over_two + sigma_over_two;
#if 0
    if(sigma<0)
    {
        Y_GLS_PRINTLN("<negative sigma>");
        goto COMPUTE_STEP;
    }
#endif
    //--------------------------------------------------------------------------
    //
    // OK, numerically acceptable step, compute atry and recompute step
    //
    //--------------------------------------------------------------------------
    for(size_t i=M;i>0;--i)
    {
        atry[i] = aorg[i] + step[i];
        step[i] = atry[i] - aorg[i];
    }


    
    //--------------------------------------------------------------------------
    //
    // compute trial D2
    //
    //--------------------------------------------------------------------------
    ORDINATE       D2_try  = s.D2(F,atry);
    Y_GLS_PRINTLN("D2_try  = " << D2_try << " @ lambda=10^" << std::setw(4) << p << ", decreasing = " << std::setw(5) << textual::boolean(decreasing) << ", sigma=" << sigma );
    
    if(D2_try>D2_org)
    {
        //----------------------------------------------------------------------
        //
        // reject, set
        //
        //----------------------------------------------------------------------
        decreasing = false;
        if(!increase())
        {
            Y_GLS_PRINTLN("<reject with spurious failure>");
            return false;
        }

        goto COMPUTE_STEP;
    }
    else
    {
        //----------------------------------------------------------------------
        //
        // accept
        //
        //----------------------------------------------------------------------
        Y_GLS_PRINTLN("<accept>");

        if(verbose)
        {
            display_variables::values(std::cerr, "\t(->) ", s.vars, atry, " (", step, ")");
        }

        //----------------------------------------------------------------------
        // update algorithm status, with aorg, atry and step
        //----------------------------------------------------------------------
        decrease();

        bool converged = true;
        {
            size_t i=M;
            for(;i>0;--i)
            {
                const ORDINATE a_new = atry[i];
                const ORDINATE a_old = aorg[i];
                const ORDINATE da    = fabs_of( step[i] );
                aorg[i] = a_new;
                if( da > vtol * max_of( fabs_of(a_new), fabs_of(a_old) ) )
                {
                    converged = false;
                    --i;
                    break;
                }
            }

            for(;i>0;--i)
            {
                aorg[i] = atry[i];
            }

        }

        //----------------------------------------------------------------------
        // testing variable convergence
        //----------------------------------------------------------------------
        if(converged)
        {
            Y_GLS_PRINTLN("<variables convergence>");
            goto CONVERGED;
        }
        

        //----------------------------------------------------------------------
        // upon already decreasing, test D2 convergence
        //----------------------------------------------------------------------
        if(decreasing)
        {
            const ORDINATE dd = fabs_of(D2_org-D2_try);
            if( dd <= dtol * max_of(D2_org,D2_try) )
            {
                Y_GLS_PRINTLN("<D2 convergence>");
                goto CONVERGED;
            }
        }
        
        decreasing = true;
        goto CYCLE;
    }
    
    
CONVERGED:
    //--------------------------------------------------------------------------
    //
    //
    // success ?
    //
    //
    //--------------------------------------------------------------------------
    Y_GLS_PRINTLN("-------- <end@cycle=" << cycle << "> -------- " );

    //--------------------------------------------------------------------------
    //
    // final D2
    //
    //--------------------------------------------------------------------------
    D2_org = s.D2(alpha,beta,F,G,aorg,used);
    Y_GLS_PRINTLN("D2_org    = " << D2_org << " (lambda=" << lambda << ")" );
    if(verbose)
    {
        display_variables::values(std::cerr, "\tgradient/", vars, beta, NULL);
        std::cerr << std::endl;
    }


    //--------------------------------------------------------------------------
    //
    // compute covariance
    //
    //--------------------------------------------------------------------------
    if(!LU::build(alpha))
    {
        Y_GLS_PRINTLN("<singular extremum>");
        return false;
    }
    LU::inverse(alpha,covar);
    for(size_t j=M;j>0;--j)
    {
        if(!used[j]) covar[j][j] = 0;
    }
    //Y_GLS_PRINTLN("covar     = " << covar);
    if(verbose)
    {
        for(size_t j=M;j>0;--j)
        {
            atmp[j] = covar[j][j];
        }
        display_variables::values(std::cerr, "\tvariance@", vars, atmp, NULL);
    }

    //--------------------------------------------------------------------------
    //
    // set new parameters
    //
    //--------------------------------------------------------------------------
    vars.set(A,aorg);
    
    
    //--------------------------------------------------------------------------
    //
    // compute d.o.f
    //
    //--------------------------------------------------------------------------
    size_t       ndof = s.count();
    const size_t nuse = s.vars.used_in(U);

    
    if(nuse>ndof)
    {
        //----------------------------------------------------------------------
        //
        // meaningless, leave error to -1
        //
        //----------------------------------------------------------------------
        Y_GLS_PRINTLN("<meaningless>" );
        return true;
    }
    else if(nuse==ndof)
    {
        //----------------------------------------------------------------------
        //
        // interpolation, set error to 0
        //
        //----------------------------------------------------------------------
        Y_GLS_PRINTLN("<interpolation>");
        tao::ld(aerr,s.zero);
        vars.set(E,aerr);
        return true;
    }
    else
    {
        //----------------------------------------------------------------------
        //
        // successfull, compute individual errors
        //
        //----------------------------------------------------------------------
        assert(ndof>nuse);
        Y_GLS_PRINTLN("<success>");
        ndof -= nuse;
        const size_t n2 = ndof*ndof;
        for(size_t i=M;i>0;--i)
        {
            aerr[i] = sqrt_of( D2_org * max_of<ORDINATE>(0,covar[i][i]) / n2 );
        }
        vars.set(E,aerr);
        if(verbose)
        {
            display_variables::errors(std::cerr, least_squares_::prefix_tab, vars, A, U, E);
        }
        Y_GLS_PRINTLN("-------- <finalized> --------");
        return true;
    }
    
}
