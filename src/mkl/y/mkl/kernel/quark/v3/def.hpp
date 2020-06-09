
//! \file

//! parallel code helper for matrices
#define Y_QUARK_MMUL_PAR_ENTER()   \
Y_QUARK_TASK_DECL()                \
matrix<T>       *A;                \
const matrix<U> *B;                \
const matrix<V> *C;                \
Y_QUARK_TASK_IMPL()                \
matrix<T>       &A      = *task.A; \
const matrix<U> &B      = *task.B; \
const matrix<V> &C      = *task.C; \
size_t           length = A.items; \
size_t           offset = 0;       \
ctx.split(length,offset)

//! parallel code helper for matrices
#define Y_QUARK_MMUL_PAR_LEAVE() \
Y_QUARK_TASK_DATA()              \
&A, &B, &C                       \
Y_QUARK_TASK_EXEC(loop)

//! parallel code helper for matrices : OFFSET to row/col
#define Y_QUARK_MMUL_RC(OFFSET) \
size_t r=0;\
size_t c=0;\
A.get_item(OFFSET, r, c)
