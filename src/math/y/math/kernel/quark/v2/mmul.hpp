//! \file

//! mmul with cast
#define Y_QUARK_mmul(R) lhs[R] = dot< typename LHS::type >::of( M[R], rhs );

template <typename T, typename LHS, typename RHS> static inline
void mmul( LHS &lhs, const matrix<T> &M, RHS &rhs )
{
    assert(lhs.size()>=M.rows);
    Y_QUARK_LOOP_SEQ(M.rows,mmul);
}

template <typename T, typename LHS, typename RHS> static inline
void mmul( LHS &lhs, const matrix<T> &M, RHS &rhs, concurrent::for_each &loop )
{
    assert(lhs.size()>=M.rows);

    Y_QUARK_TASK_DECL()
    LHS       *lhs;
    matrix<T> *M;
    RHS       *rhs;
    Y_QUARK_TASK_IMPL()
    LHS         &lhs = *task.lhs;
    matrix<T>   &M   = *task.M;
    RHS         &rhs = *task.rhs;
    Y_QUARK_LOOP_PAR(M.rows,mmul);
    Y_QUARK_TASK_DATA()
    &lhs, &M, &rhs
    Y_QUARK_TASK_EXEC(loop);

}


#undef Y_QUARK_mmul
