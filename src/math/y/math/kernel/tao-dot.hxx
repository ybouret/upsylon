template <typename T,typename U,typename V>
struct __dot
{
    const array<U> *a;
    const array<V> *b;
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
        }
    }

};

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

template <typename T,typename U,typename V> static inline
T _dot( const array<U> &a, const array<V> &b, concurrent::for_each &loop )
{
    __dot<T,U,V> args = { &a, &b };
    T sum = 0;
    loop.run( __dot<T,U,V>::call, &args);
    return sum;
}

