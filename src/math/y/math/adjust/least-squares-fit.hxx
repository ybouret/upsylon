
//! full fit algorithm and error computation
inline bool fit(SampleType<T>            &sample,
                Sequential<T>            &F,
                addressable<T>           &aorg,
                const accessible<bool>   &flags,
                addressable<T>           &aerr,
                Modify                   *modify = 0 )
{
    static const T D_FTOL = numeric<T>::sqrt_ftol;
    static const T A_FTOL = numeric<T>::ftol;

    assert( flags.size() == aorg.size() );
    assert( flags.size() == aerr.size() );

    //__________________________________________________________
    //
    Y_LS_PRINTLN( "[LS] initializing" );
    //__________________________________________________________
    sample.ready();
    atom::ld(aerr,-1);
    const size_t n = aorg.size();
    if(n<=0)
    {
        Y_LS_PRINTLN("[LS] <no parameters>");
        return true;
    }

    //__________________________________________________________
    //
    Y_LS_PRINTLN( "[LS] starting with #parameters=" << n);
    //__________________________________________________________
    allocateFor(sample,flags);
    setLambda( Algo<T>::Initial() );

    Y_LS_PRINTLN( "     flags  = " << flags );
    Y_LS_PRINTLN( "     used   = " << used  );

    //__________________________________________________________
    //
    // create the call function
    //__________________________________________________________
    ProbeD2 D2 = { &sample, &F, &atry, &aorg, &step};

    //__________________________________________________________
    //
    // create the context
    //__________________________________________________________
    Context<T> context(sample,aorg,used,atry,step);
    size_t     cycle = aliasing::_(context.cycle);
    Modify    &check = (0!=modify) ? *modify : nope;
    Y_LS_PRINTLN( "    #data   = " << context.size()  );

    // starting point...
    T      D2org = sample.computeD2(alpha, beta, F, aorg, used, *this);
    size_t nbad  = 0;

CYCLE:
    ++cycle;
    //__________________________________________________________
    //
    Y_LS_PRINTLN( "[LS] cycle  = " << cycle );

    //__________________________________________________________



    //__________________________________________________________
    //
    Y_LS_PRINTLN( "     used   = " << used  );
    Y_LS_PRINTLN( "     aorg   = " << aorg );
    Y_LS_PRINTLN( "     D2org  = " << D2org );
    Y_LS_PRINTLN( "     alpha  = " << alpha );
    Y_LS_PRINTLN( "     beta   = " << beta );
    while( !Algo<T>::ComputeCurvature(curv,lambda,alpha) )
    {
        if(!increaseLambda())
        {
            Y_LS_PRINTLN( "[LS] <SINGULAR GRADIENT>" );
            return false;
        }
    }
    //__________________________________________________________

    //__________________________________________________________
    //
    // compute step and trial position
    //
    Algo<T>::ComputeStep(step, curv, beta);
    atom::add(atry,aorg,step);
    Y_LS_PRINTLN( "     lambda = " << lambda );
    Y_LS_PRINTLN( "     step0  = " << step   );
    Y_LS_PRINTLN( "     atry0  = " << atry   );
    //__________________________________________________________



    //__________________________________________________________
    //
    // user control
    switch( check(context) )
    {
        case LeftUntouched: //Y_LS_PRINTLN( "[LS] LeftUntouched" );
            break;

        case ModifiedState: Y_LS_PRINTLN( "[LS] ModifiedState" );
            atom::sub(step,atry,aorg);
            Y_LS_PRINTLN( "     atry   = " << atry   );
            Y_LS_PRINTLN( "     step   = " << step   );
            break;

        case ModifiedShift: Y_LS_PRINTLN( "[LS] ModifiedShift" );
            atom::add(atry,aorg,step);
            Y_LS_PRINTLN( "     atry   = " << atry   );
            Y_LS_PRINTLN( "     step   = " << step   );
            break;

        case EmergencyExit: Y_LS_PRINTLN( "[LS] EmergencyExit" );
            return false;
    }
    //
    //__________________________________________________________



    //__________________________________________________________
    //
    //
    // at this point, aorg, atry and step are computed
    // now pinpoint a local minimum
    //
    //__________________________________________________________
    triplet<T> u  = { 0,     numeric<T>::inv_gold, 1 };
    triplet<T> f  = { D2org, D2(u.b),             -1 };
    bool       ok = true;

    if(f.b>f.a)
    {
        //------------------------------------------------------
        //
        // D2(u.b)>D2(u.a)
        // first trial is invalid, won't go further !
        //
        //------------------------------------------------------
        Y_LS_PRINTLN( "[LS] Backtrack Level-1" );
        ok  = false;
        f.c = f.b;
        u.c = u.b;
        bracket::inside(D2,u,f);
        (void)minimize::run(D2,u,f);
        if( f.b > f.a )
        {
            Y_LS_PRINTLN("[LS] <BAD FUNCTION>");
            return false;
        }
        assert(f.b<=f.a);
    }
    else
    {
        //------------------------------------------------------
        //
        // D2(u.b) <= D2(u.a), take next step at u.c
        //
        //------------------------------------------------------
        f.c = D2(u.c);
        if(f.c>f.a)
        {
            //--------------------------------------------------
            //
            // D2(u.c=1)>D2(u.a=0) : invalid full step
            // but minimum is already bracketed!
            //
            //--------------------------------------------------
            ok = false;
            Y_LS_PRINTLN( "[LS] Backtrack Level-2" );
            (void)minimize::run(D2,u,f);
            assert(f.b<=f.a);
        }
        else
        {
            //--------------------------------------------------
            //
            // D2(u.c=1)<=D2(u.a=0), accept full step
            //
            //--------------------------------------------------
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

    //__________________________________________________________
    //
    //
    // u.b is the optimized step fraction
    //
    //__________________________________________________________

    //----------------------------------------------------------
    // compute the new position
    //----------------------------------------------------------
    atom::setprobe(atry,aorg,u.b,step);

    //----------------------------------------------------------
    // check variable convergence
    //----------------------------------------------------------
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

    //----------------------------------------------------------
    // update aorg/D2org
    //----------------------------------------------------------
    atom::set(aorg,atry);
    const T D2old = D2org;
    D2org = sample.computeD2(alpha, beta, F, aorg, used, *this);

    Y_LS_PRINTLN("[LS] D2: " << D2old << " -> " << D2org );
    Y_LS_PRINTLN("[LS] converged variables = <" << converged_variables << ">");
    if(ok)
    {
        //------------------------------------------------------
        // check D2 convergence
        //------------------------------------------------------
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
        //------------------------------------------------------
        // check variable convergence
        //------------------------------------------------------
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




CONVERGED:
    //__________________________________________________________
    //
    Y_LS_PRINTLN( "[LS] -- Converged --" );
    Y_LS_PRINTLN( "     D2   = " << D2org );
    Y_LS_PRINTLN( "     aorg = " << aorg  );
    Y_LS_PRINTLN( "     used = " << used  );
    //__________________________________________________________
    if(!LU::build(alpha))
    {
        Y_LS_PRINTLN( "[LS] <SINGULAR Level-2>" );
        return false;
    }
    LU::inverse(alpha,curv);

    //__________________________________________________________
    //
    unit_t dof = sample.count();
    Y_LS_PRINTLN( "    count = " << dof );
    //__________________________________________________________
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
