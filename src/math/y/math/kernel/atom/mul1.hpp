//! \file

//! SIMD code
#define Y_MK_ATOM_MUL_BY(I) arr[I] *= value

//! sequential array *= value
template <typename ARRAY> static inline
void mul_by( typename ARRAY::param_type value, ARRAY &arr )
{
    for(size_t i=arr.size();i>0;--i)
    {
        Y_MK_ATOM_MUL_BY(i);
    }
}


//! parallel array *= value
template <typename ARRAY> static inline
void mul_by( typename ARRAY::param_type value, ARRAY &arr,  concurrent::for_each &loop)
{
    
    Y_MK_ATOM_OPS_API()
    ARRAY                      *array_;
    typename ARRAY::const_type *value_;
    Y_MK_ATOM_OPS_GET(self);
    ARRAY                      &arr   = *self.array_;
    typename ARRAY::const_type &value = *self.value_;
    Y_MK_ATOM_OPS_USE(arr.size(),MUL_BY)
    &arr, &value
    Y_MK_ATOM_OPS_RUN(loop);
}

#undef Y_MK_ATOM_MUL_BY

//! type dependent mulop implementation
struct mulop
{
    //! target = value * source
    template <typename T,typename U> static inline
    void __set( T &target, const T value, const U &source)
    {
        target = value * auto_cast<T,U>::_( source );
    }
    
    //! target += value * source
    template <typename T,typename U> static inline
    void __add( T &target, const T value, const U &source)
    {
        target += value * auto_cast<T,U>::_( source );
    }
    
    //! target -= value * source
    template <typename T,typename U> static inline
    void __sub( T &target, const T value, const U &source)
    {
        target -= value * auto_cast<T,U>::_( source );
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

//! SIMD kernel
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


//! SIMD kernel
#define Y_MK_ATOM_SETPROBE(i) \
target[i] = Y_MK_ATOM_CAST(TARGET,LHS,lhs[i]) + value * Y_MK_ATOM_CAST(TARGET,RHS,rhs[i])

//! sequental target = lhs + value * rhs
template<typename TARGET,typename LHS,typename RHS> static inline
void setprobe(TARGET                     &target,
              const    LHS               &lhs,
              typename TARGET::param_type value,
              const    RHS               &rhs)
{
    assert( target.size() <= lhs.size() );
    assert( target.size() <= rhs.size() );
    for(size_t i=target.size();i>0;--i)
    {
        Y_MK_ATOM_SETPROBE(i);
    }

}


//! parallel target = lhs + value * rhs
template<typename TARGET,typename LHS,typename RHS> static inline
void setprobe(TARGET                     &target,
              const    LHS               &lhs,
              typename TARGET::param_type value,
              const    RHS               &rhs,
              concurrent::for_each       &loop)
{
    assert( target.size() <= lhs.size() );
    assert( target.size() <= rhs.size() );
    
    Y_MK_ATOM_OPS_API()
    TARGET                      *target_;
    const LHS                   *lhs_;
    typename TARGET::const_type *value_;
    const RHS                   *rhs_;
    Y_MK_ATOM_OPS_GET(self);
    TARGET                      &target = *self.target_;
    const LHS                   &lhs    = *self.lhs_;
    const RHS                   &rhs    = *self.rhs_;
    typename TARGET::const_type &value  = *self.value_;
    Y_MK_ATOM_OPS_USE(target.size(),SETPROBE)
    &target,&lhs,&value,&rhs
    Y_MK_ATOM_OPS_RUN(loop);
}

#undef Y_MK_ATOM_SETPROBE
