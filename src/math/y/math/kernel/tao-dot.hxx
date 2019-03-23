//! context for parallel dot product
template <typename T,typename U,typename V>
struct __dot
{
    const array<U> *a; //!< source vector
    const array<V> *b; //!< source vector

    //! store partial dot product
    static inline void call(void *args, parallel &ctx, lockable &)
    {
        __dot<T,U,V> *self = (__dot<T,U,V>*)args;
        const array<U> &a = *(self->a);
        const array<V> &b = *(self->b);
        T sum = 0;
        size_t offset = 1;
        size_t length = a.size();
        ctx.split(length,offset);
        while(length--)
        {
            sum += static_cast<T>(a[offset]) * static_cast<T>(b[offset]);
            ++offset;
        }
        assert(is_zset(ctx.get<T>()));
        ctx.get<T>() = sum;
    }

};


//! dot product, sequential
template <typename T,typename U,typename V> static inline
T _dot( const array<U> &a, const array<V> &b )
{
    T sum = 0;
    for(size_t i=a.size();i>0;--i)
    {
        sum += static_cast<T>(a[i]) * static_cast<T>(b[i]);
    }
    return sum;
}

//! dot product, parallel
template <typename T,typename U,typename V> static inline
T _dot( const array<U> &a, const array<V> &b, concurrent::for_each &loop )
{
    __dot<T,U,V>          args   = { &a, &b };
    concurrent::executor &engine = loop.engine();
    engine.acquire_all( sizeof(T) );
    loop.run( __dot<T,U,V>::call, &args);
    return engine.sum<T>();
}

