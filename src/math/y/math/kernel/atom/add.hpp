template <typename TARGET, typename SOURCE> static inline
void add( TARGET &target, const SOURCE &source )
{
    assert( target.size() <= source.size() );
    for(size_t i=target.size();i>0;--i) target[i] += static_cast<typename TARGET::const_type>(source[i]);
}



template <typename TARGET, typename SOURCE> static inline
void add( TARGET &target, const SOURCE &source,  concurrent::for_each &loop )
{
    assert( target.size() <= source.size() );

    for(size_t i=target.size();i>0;--i) target[i] = source[i];

    struct ops {
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
                target[offset] += static_cast<typename TARGET::const_type>(source[offset]);
                ++offset;
            }
        }
    };
    ops params = { &target, &source };
    loop.run( ops::call, &params );
}


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
}

