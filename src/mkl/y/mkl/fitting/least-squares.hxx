//! \file

private:

struct F1D
{
    sample_api_type     *        _data;
    sequential_type     *        _func;
    const accessible<ORDINATE> * _aorg;
    const accessible<ORDINATE> * _step;
    addressable<ORDINATE>      * _atmp;

    inline ORDINATE operator()(const ORDINATE u)
    {
        tao::muladd(*_atmp, *_aorg, u, *_step);
        return _data->D2(*_func, *_atmp);
    }

};


public:
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
    // initialize memory and local variables
    //
    //--------------------------------------------------------------------------
    setup_fields(s,A,U,E);
    const variables &vars = s.vars; // variables for this run
    p                     = 0;      // regularization
    lambda                = lam[p]; // matching coefficient

    Y_GLS_PRINTLN("-------- <initialized: p=" << p << ", lambda=" << lambda << "> --------");
    Y_GLS_VERBOSE(display_variables::values(std::cerr, "\t(--) ",s.vars,aorg,", used=",used,NULL));


    F1D g = { &s, &F, &aorg, &step, &atmp };
    
    //--------------------------------------------------------------------------
    //
    //
    // cycle
    //
    //
    //--------------------------------------------------------------------------
    size_t cycle      = 0;      // cycle indication
    bool   decreasing = true;   // is lambda decreasing?
    if(writing)
    {
        ios::ocstream::overwrite(least_squares_::writing_id);
    }
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

    if(writing)
    {
        //const ORDINATE D2_new = g(1);
        for( ORDINATE u=0; u<=2; u += ORDINATE(0.01) )
        {
            const ORDINATE tmp = D2_org - sigma * u + 0.5 * u * sigma * u;
            //const ORDINATE tmp = D2_org - sigma * u + 0.25 * u * sigma * u;
            ios::ocstream::echo( least_squares_::writing_id, "%.15g %.15g %.15g\n", u, g(u), tmp );
        }
        ios::ocstream::echo( least_squares_::writing_id, "\n");
    }
    
    //--------------------------------------------------------------------------
    //
    // compute trial D2
    //
    //--------------------------------------------------------------------------
    ORDINATE       D2_try  = s.D2(F,atry);
    Y_GLS_PRINTLN("D2_try  = " << D2_try << " @ lambda=10^" << std::setw(4) << p << "; decreasing = " << std::setw(5) << textual::boolean(decreasing) << "; sigma=" << sigma );
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
        Y_GLS_VERBOSE(display_variables::values(std::cerr, "\t(->) ",s.vars,atry," (", step,")"));


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
    Y_GLS_VERBOSE(display_variables::values(std::cerr, "\tgradient/",vars,beta,NULL) << std::endl);


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
    Y_GLS_VERBOSE(tao::diag::get(atmp,covar);
                  display_variables::values(std::cerr, "\tvariance@", vars, atmp, NULL) );

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
        Y_GLS_VERBOSE(display_variables::errors(std::cerr, least_squares_::prefix_tab,vars,A,U,E));
        Y_GLS_PRINTLN("-------- <finalized> --------");
        return true;
    }
    
}

//______________________________________________________________________________
//
//! compute solo/unlinked errors when at fit value
/**
 \param s a sample interface
 \param F a sequential function
 \param G a gradient of F
 \param A array of parameters
 \param U array of flags to use parameters
 \param E array of errors
 */
//______________________________________________________________________________
inline void errors(sample_api_type        &s,
                   sequential_type        &F,
                   v_gradient_type        &G,
                   addressable<ORDINATE>  &A,
                   const accessible<bool> &U,
                   addressable<ORDINATE>  &E)
{
    setup_fields(s,A,U,E);
    size_t ndof = s.count();
    if(ndof<=1) return;
    --ndof;
    for(variables::iterator it=s.vars.begin();it!=s.vars.end();++it)
    {
        const variable &v = **it;
        const size_t    i = *v;
        if(U[i])
        {
            tao::ld(used,false);
            used[i] = true;
            const ORDINATE D2_org = s.D2(alpha,beta,F,G,aorg,used);
            const ORDINATE err2   = max_of<ORDINATE>(s.zero,D2_org/alpha[i][i])/(ndof*ndof);
            aerr[i] = sqrt_of(err2);
        }
        else
        {
            aerr[i] = s.zero;
        }
    }
    s.vars.set(E,aerr);
}
