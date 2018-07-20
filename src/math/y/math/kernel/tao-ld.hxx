
//! a[1..size()] = v
template <typename T>
static void _ld( array<T> &a, typename type_traits<T>::parameter_type v )
{
    for(size_t i=a.size();i>0;--i)
    {
        a[i] = v;
    }
}

//! internal concurrent wrapper
template <typename T> struct __ld
{
    Y_DECL_ARGS(T,type); //!< alias
    array<type> *a; //!< pointer to array
    const_type  *v; //!< pointer to value
    //! concurrent kernel
    static inline void call(void *args, parallel &ctx, lockable &)
    {
        __ld        *self = (__ld *)args;
        array<type> &a    = *(self->a);
        const_type  &v    = *(self->v);
        size_t offset = 1;
        size_t length = a.size();
        ctx.split(length,offset);
        while(length--)
        {
            a[offset++] = v;
        }
    }
};

//! a[1..size()] = v, concurrent
template <typename T>
static void _ld( array<T> &a, typename type_traits<T>::parameter_type v, concurrent::for_each &loop )
{
    __ld<T> args = { &a, &v };
    loop.run( __ld<T>::call, &args );
}
