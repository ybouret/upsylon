//! \file

//! sequential target=-source
template <typename TARGET, typename SOURCE> static inline
void neg( TARGET &target, const SOURCE &source )
{
    assert( target.size() <= source.size() );
    for(size_t i=target.size();i>0;--i) target[i] = -static_cast<typename TARGET::const_type>(source[i]);
}

//! sequential target=-target
template <typename TARGET> static inline
void neg( TARGET &target )
{
    neg(target,target);
}

//! SIMD kernel
#define Y_MK_ATOM_NEG(I) target[I] = -static_cast<typename TARGET::const_type>(source[I])

//! parallel target=-source
template <typename TARGET, typename SOURCE> static inline
void neg( TARGET &target, const SOURCE &source,  concurrent::for_each &loop )
{
    assert( target.size() <= source.size() );
    
    Y_MK_ATOM_OPS_API()
    TARGET       *target_;
    const SOURCE *source_;
    Y_MK_ATOM_OPS_GET(self);
    TARGET       &target = *self.target_;
    const SOURCE &source = *self.source_;
    Y_MK_ATOM_OPS_USE(target.size(),NEG)
    &target, &source
    Y_MK_ATOM_OPS_RUN(loop);
}

#undef Y_MK_ATOM_NEG

//! parallel  target=-target
template <typename TARGET> static inline
void neg( TARGET &target, concurrent::for_each &loop)
{
    neg(target,target,loop);
}
