
template <typename ARRAY> static inline
void ld( ARRAY &arr, typename ARRAY::param_type value)
{
    for(size_t i=arr.size();i>0;--i)
    {
        arr[i] = value;
    }
}




#define Y_MK_ATOM_LD(I) a[I] = *self.value_

template <typename ARRAY> static inline
void ld( ARRAY &arr, typename ARRAY::param_type value, concurrent::for_each &loop)
{
    
    Y_MK_ATOM_OPS_API()
    ARRAY                      *array_;
    typename ARRAY::const_type *value_;
    Y_MK_ATOM_OPS_GET(self);
    ARRAY &a      = *self.array_;
    Y_MK_ATOM_OPS_USE(a.size(),LD)
    &arr, &value
    Y_MK_ATOM_OPS_RUN(loop);
    
}

#undef Y_MK_ATOM_LD
