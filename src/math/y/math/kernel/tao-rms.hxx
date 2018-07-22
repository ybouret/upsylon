template <typename T> static inline
typename real_for<T>::type _rms( const array<T> &a )
{
    const size_t               n   = a.size();
    typename real_for<T>::type sum = 0;
    if(n>0)
    {
        for(size_t i=n;i>0;--i)
        {
            sum += __mod2(a[i]);
        }
        return __sqrt(sum/n);
    }
    else
    {
        return sum;
    }
}

template <typename T> static inline
typename real_for<T>::type _rms( const array<T> &a, const array<T> &b )
{
    assert(a.size()==b.size());
    const size_t               n   = a.size();
    typename real_for<T>::type sum = 0;
    if(n>0)
    {
        for(size_t i=n;i>0;--i)
        {
            sum += __mod2(a[i]-b[i]);
        }
        return __sqrt(sum/n);
    }
    else
    {
        return sum;
    }
}

template <typename T>
struct __rms
{
    const array<T> *a; //!< single of first vector
    const array<T> *b; //!< NULL of second vector

    static inline
    void call1( void *args, parallel &ctx, lockable & )
    {
        typename real_for<T>::type sum  = 0;
        __rms<T>                  *self = (__rms<T> *)args;
        const array<T>            &a    = *(self->a);
        size_t offset = 1;
        size_t length = a.size();
        ctx.split(length,offset);
        while(length-->0)
        {
            sum += __mod2( a[offset] );
            ++offset;
        }
        assert( is_zset(ctx.get<typename real_for<T>::type>()) );
        ctx.get<typename real_for<T>::type>() = sum;
    }

    static inline
    void call2( void *args, parallel &ctx, lockable & )
    {
        typename real_for<T>::type sum  = 0;
        __rms<T>                  *self = (__rms<T> *)args;
        const array<T>            &a    = *(self->a);
        const array<T>            &b    = *(self->b);
        size_t offset = 1;
        size_t length = a.size();
        ctx.split(length,offset);
        while(length-->0)
        {
            sum += __mod2( a[offset]-b[offset] );
            ++offset;
        }
        assert( is_zset(ctx.get<typename real_for<T>::type>()) );
        ctx.get<typename real_for<T>::type>() = sum;
    }

};

template <typename T> static inline
typename real_for<T>::type _rms( const array<T> &a, concurrent::for_each &loop )
{
    typename real_for<T>::type sum = 0;
    const size_t               n   = a.size();
    if(n>0)
    {
        __rms<T> args = { &a, 0 };
        concurrent::executor &engine = loop.engine();
        engine.make_for<typename real_for<T>::type>();
        loop.run( __rms<T>::call1, &args );
        size_t i = engine.num_threads();
        while(i-->0)
        {
            sum += engine[i].get<typename real_for<T>::type>();
        }
        return __sqrt(sum/n);
    }
    else
    {
        return sum;
    }
}

template <typename T> static inline
typename real_for<T>::type _rms( const array<T> &a, const array<T> &b, concurrent::for_each &loop )
{
    typename real_for<T>::type sum = 0;
    const size_t               n   = a.size();
    if(n>0)
    {
        __rms<T> args = { &a, &b };
        concurrent::executor &engine = loop.engine();
        engine.make_for<typename real_for<T>::type>();
        loop.run( __rms<T>::call2, &args );
        size_t i = engine.num_threads();
        while(i-->0)
        {
            sum += engine[i].get<typename real_for<T>::type>();
        }
        return __sqrt(sum/n);
    }
    else
    {
        return sum;
    }
}

