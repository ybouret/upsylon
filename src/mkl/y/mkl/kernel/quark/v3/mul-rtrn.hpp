
//! \file

//______________________________________________________________________________
//
//! check arguments for matrix multiplication with right transposed
//______________________________________________________________________________
#define Y_QUARK_MMUL_RTRN_CHK() \
assert(A.rows==B.rows);         \
assert(A.cols==C.rows);         \
assert(B.cols==C.cols)

//______________________________________________________________________________
//
//! code to use local sum
//______________________________________________________________________________
#define Y_QUARK_MMUL_RTRN_SUM(TARGET,OP)                      \
typename matrix<T>::mutable_type sum = 0;                     \
for(size_t k=n;k>0;--k)                                       \
{                                                             \
sum += auto_cast<T,U>::_(Br[k]) * auto_cast<T,V>::_(Cc[k]);   \
}                                                             \
TARGET OP sum

//______________________________________________________________________________
//
//! sequential MMUL_RTRN
//______________________________________________________________________________
#define Y_QUARK_MMUL_RTRN_SEQ(METHOD,OP)                          \
template <typename T,typename U,typename V> static inline         \
void METHOD(matrix<T> &A, const matrix<U> &B, const matrix<V> &C) \
{\
Y_QUARK_MMUL_RTRN_CHK();\
const size_t n = B.cols;\
for(size_t r=A.rows;r>0;--r)\
{\
addressable<T>      &Ar = A[r];\
const accessible<U> &Br = B[r];\
for(size_t c=A.cols;c>0;--c)\
{\
const accessible<V> &Cc = C[c];\
Y_QUARK_MMUL_RTRN_SUM(Ar[c],OP);\
}\
}\
}

//______________________________________________________________________________
//
//! parallel MMUL_RTRN
//______________________________________________________________________________
#define Y_QUARK_MMUL_RTRN_PAR(METHOD,OP)                  \
template <typename T,typename U,typename V> static inline \
void METHOD(matrix<T> &A, const matrix<U> &B, const matrix<V> &C, concurrent::for_each &loop)\
{\
Y_QUARK_MMUL_RTRN_CHK();           \
Y_QUARK_MMUL_PAR_ENTER();         \
const size_t     n      = B.cols; \
while(length-- > 0 )              \
{\
Y_QUARK_MMUL_RC(offset++);\
const accessible<U> &Br = B[r];\
const accessible<V> &Cc = C[c];\
Y_QUARK_MMUL_RTRN_SUM(A[r][c],OP);\
}\
Y_QUARK_MMUL_PAR_LEAVE();\
}

//______________________________________________________________________________
//
//! MMUL_RTRN
//______________________________________________________________________________
#define Y_QUARK_MMUL_RTRN(METHOD,OP) Y_QUARK_MMUL_RTRN_SEQ(METHOD,OP) Y_QUARK_MMUL_RTRN_PAR(METHOD,OP)

Y_QUARK_MMUL_RTRN(mmul_rtrn,=)
Y_QUARK_MMUL_RTRN(mmul_add_rtrn,+=)
Y_QUARK_MMUL_RTRN(mmul_sub_rtrn,-=)

#undef Y_QUARK_MMUL_RTRN
#undef Y_QUARK_MMUL_RTRN_PAR
#undef Y_QUARK_MMUL_RTRN_SEQ
#undef Y_QUARK_MMUL_SUM
#undef Y_QUARK_MMUL_CHK
