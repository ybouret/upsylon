//! \file

//! mmul with cast
#define Y_QUARK_mmul(R) lhs[R] = dot< typename LHS::type >::of( M[R], rhs );
Y_QUARK_V2(mmul,assert(lhs.size()>=M.rows);assert(rhs.size()>=M.cols))
#undef Y_QUARK_mmul

//! mmul_add with casts
#define Y_QUARK_mmul_add(R) lhs[R] += dot< typename LHS::type >::of( M[R], rhs );
Y_QUARK_V2(mmul_add,assert(lhs.size()>=M.rows);assert(rhs.size()>=M.cols))
#undef Y_QUARK_mmul_add


//! mmul_sub with casts
#define Y_QUARK_mmul_sub(R) lhs[R] -= dot< typename LHS::type >::of( M[R], rhs );
Y_QUARK_V2(mmul_sub,assert(lhs.size()>=M.rows);assert(rhs.size()>=M.cols))
#undef Y_QUARK_mmul_sub
