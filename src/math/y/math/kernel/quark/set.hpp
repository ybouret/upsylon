
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

#define Y_QUARK_MULSET(I) a[I] = x * Y_QUARK_CAST(TARGET,SOURCE,b[I])

//! SEQUENTIAL: a=x*b
template <typename TARGET, typename SOURCE> static inline
void mul_set( TARGET &a, typename TARGET::param_type x, SOURCE &b )
{
    assert(a.size()<=b.size());
    Y_QUARK_LOOP(a.size(),MULSET,1);
}


//! PARALLEL: a=x*b
template <typename TARGET, typename SOURCE> static inline
void mul_set( TARGET &a, typename TARGET::param_type x, SOURCE &b, concurrent::for_each &loop)
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
    size_t offset = 1;
    size_t length = a.size();
    ctx.split(length,offset);
    Y_QUARK_LOOP(length,MULSET,offset);
    Y_QUARK_TASK_DATA()
    &a, &b, &x
    Y_QUARK_TASK_EXEC(loop);

}
#undef Y_QUARK_MULSET
