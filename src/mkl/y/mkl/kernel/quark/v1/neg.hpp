//! \file

//! neg items with cast
#define Y_QUARK_NEG(I) a[I] = -Y_QUARK_CAST(TARGET,SOURCE,b[I])

//! SEQUENTIAL: a=-b
template <typename TARGET, typename SOURCE> static inline
void neg(TARGET &a, SOURCE &b)
{
    assert(a.size()<=b.size());
    Y_QUARK_LOOP_SEQ(a.size(),NEG);
}

//! SEQUENTIAL: a=-a
template <typename TARGET> static inline
void neg(TARGET &a)
{
    neg(a,a);
}

//! PARALLEL: a=-b
template <typename TARGET, typename SOURCE> static inline
void neg(TARGET &a, SOURCE &b, concurrent::for_each &loop)
{
    assert(a.size()<=b.size());

    Y_QUARK_TASK_DECL()
    TARGET *a;
    SOURCE *b;
    Y_QUARK_TASK_IMPL()
    TARGET &a     = *task.a;
    SOURCE &b     = *task.b;
    Y_QUARK_LOOP_PAR(a.size(),NEG);
    Y_QUARK_TASK_DATA()
    &a, &b
    Y_QUARK_TASK_EXEC(loop);

}

//! PARALLEL: a=-a
template <typename TARGET> static inline
void neg(TARGET &a, concurrent::for_each &loop)
{
    neg(a,a,loop);
}

#undef Y_QUARK_NEG
