
template <typename T,typename U,typename V> static inline
void _mmul( matrix<T> &M, const matrix<U> &A, const matrix<V> &B)
{
    assert(M.rows==A.rows);
    assert(A.cols==B.rows);
    assert(M.cols==B.cols);
    const size_t n = A.cols;
    for(size_t i=M.rows;i>0;--i)
    {
        array<T> &M_i = M[i];
        for(size_t j=M.cols;j>0;--j)
        {
            T sum = 0;
            for(size_t k=n;k>0;--k)
            {
                sum += T(A[i][k]) * T(B[k][j]);
            }
            M_i[j] = sum;
        }
    }
}

template <typename T, typename U, typename V>
struct __mmul
{
    matrix<T>       *pM;
    const matrix<U> *pA;
    const matrix<V> *pB;
    
    static inline void call( void *args, parallel &ctx, lockable & )
    {
        __mmul *         self   = (__mmul *)args;
        matrix<T>       &M      = *(self->pM);
        const matrix<U> &A      = *(self->pA);
        const matrix<V> &B      = *(self->pB);
        const size_t     n      = A.cols; // internal summation
        size_t           offset = 0;
        size_t           length = M.items;
        size_t           r=0,c=0;
        ctx.split(length,offset);
        T               *data = *M + offset;
        while(length-->0)
        {
            // compute r,c from offset
            M.get_item(offset,r,c);
            const array<U> &A_r = A[r];
            T sum = 0;
            for(size_t k=n;k>0;--k)
            {
                sum += T(A_r[k]) * T(B[k][c]);
            }
            *(data++) = sum;
            // next
            ++offset;
        }
    }
    
};


template <typename T,typename U,typename V> static inline
void _mmul( matrix<T> &M, const matrix<U> &A, const matrix<V> &B, concurrent::for_each &loop)
{
    assert(M.rows==A.rows);
    assert(A.cols==B.rows);
    assert(M.cols==B.cols);
    
    __mmul<T,U,V> args = { &M, &A, &B };
    loop.run( __mmul<T,U,V>::call , &args);
    
}
