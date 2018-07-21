template <typename T, typename U, typename V> static inline
void _mmul( array<T> &a, const matrix<U> &M, const array<V> &b )
{
    assert( a.size() <= M.rows   );
    assert( M.cols   == b.size() );
    for(size_t r=M.rows;r>0;--r)
    {
        a[r] = _dot<T,U,V>(M[r],b);
    }
}

template <typename T, typename U, typename V>
struct __mmul
{
    array<T>        *a;
    const matrix<U> *M;
    const array<V>  *b;

    static inline void call( void *args, parallel &ctx, lockable & )
    {
        __mmul<T,U,V> *self = (__mmul<T,U,V> *)args;
        array<T>        &a = *(self->a);
        const matrix<U> &M = *(self->M);
        const array<V>  &b = *(self->b);
        size_t offset = 1;
        size_t length  = M.rows;
        ctx.split(length,offset);
        while(length-->0)
        {
            a[offset] = _dot<T,U,V>(M[offset],b);
            ++offset;
        }
    }
};

template <typename T, typename U, typename V> static inline
void _mmul( array<T> &a, const matrix<U> &M, const array<V> &b, concurrent::for_each &loop )
{
    assert( a.size() <= M.rows   );
    assert( M.cols   == b.size() );

    __mmul<T,U,V> args = { &a, &M, &b };
    loop.run(__mmul<T,U,V>::call, &args);
}
