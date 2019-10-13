//! \file

//! SIMD kernel
#define Y_MK_ATOM_SET(offset) \
target[offset] = static_cast<typename TARGET::const_type>(source[offset])

//! sequential target=source
template <typename TARGET, typename SOURCE> static inline
void set( TARGET &target, const SOURCE &source )
{
    assert( target.size() <= source.size() );
    for(size_t i=target.size();i>0;--i)
    {
        Y_MK_ATOM_SET(i);
    }
}



//! parallel target=source
template <typename TARGET, typename SOURCE> static inline
void set( TARGET &target, const SOURCE &source,  concurrent::for_each &loop )
{
    assert( target.size() <= source.size() );
    
    Y_MK_ATOM_OPS_API()
    TARGET       *target_;
    const SOURCE *source_;
    Y_MK_ATOM_OPS_GET(self);
    TARGET       &target = *self.target_;
    const SOURCE &source = *self.source_;
    Y_MK_ATOM_OPS_USE(target.size(),SET)
    &target, &source
    Y_MK_ATOM_OPS_RUN(loop);
   
}

#undef Y_MK_ATOM_SET

