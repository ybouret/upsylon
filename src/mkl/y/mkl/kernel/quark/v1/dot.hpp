//! \file

//! dot operation with cast
#define Y_QUARK_dot(I) ans += Y_QUARK_TO(T,LHS,lhs[I]) * Y_QUARK_TO(T,RHS,rhs[I])

//! dot products
template <typename T>
struct dot
{
    Y_DECL_ARGS(T,type); //!< aliases

    //! SEQUENTIAL: lhs.rhs
    template <typename LHS, typename RHS> static inline
    mutable_type of(LHS &lhs, RHS &rhs)
    {
        assert(lhs.size()<=rhs.size());
        mutable_type ans = 0;
        Y_QUARK_LOOP_SEQ(lhs.size(),dot);
        return ans;
    }

    //! parallel: lhs.rhs
    template <typename LHS, typename RHS> static inline
    mutable_type of(LHS &lhs, RHS &rhs, concurrent::for_each &loop)
    {
        assert(lhs.size()<=rhs.size());
        
        concurrent::executor &par = loop.engine();
        par.make<mutable_type>(model);

        Y_QUARK_TASK_DECL()
        LHS *lhs;
        RHS *rhs;
        Y_QUARK_TASK_IMPL()
        mutable_type ans = 0;
        LHS         &lhs = *task.lhs;
        RHS         &rhs = *task.rhs;
        Y_QUARK_LOOP_PAR(lhs.size(),dot);
        assert( 0 == (*ctx).get<mutable_type>() );
        (*ctx).get<mutable_type>() = ans;
        Y_QUARK_TASK_DATA()
        &lhs, &rhs
        Y_QUARK_TASK_EXEC(loop);
        return par.sum<mutable_type>();
    }

};


#undef Y_QUARK_dot
