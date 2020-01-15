//! \file

#define Y_QUARK_MUL_CHECK assert(lhs.size()>=M.rows);assert(rhs.size()>=M.cols)

//! mmul with cast
#define Y_QUARK_mul(R) lhs[R] = dot< typename LHS::type >::of( M[R], rhs );
Y_QUARK_V2(mul,Y_QUARK_MUL_CHECK)
#undef Y_QUARK_mul

//! mmul_add with casts
#define Y_QUARK_mul_add(R) lhs[R] += dot< typename LHS::type >::of( M[R], rhs );
Y_QUARK_V2(mul_add,Y_QUARK_MUL_CHECK)
#undef Y_QUARK_mul_add


//! mmul_sub with casts
#define Y_QUARK_mul_sub(R) lhs[R] -= dot< typename LHS::type >::of( M[R], rhs );
Y_QUARK_V2(mul_sub,Y_QUARK_MUL_CHECK)
#undef Y_QUARK_mul_sub
