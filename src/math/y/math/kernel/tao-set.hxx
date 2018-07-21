template <typename T,typename U> static inline
void _set( array<T> &a, const array<U> &b)
{
    assert(b.size()>=a.size());
    for(size_t i=a.size();i>0;--i) a[i] = static_cast<T>(b[i]);
}

//! internal concurrent wrapper
template <typename T, typename U> struct __set
{
    array<T>  *a; //!< pointer to array
    const array<U>  *b; //!< pointer to value
    //! concurrent kernel
    static inline void call_set(void *args, parallel &ctx, lockable &)
    {
        __set          *self = (__set *)args;
        array<T>       &a    = *(self->a);
        const array<U> &b    = *(self->b);
        size_t offset = 1;
        size_t length = a.size();
        ctx.split(length,offset);
        while(length--)
        {
            a[offset] = static_cast<T>(b[offset]);
            ++offset;
        }
    }

    static inline void call_add(void *args, parallel &ctx, lockable &)
    {
        __set          *self = (__set *)args;
        array<T>       &a    = *(self->a);
        const array<U> &b    = *(self->b);
        size_t offset = 1;
        size_t length = a.size();
        ctx.split(length,offset);
        while(length--)
        {
            a[offset] += static_cast<T>(b[offset]);
            ++offset;
        }
    }

    static inline void call_sub(void *args, parallel &ctx, lockable &)
    {
        __set          *self = (__set *)args;
        array<T>       &a    = *(self->a);
        const array<U> &b    = *(self->b);
        size_t offset = 1;
        size_t length = a.size();
        ctx.split(length,offset);
        while(length--)
        {
            a[offset] -= static_cast<T>(b[offset]);
            ++offset;
        }
    }
};

template <typename T,typename U> static inline
void _set( array<T> &a, const array<U> &b, concurrent::for_each &loop)
{
    __set<T,U> args= { &a, &b };
    loop.run( __set<T,U>::call_set, &args );
}
