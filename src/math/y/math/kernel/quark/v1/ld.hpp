//! \file

//! LD proc
#define Y_QUARK_LD(I) arr[I] = value

//! SEQUENTIAL: load the same value
template <typename ARRAY> static inline
void ld(ARRAY &arr, typename ARRAY::param_type value)
{
    Y_QUARK_LOOP_SEQ(arr.size(),LD);
}


//! PARALLEL: load the same value
template <typename ARRAY> static inline
void ld(ARRAY &arr, typename ARRAY::param_type value, concurrent::for_each &loop)
{
    Y_QUARK_TASK_DECL()
    ARRAY                      *arr;
    typename ARRAY::const_type *value;
    Y_QUARK_TASK_IMPL()
    ARRAY                      &arr    = *task.arr;
    typename ARRAY::const_type &value  = *task.value;
    Y_QUARK_LOOP_PAR(arr.size(),LD);
    Y_QUARK_TASK_DATA()
    &arr, &value
    Y_QUARK_TASK_EXEC(loop);
}

#undef Y_QUARK_LD
