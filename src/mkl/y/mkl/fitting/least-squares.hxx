//! \file

private:
struct d2_wrapper
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
                addressable<ORDINATE>  &E,
                const unsigned          flags=0x00)
{
    //--------------------------------------------------------------------------
    //
    //
    // initialize
    //
    //
    //--------------------------------------------------------------------------
    static const ORDINATE vtol  = numeric<ORDINATE>::ftol;
    static const ORDINATE dtol  = numeric<ORDINATE>::sqrt_ftol;
    static const ORDINATE ufac  = numeric<ORDINATE>::gold;

    //--------------------------------------------------------------------------
    //
    // variables
    //
    //--------------------------------------------------------------------------
    assert(A.size()==U.size());
    M      = A.size();             // dimensions
    p      = 0;                    // regularization
    lambda = lam[p];               // matching coefficient
    
    //--------------------------------------------------------------------------
    //
    // memory
    //
    //--------------------------------------------------------------------------
    alpha.make(M,M);
    covar.make(M,M);
    beta.adjust(M,s.zero);
    aorg.adjust(M,s.zero);
    atry.adjust(M,s.zero);
    step.adjust(M,s.zero);
    atmp.adjust(M,s.zero);
    used.adjust(M,false);
    tao::set(aorg,A);
    tao::set(used,U);
    tao::ld(E,-1);
    s.setup(A);
    
    d2_wrapper f1D    = { &s, &F, &aorg, &step, &atmp };
    const bool expand = 0 != (flags&Y_GLS_EXPAND);

    Y_GLS_PRINTLN("####### initialized: p=" << p << ", lambda=" << lambda );
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
    //ios::ocstream::overwrite("d2.dat");
    size_t cycle      = 0;      // cycle indication
    bool   decreasing = true;   // is lambda decreasing?
CYCLE:
    ++cycle;
    ORDINATE D2_org = s.D2(alpha,beta,F,G,aorg,U);
    Y_GLS_PRINTLN("-------- <run@cycle=" << cycle << "> -------- " );
    Y_GLS_PRINTLN("D2_org  = " << D2_org << " | beta=" << beta);
    
    //--------------------------------------------------------------------------
    //
    // compute curvature with lamba increase, then compute the step.
    // The step has some numerical noise when small
    //
    //--------------------------------------------------------------------------
COMPUTE_STEP:
    if(!compute_step(decreasing))
    {
        // here, a singular curvature is met
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
    if(sigma<0)
    {
        Y_GLS_PRINTLN("<negative sigma>");
        goto COMPUTE_STEP;
    }
    
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
    Y_GLS_PRINTLN("D2_try  = " << D2_try << " @ lambda=10^" << p << ", decreasing = " << textual::boolean(decreasing));
    
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

        //----------------------------------------------------------------------
        // update position
        //----------------------------------------------------------------------
#if 1
        {
            ios::ocstream fp("d2.dat",false);
            for(ORDINATE u=0;u<=2;u+=ORDINATE(0.02))
            {
                fp("%.15g %.15e %.15e\n",u,f1D(u),D2_org - sigma*u);
            }
            fp << '\n';
        }

#endif

        if(expand)
        {
            //------------------------------------------------------------------
            // try speed up decreasing by look up
            //------------------------------------------------------------------

            // setup triplets
            triplet<ORDINATE> u = { 0, 1, ufac };
            triplet<ORDINATE> f = { D2_org, D2_try, f1D(u.c) };

            // bracket minimum
            while(f.c<f.b)
            {
                u.c *= ufac;
                f.c  = f1D(u.c);
            }

            // reduce interval
            do {
                minimize::__step(f1D, u, f);
            } while( u.c-u.a > 1e-2 );

            // compute new point@ atry and recompute step
            D2_try = f1D(u.b);
            for(size_t i=M;i>0;--i)
            {
                atry[i] = atmp[i];
                step[i] = atry[i]-aorg[i];
            }
            Y_GLS_PRINTLN("D2_opt  = " << D2_try);

        }

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

        // testing variable convergence
        if(converged)
        {
            Y_GLS_PRINTLN("<variables convergence>");
            goto CONVERGED;
        }
        
        
        // upon decreasing, test D2 convergence
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
    Y_GLS_PRINTLN("lambda    = " << lambda);
    
    //--------------------------------------------------------------------------
    //
    // final D2
    //
    //--------------------------------------------------------------------------
    D2_org = s.D2(alpha,beta,F,G,aorg,used);
    
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

    //--------------------------------------------------------------------------
    //
    // set new parameters
    //
    //--------------------------------------------------------------------------
    tao::set(A,aorg);
    
    
    //--------------------------------------------------------------------------
    //
    // compute d.o.f
    //
    //--------------------------------------------------------------------------
    size_t       ndof = s.count();
    const size_t nuse = s.vars.used_in(used);

    
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
        tao::ld(E,s.zero);
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
            if(used[i])
            {
                E[i] = sqrt_of( D2_org * max_of<ORDINATE>(0,covar[i][i]) / n2 );
            }
            else
            {
                E[i] = s.zero;
            }
        }
        if(verbose)
        {
            //s.vars.display(std::cerr,A,E," \\pm ","\t(*) ","");
        }
        return true;
    }
    
}
