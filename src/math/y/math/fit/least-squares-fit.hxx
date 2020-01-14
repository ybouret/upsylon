//______________________________________________________________________________
//
//! full fit algorithm and error computation
//______________________________________________________________________________
inline bool fit(SampleType<T>            &sample,
                Sequential<T>            &F,
                addressable<T>           &aorg,
                const accessible<bool>   &flags,
                addressable<T>           &aerr,
                Controls                 *ctrl = 0,
                const size_t              nmax = 0)
{
    static const T D_FTOL = numeric<T>::sqrt_ftol;
    static const T A_FTOL = numeric<T>::ftol;

    assert( flags.size() == aorg.size() );
    assert( flags.size() == aerr.size() );
    const size_t n          = aorg.size();
    const bool   checkCycle = (nmax>0);

    //__________________________________________________________________________
    //
    // Administrativia
    //
    Y_LS_PRINTLN( "[LS] initializing, #parameters=" << n );
    //
    //__________________________________________________________________________
    sample.ready();                             // prepare the sample
    atom::ld(aerr,-1);                          // invalidate errors
    if(n<=0)                                    //
    {                                           //
        Y_LS_PRINTLN("[LS] <no parameters>");   //
        return true;                            //
    }                                           //
    allocateFor(sample,flags);                  // allocate memory and setup used
    setLambda( Algo<T>::Initial() );            // assign
    Y_LS_PRINTLN( "     flags  = " << flags );
    Y_LS_PRINTLN( "     used   = " << used  );


    //__________________________________________________________________________
    //
    // create the frames
    //__________________________________________________________________________
    size_t    cycle = 0;
    Frame<T>  stepFrame(sample,aorg,used,step);
    Control  &stepControl = ( (0!=ctrl) && (0!=ctrl->step) ) ? *(ctrl->step) : nope;

    Frame<T>   atryFrame(sample,aorg,used,atry);
    Control   &atryControl = ( (0!=ctrl) && (0!=ctrl->atry) ) ? *(ctrl->atry) : nope;


    //__________________________________________________________________________
    //
    // create the call function
    //__________________________________________________________________________
    ProbeD2 D2 = { &sample, &F, &atry, &aorg, &step, };


    //__________________________________________________________________________
    //
    //
    // Effective Algorithm Starts now!
    //
    //__________________________________________________________________________
    T      D2org = sample.computeD2(alpha, beta, F, aorg, used, *this);
    size_t nbad  = 0; // number of successive bad steps

CYCLE:
    ++cycle;

    //__________________________________________________________________________
    //
    Y_LS_PRINTLN( "[LS] cycle  = " << cycle );
    //__________________________________________________________________________
    if(checkCycle&& (cycle>nmax) )
    {
        Y_LS_PRINTLN( "[LS] done " << nmax << " cycles" );
        goto CONVERGED;
    }
    Y_LS_PRINTLN( "     used   = " << used  );
    Y_LS_PRINTLN( "     aorg   = " << aorg  );
    Y_LS_PRINTLN( "     D2org  = " << D2org );
    Y_LS_PRINTLN( "     alpha  = " << alpha );
    Y_LS_PRINTLN( "     beta   = " << beta  );


    if(!buildStep())
    {
        return false;
    }
    
    atom::add(atry,aorg,step);
    Y_LS_PRINTLN( "     lambda = " << lambda );
    Y_LS_PRINTLN( "     step0  = " << step   );
    Y_LS_PRINTLN( "     atry0  = " << atry   );
    //__________________________________________________________________________



    //__________________________________________________________________________
    //
    // step/atry consistency
    //
    //
STEP_CONTROL:
    switch( stepControl(stepFrame,cycle) )
    {
        case LeftUntouched: break;
        case EmergencyExit: Y_LS_PRINTLN( "[LS] <STEP #" << cycle << " CONTROL EXIT>" ); return false;
        case ModifiedValue: Y_LS_PRINTLN( "[LS] changed step #" << cycle );
            atom::add(atry,aorg,step);
            break;
    }


    switch( atryControl(atryFrame,cycle) )
    {
        case LeftUntouched: break;
        case EmergencyExit: Y_LS_PRINTLN( "[LS] <ATRY #" << cycle << " CONTROL EXIT>" ); return false;
        case ModifiedValue: Y_LS_PRINTLN( "[LS] changed atry #" << cycle );
            atom::sub(step,atry,aorg);
            Y_LS_PRINTLN( "     atry1  = " << atry   );
            goto STEP_CONTROL;
    }
    //
    //
    //__________________________________________________________________________


    //__________________________________________________________________________
    //
    //
    // at this point, aorg, atry and step are computed
    // now pinpoint a local minimum along step
    // and we assume all values in [aorg,atry] are ok
    // a first trial is taken to probe the local topology
    //
    //__________________________________________________________________________
    {
        // this is in a block to bypass the nmax condition
        triplet<T> u  = { 0,     numeric<T>::inv_gold, 1 };
        triplet<T> f  = { D2org, D2(u.b),             -1 };
        bool       ok = true;



        if(f.b>f.a)
        {
            //------------------------------------------------------------------
            //
            // D2(u.b)>D2(u.a)
            // first trial is invalid, won't go further !
            //
            //------------------------------------------------------------------
            Y_LS_PRINTLN( "[LS] Backtrack Level-1" );
            ok  = false;

            f.c = f.b;                   // initialize an inside bracketing: f.c <- f.b
            u.c = u.b;                   // initialize an inside bracketing: u.c <- u.c
            bracket::inside(D2,u,f);     // bracket/min
            (void)minimize::run(D2,u,f); // minimize
            if( f.b > f.a )
            {
                Y_LS_PRINTLN("[LS] <BAD FUNCTION>");
                return false;
            }
            assert(f.b<=f.a);
        }
        else
        {
            //------------------------------------------------------------------
            //
            // D2(u.b) <= D2(u.a), take next step at u.c
            //
            //------------------------------------------------------------------
            f.c = D2(u.c);
            if(f.c>f.a)
            {
                //--------------------------------------------------------------
                //
                // D2(u.c=1)>D2(u.a=0) : invalid full step
                // but minimum is already bracketed!
                //
                //--------------------------------------------------------------
                ok = false;
                Y_LS_PRINTLN( "[LS] Backtrack Level-2" );
                (void)minimize::run(D2,u,f);
                assert(f.b<=f.a);
            }
            else
            {
                //--------------------------------------------------------------
                //
                // D2(u.c=1)<=D2(u.a=0), accept full step
                //
                //--------------------------------------------------------------
                if(f.b<=f.c)
                {
                    // Damping! we met a lower point
                    (void)minimize::run(D2,u,f);
                    assert(f.b<=f.a);
                }
                else
                {
                    // regular full step
                    u.b = u.c;
                    f.b = f.c;
                    assert(f.b<=f.a);
                }
            }
        }

        //----------------------------------------------------------------------
        //
        //
        // u.b is the optimized step fraction
        //
        //----------------------------------------------------------------------

        //----------------------------------------------------------------------
        // compute the new position
        //----------------------------------------------------------------------
        atom::setprobe(atry,aorg,u.b,step);

        //----------------------------------------------------------------------
        // check variable convergence
        //----------------------------------------------------------------------
        bool converged_variables = true;
        for(size_t i=n;i>0;--i)
        {
            if(!used[i]) continue;
            const T a_old = aorg[i];
            const T a_new = atry[i];
            const T da    = fabs_of( a_new-a_old );
            if( da > A_FTOL * max_of( fabs_of(a_new), fabs_of(a_old) ) )
            {
                converged_variables = false;
                break;
            }
        }

        //----------------------------------------------------------------------
        // update aorg/D2org and update gradient/curvature
        //----------------------------------------------------------------------
        atom::set(aorg,atry);
        const T D2old = D2org;
        D2org = sample.computeD2(alpha, beta, F, aorg, used, *this);

        Y_LS_PRINTLN("[LS] D2: " << D2old << " -> " << D2org );
        Y_LS_PRINTLN("[LS] converged variables = <" << converged_variables << ">");
        if(ok)
        {
            //------------------------------------------------------------------
            // check D2 convergence
            //------------------------------------------------------------------
            const bool converged_squares = (fabs_of(D2org-D2old) <= D_FTOL * D2old);
            Y_LS_PRINTLN("[LS] converged squares   = <" << converged_squares   << ">");
            if( converged_variables || converged_squares  )
            {
                goto CONVERGED;
            }
            decreaseLambda();
            nbad = 0;
        }
        else
        {
            //------------------------------------------------------------------
            // check variable convergence
            //------------------------------------------------------------------
            ++nbad;
            if( (nbad>0) && converged_variables )
            {
                Y_LS_PRINTLN("[LS] spurious convergence");
                goto CONVERGED;
            }

            if( !increaseLambda() )
            {
                Y_LS_PRINTLN("[LS] <OVERFLOW>");
                return false;
            }
        }
        goto CYCLE;


    }

CONVERGED:
    //--------------------------------------------------------------------------
    //
    Y_LS_PRINTLN( "[LS] -- Converged/Done --" );
    Y_LS_PRINTLN( "     D2   = " << D2org );
    Y_LS_PRINTLN( "     aorg = " << aorg  );
    Y_LS_PRINTLN( "     used = " << used  );
    //
    //--------------------------------------------------------------------------
    if(!LU::build(alpha))
    {
        Y_LS_PRINTLN( "[LS] <SINGULAR Level-2>" );
        return false;
    }
    LU::inverse(alpha,curv);

    //--------------------------------------------------------------------------
    //
    unit_t dof = sample.count();
    Y_LS_PRINTLN( "    count = " << dof );
    //--------------------------------------------------------------------------
    for(size_t i=n;i>0;--i)
    {
        if(used[i]) --dof;
    }
    Y_LS_PRINTLN( "      dof = " << dof );
    if(dof<0)
    {
        Y_LS_PRINTLN( "[LS] <MEANINGLESS>" );
        return false;
    }
    else if(0==dof)
    {
        Y_LS_PRINTLN( "[LS] <interpolation>" );
        atom::ld(aerr,0);
        return true;
    }
    else
    {
        assert(dof>0);
        const T real_count = T(dof);
        const T sig2err    = T(1)/sqrt_of(real_count);
        for(size_t i=n;i>0;--i)
        {
            if(used[i])
            {
                const T sig = sqrt_of( (D2org * curv[i][i]) / dof );
                aerr[i]     = sig*sig2err;
            }
            else
            {
                aerr[i] = 0;
            }
        }
        Y_LS_PRINTLN( "     aerr = " << aerr );
        return true;
    }



}
