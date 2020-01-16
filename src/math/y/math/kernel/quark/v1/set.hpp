//! \file

//! set item with cast
#define Y_QUARK_SET(I) a[I] = Y_QUARK_CAST(TARGET,SOURCE,b[I])

//! SEQUENTIAL: a=b
template <typename TARGET, typename SOURCE> static inline
void set( TARGET &a, SOURCE &b )
{
    assert(a.size()<=b.size());
    Y_QUARK_LOOP_SEQ(a.size(),SET);
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
    Y_QUARK_LOOP_PAR(a.size(),SET);
    Y_QUARK_TASK_DATA()
    &a, &b
    Y_QUARK_TASK_EXEC(loop);

}
#undef Y_QUARK_SET

//! mulitply and set item with cast
#define Y_QUARK_MULSET(I) a[I] = x * Y_QUARK_CAST(TARGET,SOURCE,b[I])

//! SEQUENTIAL: a=x*b
template <typename TARGET, typename SOURCE> static inline
void mulset( TARGET &a, typename TARGET::param_type x, SOURCE &b )
{
    assert(a.size()<=b.size());
    Y_QUARK_LOOP_SEQ(a.size(),MULSET);
}


//! PARALLEL: a=x*b
template <typename TARGET, typename SOURCE> static inline
void mulset( TARGET &a, typename TARGET::param_type x, SOURCE &b, concurrent::for_each &loop)
{
    assert(a.size()<=b.size());

    Y_QUARK_TASK_DECL()
    TARGET *a;
    SOURCE *b;
    typename TARGET::const_type *x;
    Y_QUARK_TASK_IMPL()
    TARGET &a     = *task.a;
    SOURCE &b     = *task.b;
    typename TARGET::const_type x = *task.x;
    Y_QUARK_LOOP_PAR(a.size(),MULSET);
    Y_QUARK_TASK_DATA()
    &a, &b, &x
    Y_QUARK_TASK_EXEC(loop);

}
#undef Y_QUARK_MULSET
