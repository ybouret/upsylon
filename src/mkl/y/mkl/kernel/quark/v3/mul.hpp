//! \file

//______________________________________________________________________________
//
//! check arguments for matrix multiplication
//______________________________________________________________________________
#define Y_QUARK_MMUL_CHK() \
assert(A.rows==B.rows);  assert(A.cols==C.cols); assert(B.cols==C.rows)

//______________________________________________________________________________
//
//! default code to used local sum
/**
 - use local sum of the terms
 - TARGET [=,+=,-=] sum
 */
//______________________________________________________________________________
#define Y_QUARK_MMUL_SUM(TARGET,OP)                           \
typename matrix<T>::mutable_type sum = 0;                     \
for(size_t k=n;k>0;--k)                                       \
{                                                             \
sum += auto_cast<T,U>::_(Br[k]) * auto_cast<T,V>::_(C[k][c]); \
}                                                             \
TARGET OP sum

//______________________________________________________________________________
//
//! sequential MMUL code
//______________________________________________________________________________
#define Y_QUARK_MMUL_SEQ(METHOD,OP)                                 \
template <typename T,typename U,typename V> static inline           \
void METHOD(matrix<T> &A, const matrix<U> &B, const matrix<V> &C) { \
Y_QUARK_MMUL_CHK(); const size_t n = B.cols;                        \
for(size_t r=A.rows;r>0;--r) {                                      \
addressable<T>      &Ar = A[r];                                     \
const accessible<U> &Br = B[r];                                     \
for(size_t c=A.cols;c>0;--c) { Y_QUARK_MMUL_SUM(Ar[c],OP);}         \
} }

//______________________________________________________________________________
//
//! parallel MMUL code
//______________________________________________________________________________
#define Y_QUARK_MMUL_PAR(METHOD,OP)                                 \
template <typename T,typename U,typename V> static inline           \
void METHOD(matrix<T> &A, const matrix<U> &B, const matrix<V> &C,   \
concurrent::for_each &loop) { Y_QUARK_MMUL_CHK();                   \
Y_QUARK_MMUL_PAR_ENTER();                                           \
const size_t     n      = B.cols;                                   \
while(length-- > 0 ) {                                              \
Y_QUARK_MMUL_RC(offset++);                                          \
const accessible<U> &Br = B[r];                                     \
Y_QUARK_MMUL_SUM(A[r][c],OP); }                                     \
Y_QUARK_MMUL_PAR_LEAVE();                                           \
}

#define Y_QUARK_MMUL(METHOD,OP) Y_QUARK_MMUL_SEQ(METHOD,OP) Y_QUARK_MMUL_PAR(METHOD,OP)

Y_QUARK_MMUL(mmul,=)
Y_QUARK_MMUL(mmul_add,+=)
Y_QUARK_MMUL(mmul_sub,-=)

#undef Y_QUARK_MMUL
#undef Y_QUARK_MMUL_PAR
#undef Y_QUARK_MMUL_SEQ
#undef Y_QUARK_MMUL_SUM
#undef Y_QUARK_MMUL_CHK
