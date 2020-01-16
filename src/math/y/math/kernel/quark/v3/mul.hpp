
template <typename T,typename U,typename V> static inline
void mmul( matrix<T> &A, const matrix<U> &B, const matrix<V> &C)
{
    assert(A.rows==B.rows);
    assert(A.cols==C.cols);
    for(size_t r=A.rows;r>0;--r)
    {
        
    }
}
