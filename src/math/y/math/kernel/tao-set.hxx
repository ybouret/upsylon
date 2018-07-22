//! set a=b, sequential
template <typename T,typename U> static inline
void _set( array<T> &a, const array<U> &b)
{
    assert(b.size()>=a.size());
    for(size_t i=a.size();i>0;--i) a[i] = static_cast<T>(b[i]);
}

//! internal concurrent wrapper
template <typename T, typename U> struct __set
{
    array<T>        *a; //!< pointer to array
    const array<U>  *b; //!< pointer to value
    const T         *x; //!< some value

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

    //! concurrent kernel to add
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

    //! concurrent kernel to sub
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

    //! concurent kernel to muladd
    static inline void call_muladd(void *args, parallel &ctx, lockable &)
    {
        __set          *self = (__set *)args;
        array<T>       &a    = *(self->a);
        const array<U> &b    = *(self->b);
        const T        &x    = *(self->x);
        size_t offset = 1;
        size_t length = a.size();
        ctx.split(length,offset);
        while(length--)
        {
            a[offset] += x * static_cast<T>(b[offset]);
            ++offset;
        }
    }

    //! concurrent kernel to mulsub
    static inline void call_mulsub(void *args, parallel &ctx, lockable &)
    {
        __set          *self = (__set *)args;
        array<T>       &a    = *(self->a);
        const array<U> &b    = *(self->b);
        const T        &x    = *(self->x);
        size_t offset = 1;
        size_t length = a.size();
        ctx.split(length,offset);
        while(length--)
        {
            a[offset] -= x * static_cast<T>(b[offset]);
            ++offset;
        }
    }

    //! concurrent kernel to mulset
    static inline void call_mulset(void *args, parallel &ctx, lockable &)
    {
        __set          *self = (__set *)args;
        array<T>       &a    = *(self->a);
        const array<U> &b    = *(self->b);
        const T        &x    = *(self->x);
        size_t offset = 1;
        size_t length = a.size();
        ctx.split(length,offset);
        while(length--)
        {
            a[offset] = x * static_cast<T>(b[offset]);
            ++offset;
        }
    }
};

//! set a=b in parallel
template <typename T,typename U> static inline
void _set( array<T> &a, const array<U> &b, concurrent::for_each &loop)
{
    __set<T,U> args= { &a, &b, NULL};
    loop.run( __set<T,U>::call_set, &args );
}
