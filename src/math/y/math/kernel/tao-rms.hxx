//! RMS, sequential
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

//! MOD2, sequential
template <typename T> static inline
typename real_for<T>::type _mod2( const array<T> &a )
{
    const size_t n   = a.size();
    if(n>0)
    {
        typename real_for<T>::type sum = 0;
        for(size_t i=n;i>0;--i)
        {
            sum += __mod2(a[i]);
        }
        return sum;
    }
    else
    {
        return 0;
    }
}

//! RMS, sequential
template <typename T> static inline
typename real_for<T>::type _rms( const array<T> &a, const array<T> &b )
{
    assert(a.size()==b.size());
    const size_t               n   = a.size();
    if(n>0)
    {
        typename real_for<T>::type sum = 0;
        for(size_t i=n;i>0;--i)
        {
            sum += __mod2(a[i]-b[i]);
        }
        return __sqrt(sum/n);
    }
    else
    {
        return 0;
    }
}

//! MOD2, sequential
template <typename T> static inline
typename real_for<T>::type _mod2( const array<T> &a, const array<T> &b )
{
    assert(a.size()==b.size());
    const size_t n   = a.size();
    if(n>0)
    {
        typename real_for<T>::type sum = 0;
        for(size_t i=n;i>0;--i)
        {
            sum += __mod2(a[i]-b[i]);
        }
        return sum;
    }
    else
    {
        return 0;
    }
}

//! context to perform concurrent RMS/MOD2
template <typename T>
struct __rms
{
    const array<T> *a; //!< single of first vector
    const array<T> *b; //!< NULL of second vector

    //! store sum of local mod2
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

    //! store sum of local mod2
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

//! RMS, parallel
template <typename T> static inline
typename real_for<T>::type _rms( const array<T> &a, concurrent::for_each &loop )
{
    const size_t n   = a.size();
    if(n>0)
    {
        __rms<T> args = { &a, 0 };
        concurrent::executor &engine = loop.engine();
        engine.make_for<typename real_for<T>::type>();
        loop.run( __rms<T>::call1, &args );
        return __sqrt( engine.sum<typename real_for<T>::type>()/n );
    }
    else
    {
        return 0;
    }
}

//! MOD2, parallel
template <typename T> static inline
typename real_for<T>::type _mod2( const array<T> &a, concurrent::for_each &loop )
{
    const size_t n   = a.size();
    if(n>0)
    {
        __rms<T> args = { &a, 0 };
        concurrent::executor &engine = loop.engine();
        engine.make_for<typename real_for<T>::type>();
        loop.run( __rms<T>::call1, &args );
        return engine.sum<typename real_for<T>::type>();
    }
    else
    {
        return 0;
    }
}

//! RMS, parallel
template <typename T> static inline
typename real_for<T>::type _rms( const array<T> &a, const array<T> &b, concurrent::for_each &loop )
{
    const size_t n   = a.size();
    if(n>0)
    {
        __rms<T> args = { &a, &b };
        concurrent::executor &engine = loop.engine();
        engine.make_for<typename real_for<T>::type>();
        loop.run( __rms<T>::call2, &args );
        return __sqrt(engine.sum<typename real_for<T>::type>()/n);
    }
    else
    {
        return 0;
    }
}

//! MOD2, parallel
template <typename T> static inline
typename real_for<T>::type _mod2( const array<T> &a, const array<T> &b, concurrent::for_each &loop )
{
    const size_t n   = a.size();
    if(n>0)
    {
        __rms<T> args = { &a, &b };
        concurrent::executor &engine = loop.engine();
        engine.make_for<typename real_for<T>::type>();
        loop.run( __rms<T>::call2, &args );
        return engine.sum<typename real_for<T>::type>();
    }
    else
    {
        return 0;
    }
}


