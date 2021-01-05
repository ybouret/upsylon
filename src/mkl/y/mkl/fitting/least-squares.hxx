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

    //--------------------------------------------------------------------------
    //
    // variables
    //
    //--------------------------------------------------------------------------
    assert(A.size()==U.size());
    M      = A.size();
    p      = 0;
    lambda = lam[p];

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
    size_t cycle     =0;
    bool   decreasing=true;
CYCLE:
    ++cycle;
    ORDINATE D2_org = s.D2(alpha,beta,F,G,aorg,U);
    Y_GLS_PRINTLN("-------- <run@cycle=" << cycle << "> -------- " );
    Y_GLS_PRINTLN("D2_org  = " << D2_org << " | beta=" << beta);

    //--------------------------------------------------------------------------
    //
    // compute curvature with lamba increase
    //
    //--------------------------------------------------------------------------
COMPUTE_STEP:
    if(!compute_step(decreasing))
    {
        return false;
    }

    //--------------------------------------------------------------------------
    //
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
    for(size_t i=M;i>0;--i)
    {
        const ORDINATE a_new = atry[i];
        const ORDINATE a_old = aorg[i];
        const ORDINATE da    = fabs_of( step[i] = a_new - a_old );
        if( da > numeric<double>::ftol * max_of( fabs_of(a_new), fabs_of(a_old) ) )
        {
            converged = false;
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

        if(converged)
        {
            Y_GLS_PRINTLN("<variables convergence>");
            goto CONVERGED;
        }

        if(decreasing)
        {
            const ORDINATE dd = fabs_of(D2_org-D2_try);
            if( dd <= numeric<ORDINATE>::sqrt_ftol * max_of(D2_org,D2_try) )
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
