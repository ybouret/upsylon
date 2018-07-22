//! multiply, sequential
template <typename T, typename U, typename V> static inline
void _mul( array<T> &a, const matrix<U> &M, const array<V> &b )
{
    assert( a.size() <= M.rows   );
    assert( M.cols   == b.size() );
    for(size_t r=M.rows;r>0;--r)
    {
        a[r] = _dot<T,U,V>(M[r],b);
    }
}

//! concurrent wrapper
template <typename T, typename U, typename V>
struct __mul
{
    array<T>        *a; //!< target vector
    const matrix<U> *M; //!< source matrix
    const array<V>  *b; //!< source vector

    //! mulitply
    static inline void call( void *args, parallel &ctx, lockable & )
    {
        __mul<T,U,V>   *self = (__mul<T,U,V> *)args;
        array<T>        &a   = *(self->a);
        const matrix<U> &M   = *(self->M);
        const array<V>  &b   = *(self->b);
        size_t offset = 1;
        size_t length  = M.rows;
        ctx.split(length,offset);
        while(length-->0)
        {
            a[offset] = _dot<T,U,V>(M[offset],b);
            ++offset;
        }
    }

    //! multiply and add
    static inline void call_add( void *args, parallel &ctx, lockable & )
    {
        __mul<T,U,V>   *self = (__mul<T,U,V> *)args;
        array<T>        &a   = *(self->a);
        const matrix<U> &M   = *(self->M);
        const array<V>  &b   = *(self->b);
        size_t offset = 1;
        size_t length  = M.rows;
        ctx.split(length,offset);
        while(length-->0)
        {
            a[offset] += _dot<T,U,V>(M[offset],b);
            ++offset;
        }
    }

    //! multiply and add
    static inline void call_sub( void *args, parallel &ctx, lockable & )
    {
        __mul<T,U,V>   *self = (__mul<T,U,V> *)args;
        array<T>        &a   = *(self->a);
        const matrix<U> &M   = *(self->M);
        const array<V>  &b   = *(self->b);
        size_t offset = 1;
        size_t length  = M.rows;
        ctx.split(length,offset);
        while(length-->0)
        {
            a[offset] -= _dot<T,U,V>(M[offset],b);
            ++offset;
        }
    }


};

//! concurrent multiplication
template <typename T, typename U, typename V> static inline
void _mul( array<T> &a, const matrix<U> &M, const array<V> &b, concurrent::for_each &loop )
{
    assert( a.size() <= M.rows   );
    assert( M.cols   == b.size() );

    const size_t nr = M.rows;
    if(nr>=M.cols)
    {
        __mul<T,U,V> args = { &a, &M, &b };
        loop.run(__mul<T,U,V>::call, &args);
    }
    else
    {
        for(size_t r=nr;r>0;--r)
        {
            a[r] = _dot<T,U,V>(M[r],b,loop);
        }
    }
}

//! multiply and add
template <typename T, typename U, typename V> static inline
void _mul_add( array<T> &a, const matrix<U> &M, const array<V> &b )
{
    assert( a.size() <= M.rows   );
    assert( M.cols   == b.size() );
    for(size_t r=M.rows;r>0;--r)
    {
        a[r] += _dot<T,U,V>(M[r],b);
    }
}

//! multiply and add, concurrent
template <typename T, typename U, typename V> static inline
void _mul_add( array<T> &a, const matrix<U> &M, const array<V> &b, concurrent::for_each &loop )
{
    assert( a.size() <= M.rows   );
    assert( M.cols   == b.size() );

    const size_t nr = M.rows;
    if(nr>=M.cols)
    {
        __mul<T,U,V> args = { &a, &M, &b };
        loop.run(__mul<T,U,V>::call_add, &args);
    }
    else
    {
        for(size_t r=nr;r>0;--r)
        {
            a[r] += _dot<T,U,V>(M[r],b,loop);
        }
    }
}


//! multiply and sub
template <typename T, typename U, typename V> static inline
void _mul_sub( array<T> &a, const matrix<U> &M, const array<V> &b )
{
    assert( a.size() <= M.rows   );
    assert( M.cols   == b.size() );
    for(size_t r=M.rows;r>0;--r)
    {
        a[r] -= _dot<T,U,V>(M[r],b);
    }
}

//! multiply and sub, concurrent
template <typename T, typename U, typename V> static inline
void _mul_sub( array<T> &a, const matrix<U> &M, const array<V> &b, concurrent::for_each &loop )
{
    assert( a.size() <= M.rows   );
    assert( M.cols   == b.size() );

    const size_t nr = M.rows;
    if(nr>=M.cols)
    {
        __mul<T,U,V> args = { &a, &M, &b };
        loop.run(__mul<T,U,V>::call_sub, &args);
    }
    else
    {
        for(size_t r=nr;r>0;--r)
        {
            a[r] -= _dot<T,U,V>(M[r],b,loop);
        }
    }
}


