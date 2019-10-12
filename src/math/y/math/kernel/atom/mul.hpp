//! \file

//! sequential array *= value
template <typename ARRAY> static inline
void mul_by( typename ARRAY::param_type value, ARRAY &arr )
{
    for(size_t i=arr.size();i>0;--i)
    {
        arr[i] *= value;
    }
}


//! SIMD code
#define Y_MK_ATOM_MUL_BY(I) a[I] *= *self.value_

//! parallel array *= value
template <typename ARRAY> static inline
void mul_by( typename ARRAY::param_type value, ARRAY &arr,  concurrent::for_each &loop)
{
    
    Y_MK_ATOM_OPS_API()
    ARRAY                      *array_;
    typename ARRAY::const_type *value_;
    Y_MK_ATOM_OPS_GET(self);
    ARRAY &a      = *self.array_;
    Y_MK_ATOM_OPS_USE(a.size(),MUL_BY)
    &arr, &value
    Y_MK_ATOM_OPS_RUN(loop);
    
}
#undef Y_MK_ATOM_MUL_BY

struct mulop
{
    template <typename T,typename U> static inline
    void __set( T &target, const T value, const U &source)
    {
        target = value * static_cast<T>( source );
    }
    
    template <typename T,typename U> static inline
    void __add( T &target, const T value, const U &source)
    {
        target += value * static_cast<T>( source );
    }
    
    template <typename T,typename U> static inline
    void __sub( T &target, const T value, const U &source)
    {
        target -= value * static_cast<T>( source );
    }
    
};

template <
typename TARGET,
typename SOURCE,
typename MUL_OP> static inline
void mul_( TARGET &target, typename TARGET::param_type value, const SOURCE &source, MUL_OP &op )
{
    assert(target.size()<=source.size());
    for(size_t i=target.size();i>0;--i)
    {
        op( target[i], value, source[i] );
    }
}

//! sequential target += value * source
template <typename TARGET, typename SOURCE> static inline
void muladd( TARGET &target, typename TARGET::param_type value, const SOURCE &source )
{
    mul_(target,value,source, mulop::__add<typename TARGET::mutable_type,typename SOURCE::mutable_type>);
}

//! sequential target += value * source
template <typename TARGET, typename SOURCE> static inline
void mulset( TARGET &target, typename TARGET::param_type value, const SOURCE &source )
{
    mul_(target,value,source, mulop::__set<typename TARGET::mutable_type,typename SOURCE::mutable_type>);
}

//! sequential target += value * source
template <typename TARGET, typename SOURCE> static inline
void mulsub( TARGET &target, typename TARGET::param_type value, const SOURCE &source )
{
    mul_(target,value,source, mulop::__sub<typename TARGET::mutable_type,typename SOURCE::mutable_type>);
}

#define Y_MK_ATOM_MUL_OP(i) op( target[i], value, source[i] )

//! parallel mulop
template <typename TARGET,typename SOURCE,typename MUL_OP> static inline
void mul_(TARGET                     &target,
          typename TARGET::param_type value,
          const SOURCE               &source,
          concurrent::for_each       &loop,
          MUL_OP                     &op)
{
    Y_MK_ATOM_OPS_API()
    TARGET                      *target_;
    const SOURCE                *source_;
    typename TARGET::const_type *value_;
    MUL_OP                      *op_;
    Y_MK_ATOM_OPS_GET(self);
    TARGET                      &target = *self.target_;
    const SOURCE                &source = *self.source_;
    typename TARGET::const_type &value  = *self.value_;
    MUL_OP                      &op     = *self.op_;
    Y_MK_ATOM_OPS_USE(target.size(),MUL_OP)
    &target,&source,&value,&op
    Y_MK_ATOM_OPS_RUN(loop);
}
#undef Y_MK_ATOM_MUL_OP


//! parallel target += value * source
template <typename TARGET,typename SOURCE> static inline
void muladd( TARGET &target, typename TARGET::param_type value, const SOURCE &source,  concurrent::for_each &loop)
{
    mul_(target,value,source,loop,mulop::__add<typename TARGET::mutable_type,typename SOURCE::mutable_type>);
}

//! parallel target = value * source
template <typename TARGET,typename SOURCE> static inline
void mulset( TARGET &target, typename TARGET::param_type value, const SOURCE &source,  concurrent::for_each &loop)
{
    mul_(target,value,source,loop,mulop::__set<typename TARGET::mutable_type,typename SOURCE::mutable_type>);
}

//! parallel target = value * source
template <typename TARGET,typename SOURCE> static inline
void mulsub( TARGET &target, typename TARGET::param_type value, const SOURCE &source,  concurrent::for_each &loop)
{
    mul_(target,value,source,loop,mulop::__sub<typename TARGET::mutable_type,typename SOURCE::mutable_type>);
}




