

template <typename TARGET, typename SOURCE> static inline
void neg( TARGET &target, const SOURCE &source )
{
    assert( target.size() <= source.size() );
    for(size_t i=target.size();i>0;--i) target[i] = -static_cast<typename TARGET::const_type>(source[i]);
}

template <typename TARGET> static inline
void neg( TARGET &target )
{
    neg(target,target);
}


template <typename TARGET, typename SOURCE> static inline
void neg( TARGET &target, const SOURCE &source,  concurrent::for_each &loop )
{
    assert( target.size() <= source.size() );
    
    struct ops
    {
        TARGET       *target_;
        const SOURCE *source_;
        static inline void call(void *args, parallel &ctx, lockable &)
        {
            ops          &params = *static_cast<ops*>(args);
            TARGET       &target = *params.target_;
            const SOURCE &source = *params.source_;
            size_t offset = 1;
            size_t length = target.size();
            ctx.split(length,offset);
            while(length--)
            {
                target[offset] = -static_cast<typename TARGET::const_type>(source[offset]);
                ++offset;
            }
        }
    };
    ops params = { &target, &source };
    loop.run( ops::call, &params );
}

template <typename TARGET> static inline
void neg( TARGET &target, concurrent::for_each &loop)
{
    neg(target,target,loop);
}
