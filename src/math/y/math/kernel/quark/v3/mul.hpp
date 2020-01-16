//! \file

#define Y_QUARK_MMUL_CHK() \
assert(A.rows==B.rows);    \
assert(A.cols==C.cols);    \
assert(B.cols==C.rows)


#define Y_QUARK_MMUL_SUM(TARGET,OP)                           \
typename matrix<T>::mutable_type sum = 0;                     \
for(size_t k=n;k>0;--k)                                       \
{                                                             \
sum += auto_cast<T,U>::_(Br[k]) * auto_cast<T,V>::_(C[k][c]); \
}                                                             \
TARGET OP sum

#define Y_QUARK_MMUL(METHOD,OP)                                   \
template <typename T,typename U,typename V> static inline         \
void METHOD(matrix<T> &A, const matrix<U> &B, const matrix<V> &C) \
{\
    Y_QUARK_MMUL_CHK();\
    const size_t n = B.cols;\
    for(size_t r=A.rows;r>0;--r)\
    {\
        addressable<T>      &Ar = A[r];\
        const accessible<U> &Br = B[r];\
        for(size_t c=A.cols;c>0;--c)\
        {\
            Y_QUARK_MMUL_SUM(Ar[c],OP);\
        }\
    }\
}\
\
template <typename T,typename U,typename V> static inline \
void METHOD(matrix<T> &A, const matrix<U> &B, const matrix<V> &C, concurrent::for_each &loop)\
{\
    Y_QUARK_MMUL_CHK();               \
    Y_QUARK_MMUL_PAR_ENTER();         \
    const size_t     n      = B.cols; \
    while(length-- > 0 )              \
    {\
        Y_QUARK_MMUL_RC(offset++);\
        const accessible<U> &Br = B[r];\
        Y_QUARK_MMUL_SUM(A[r][c],OP);\
    }\
    Y_QUARK_MMUL_PAR_LEAVE();\
}

Y_QUARK_MMUL(mmul,=)
Y_QUARK_MMUL(mmul_add,+=)
Y_QUARK_MMUL(mmul_sub,-=)

#undef Y_QUARK_MMUL
#undef Y_QUARK_MMUL_SUM
#undef Y_QUARK_MMUL_CHK
