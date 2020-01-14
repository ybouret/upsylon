
#define Y_QUARK_SET(I) a[I] = Y_QUARK_CAST(TARGET,SOURCE,b[I])

//! SEQUENTIAL: a=b
template <typename TARGET, typename SOURCE> static inline
void set( TARGET &a, SOURCE &b )
{
    assert(a.size()<=b.size());
    Y_QUARK_LOOP(a.size(),SET,1);
}


//! PARALLEL: a=b
template <typename TARGET, typename SOURCE> static inline
void set( TARGET &a, SOURCE &b, concurrent::for_each &loop)
{
    assert(a.size()<=b.size());

    Y_QUARK_TASK_DECL()
    TARGET *a;
    SOURCE *b;
    Y_QUARK_TASK_IMPL()
    TARGET &a     = *task.a;
    SOURCE &b     = *task.b;
    size_t offset = 1;
    size_t length = a.size();
    ctx.split(length,offset);
    Y_QUARK_LOOP(length,SET,offset);
    Y_QUARK_TASK_DATA()
    &a, &b
    Y_QUARK_TASK_EXEC(loop);

}
#undef Y_QUARK_SET

#define Y_QUARK_VEC(I) tgt[I] = Y_QUARK_CAST(TARGET,RHS,rhs[I]) - Y_QUARK_CAST(TARGET,LHS,lhs[I])

//! SEQUENTIAL: tgt=rhs-lhs
template <typename TARGET, typename LHS, typename RHS> static inline
void vec( TARGET &tgt, LHS &lhs, RHS &rhs )
{
    assert(tgt.size()<=lhs.size());
    assert(tgt.size()<=rhs.size());
    Y_QUARK_LOOP(tgt.size(),VEC,1);
}


//! PARALLEL: tgt=rhs-lhs
template <typename TARGET, typename LHS, typename RHS> static inline
void vec( TARGET &tgt, LHS &lhs, RHS &rhs, concurrent::for_each &loop )
{
    assert(tgt.size()<=lhs.size());
    assert(tgt.size()<=rhs.size());

    Y_QUARK_TASK_DECL()
    TARGET *tgt;
    LHS    *lhs;
    RHS    *rhs;
    Y_QUARK_TASK_IMPL()
    TARGET &tgt   = *task.a;
    LHS    &lhs   = *task.lhs;
    RHS    &rhs   = *task.rhs;
    size_t offset = 1;
    size_t length = tgt.size();
    ctx.split(length,offset);
    Y_QUARK_LOOP(length,VEC,offset);
    Y_QUARK_TASK_DATA()
    &tgt, &lhs, &rhs
    Y_QUARK_TASK_EXEC(loop);
}

#undef Y_QUARK_VEC
