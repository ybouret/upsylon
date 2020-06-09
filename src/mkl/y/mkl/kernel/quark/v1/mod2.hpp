//! \file

//! mod2 addition
#define Y_QUARK_mod2(I)      ans += __mod2( lhs[I] )

//! mod2 of delta addition
#define Y_QUARK_mod2delta(I) ans += __mod2( lhs[I] - Y_QUARK_CAST(LHS,RHS,rhs[I]) )

//! taking mod2 of arrays
template <typename T>
struct mod2
{
    Y_DECL_ARGS(T,type);                                     //!< aliases
    typedef typename real_for<mutable_type>::type real_type; //!< alias

    //! SEQUENTIAL: |lhs|^2
    template <typename LHS> static inline
    real_type of( LHS &lhs  )
    {
        real_type ans = 0;
        Y_QUARK_LOOP_SEQ(lhs.size(),mod2);
        return ans;
    }

    //! PARALLEL: |lhs|^2
    template <typename LHS> static inline
    real_type of(LHS &lhs, concurrent::for_each &loop)
    {
        
        concurrent::executor &par = loop.engine();
        par.acquire_for<mutable_type>();

        Y_QUARK_TASK_DECL()
        LHS *lhs;
        Y_QUARK_TASK_IMPL()
        mutable_type ans = 0;
        LHS         &lhs = *task.lhs;
        Y_QUARK_LOOP_PAR(lhs.size(),mod2);
        assert( 0 == ctx.get<mutable_type>() );
        ctx.get<mutable_type>() = ans;
        Y_QUARK_TASK_DATA()
        &lhs
        Y_QUARK_TASK_EXEC(loop);
        return par.sum<mutable_type>();
    }

    //! SEQUENTIAL: |lhs-rhs|^2
    template <typename LHS, typename RHS> static inline
    real_type of(LHS &lhs, RHS &rhs)
    {
        assert(lhs.size()<=rhs.size());
        real_type ans = 0;
        Y_QUARK_LOOP_SEQ(lhs.size(),mod2delta);
        return ans;
    }

    //! PARALLEL: |lhs-rhs|^2
    template <typename LHS, typename RHS> static inline
    mutable_type of(LHS &lhs, RHS &rhs, concurrent::for_each &loop)
    {
        assert(lhs.size()<=rhs.size());

        concurrent::executor &par = loop.engine();
        par.acquire_for<mutable_type>();

        Y_QUARK_TASK_DECL()
        LHS *lhs;
        RHS *rhs;
        Y_QUARK_TASK_IMPL()
        mutable_type ans = 0;
        LHS         &lhs = *task.lhs;
        RHS         &rhs = *task.rhs;
        Y_QUARK_LOOP_PAR(lhs.size(),mod2delta);
        assert( 0 == ctx.get<mutable_type>() );
        ctx.get<mutable_type>() = ans;
        Y_QUARK_TASK_DATA()
        &lhs, &rhs
        Y_QUARK_TASK_EXEC(loop);
        return par.sum<mutable_type>();
    }


};

#undef Y_QUARK_mod2


