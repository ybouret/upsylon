
//! LD proc
#define Y_QUARK_LD(I) arr[I] = value

//! SEQUENTIAL: load the same value
template <typename ARRAY> static inline
void ld( ARRAY &arr, typename ARRAY::param_type value)
{
    Y_QUARK_LOOP(arr.size(),LD,1);
}


//! PARALLEL: load the same value
template <typename ARRAY> static inline
void ld( ARRAY &arr, typename ARRAY::param_type value, concurrent::for_each &loop )
{
    Y_QUARK_TASK_DECL()
    ARRAY                      *arr;
    typename ARRAY::const_type *value;
    Y_QUARK_TASK_IMPL()
    ARRAY                      &arr    = *task.arr;
    typename ARRAY::const_type &value  = *task.value;
    size_t                      length = arr.size();
    size_t                      offset = 1;
    ctx.split(length,offset);
    Y_LOOP_FUNC(length,Y_QUARK_LD,offset);
    Y_QUARK_TASK_DATA()
    &arr, &value
    Y_QUARK_TASK_EXEC(loop);
}

#undef Y_QUARK_LD
