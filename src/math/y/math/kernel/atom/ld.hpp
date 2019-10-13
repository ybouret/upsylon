//! \file

//! SIMD kernel
#define Y_MK_ATOM_LD(I) arr[I] = value

//! sequential arr = [value..value]
template <typename ARRAY> static inline
void ld( ARRAY &arr, typename ARRAY::param_type value)
{
    for(size_t i=arr.size();i>0;--i)
    {
        Y_MK_ATOM_LD(i);
    }
}




//! parallel arr = [value..value]
template <typename ARRAY> static inline
void ld( ARRAY &arr, typename ARRAY::param_type value, concurrent::for_each &loop)
{
    
    Y_MK_ATOM_OPS_API()
    ARRAY                      *array_;
    typename ARRAY::const_type *value_;
    Y_MK_ATOM_OPS_GET(self);
    ARRAY &arr                        = *self.array_;
    typename ARRAY::const_type &value = *self.value_;
    Y_MK_ATOM_OPS_USE(arr.size(),LD)
    &arr, &value
    Y_MK_ATOM_OPS_RUN(loop);
    
}

#undef Y_MK_ATOM_LD
