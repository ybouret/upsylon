template <typename TARGET, typename SOURCE> static inline
void add( TARGET &target, const SOURCE &source )
{
    assert( target.size() <= source.size() );
    for(size_t i=target.size();i>0;--i) target[i] += static_cast<typename TARGET::const_type>(source[i]);
}


#define Y_MK_ATOM_ADD(I) target[I] += static_cast<typename TARGET::const_type>(source[I])

template <typename TARGET, typename SOURCE> static inline
void add( TARGET &target, const SOURCE &source,  concurrent::for_each &loop )
{
    assert( target.size() <= source.size() );

    Y_MK_ATOM_OPS_API()
    TARGET       *target_;
    const SOURCE *source_;
    Y_MK_ATOM_OPS_GET(self);
    TARGET       &target = *self.target_;
    const SOURCE &source = *self.source_;
    Y_MK_ATOM_OPS_USE(target.size(),ADD)
    &target, &source
    Y_MK_ATOM_OPS_RUN(loop);
    
}

#undef Y_MK_ATOM_ADD

#define Y_MK_ATOM_ADD(offset) \
target[offset] = static_cast<typename TARGET::const_type>(lhs[offset]) + static_cast<typename TARGET::const_type>(rhs[offset])


template <typename TARGET, typename LHS, typename RHS> static inline
void add( TARGET &target, const LHS &lhs, const RHS &rhs )
{
    assert( target.size() <= lhs.size() );
    assert( target.size() <= rhs.size() );
    for(size_t i=target.size();i>0;--i) target[i] = static_cast<typename TARGET::const_type>(lhs[i]) + static_cast<typename TARGET::const_type>(rhs[i]);
}


template <typename TARGET, typename LHS, typename RHS> static inline
void add( TARGET &target, const LHS &lhs, const RHS &rhs, concurrent::for_each &loop )
{
    assert( target.size() <= lhs.size() );
    assert( target.size() <= rhs.size() );
    Y_MK_ATOM_OPS_API()
    TARGET       *target_;
    const LHS    *lhs_;
    const RHS    *rhs_;
    Y_MK_ATOM_OPS_GET(self);
    TARGET       &target = *self.target_;
    const LHS    &lhs    = *self.lhs;
    const LHS    &rhs    = *self.rhs;
    Y_MK_ATOM_OPS_USE(target.size(),ADD)
    &target, &lhs, &rhs
    Y_MK_ATOM_OPS_RUN(loop);
#if 0
    struct ops {
        TARGET       *target_;
        const LHS    *lhs_;
        const RHS    *rhs_;

        static inline void call(void *args, parallel &ctx, lockable &)
        {
            ops          &params = *static_cast<ops*>(args);
            TARGET       &target = *params.target_;
            const LHS    &lhs    = *params.lhs_;
            const RHS    &rhs    = *params.rhs_;

            size_t offset = 1;
            size_t length = target.size();
            ctx.split(length,offset);
            while(length--)
            {
                target[offset] = static_cast<typename TARGET::const_type>(lhs[offset]) + static_cast<typename TARGET::const_type>(rhs[offset]);
                ++offset;
            }
        }
    };

    ops params = { &target, &lhs, &rhs };
    loop.run( ops::call, &params );
#endif
}

#undef Y_MK_ATOM_ADD

