//! \file

//! SIMD kernel
#define Y_MK_ATOM_NEG(I) target[I] = -Y_MK_ATOM_CAST(TARGET,SOURCE,source[I])

//! sequential target=-source
template <typename TARGET, typename SOURCE> static inline
void neg( TARGET &target, const SOURCE &source )
{
    assert( target.size() <= source.size() );
    for(size_t i=target.size();i>0;--i)
    {
        Y_MK_ATOM_NEG(i);
    }
}

//! sequential target=-target
template <typename TARGET> static inline
void neg( TARGET &target )
{
    neg(target,target);
}

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
