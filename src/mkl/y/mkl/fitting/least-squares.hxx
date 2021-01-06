//! \file


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
    static const ORDINATE vtol = numeric<ORDINATE>::ftol;
    static const ORDINATE dtol = numeric<ORDINATE>::sqrt_ftol;

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
    used.adjust(M,false);
    tao::set(aorg,A);
    tao::set(used,U);
    tao::ld(E,-1);
    s.setup(A);

    Y_GLS_PRINTLN("####### initialized: p=" << p << ", lambda=" << lambda );
    if(verbose)
    {
        s.vars.display(std::cerr,aorg,used,", used = ","\t(--) ","");
    }

    //--------------------------------------------------------------------------
    //
    //
    // cycle
    //
    //
    //--------------------------------------------------------------------------
    size_t cycle      = 0;
    bool   decreasing = true;
CYCLE:
    ++cycle;
    ORDINATE D2_org = s.D2(alpha,beta,F,G,aorg,U);
    Y_GLS_PRINTLN("-------- <run@cycle=" << cycle << "> -------- " );
    Y_GLS_PRINTLN("D2_org  = " << D2_org << " | beta=" << beta);

    //--------------------------------------------------------------------------
    //
    // compute curvature with lamba increase, then compute the step
    // the step has a part of errors, since it's numerical around zero
    //
    //--------------------------------------------------------------------------
COMPUTE_STEP:
    if(!compute_step(decreasing))
    {
        // singular curvature
        return false;
    }



    //--------------------------------------------------------------------------
    //
    // At this point, the curvature is not singular, so
    // compute atry
    //
    //--------------------------------------------------------------------------
    for(size_t i=M;i>0;--i)
    {
        atry[i] = aorg[i] + step[i];
    }

    //--------------------------------------------------------------------------
    //
    // TODO: control atry
    //
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    //
    // recompute step and check variable convergence for later
    //
    //--------------------------------------------------------------------------
    bool converged = true;
    {
        size_t i=M;
        for(;i>0;--i)
        {
            const ORDINATE a_new = atry[i];
            const ORDINATE a_old = aorg[i];
            const ORDINATE da    = fabs_of( step[i] = a_new - a_old );
            if( da > vtol * max_of( fabs_of(a_new), fabs_of(a_old) ) )
            {
                converged = false;
                --i;
                break;
            }
        }

        for(;i>0;--i)
        {
            const ORDINATE a_new = atry[i];
            const ORDINATE a_old = aorg[i];
            step[i] = a_new - a_old;
        }

    }
    

    //--------------------------------------------------------------------------
    //
    // compute trial D2
    //
    //--------------------------------------------------------------------------
    const ORDINATE D2_try = s.D2(F,atry);
    Y_GLS_PRINTLN("D2_try  = " << D2_try << " @ lambda=10^" << p << ", decreasing = " << textual::boolean(decreasing));

    if(D2_try>D2_org)
    {
        //----------------------------------------------------------------------
        // reject
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
        // accept
        //----------------------------------------------------------------------
        Y_GLS_PRINTLN("<accept>");
        tao::set(aorg,atry);
        decrease();

        if(verbose)
        {
            s.vars.display(std::cerr,atry,step," (","\t(->) ",")");
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
    //Y_GLS_PRINTLN("covar    = " << covar);

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
    size_t ndof = s.count();
    size_t nuse = 0;
    for(size_t i=M;i>0;--i)
    {
        if(used[i]) ++nuse;
    }

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
            s.vars.display(std::cerr,A,E," \\pm ","\t(*) ","");
        }
        return true;
    }

}
