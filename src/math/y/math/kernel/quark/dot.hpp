
#define Y_QUARK_dot(I) ans += Y_QUARK_TO(T,LHS,lhs[I]) * Y_QUARK_TO(T,RHS,rhs[I])

template <typename T>
struct dot
{
    Y_DECL_ARGS(T,type);

    template <typename LHS, typename RHS> static inline
    mutable_type of( LHS &lhs, RHS &rhs )
    {
        assert(lhs.size()<=rhs.size());
        mutable_type ans = 0;
        Y_QUARK_LOOP_SEQ(lhs.size(),dot);
        return ans;
    }

    template <typename LHS, typename RHS> static inline
    mutable_type of( LHS &lhs, RHS &rhs, concurrent::for_each &loop )
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
        Y_QUARK_LOOP_PAR(lhs.size(),dot);
        assert( 0 == ctx.get<mutable_type>() );
        ctx.get<mutable_type>() = ans;
        Y_QUARK_TASK_DATA()
        &lhs, &rhs
        Y_QUARK_TASK_EXEC(loop);
        return par.sum<mutable_type>();
    }

};
