//! \file

#define Y_QUARK_MUL_CHECK assert(lhs.size()>=M.rows);assert(rhs.size()>=M.cols)

//! mul with cast
#define Y_QUARK_mul(R) lhs[R] = dot< typename LHS::type >::of( M[R], rhs )
Y_QUARK_V2(mul,Y_QUARK_MUL_CHECK)
#undef Y_QUARK_mul

//! mul_add with casts
#define Y_QUARK_mul_add(R) lhs[R] += dot< typename LHS::type >::of( M[R], rhs )
Y_QUARK_V2(mul_add,Y_QUARK_MUL_CHECK)
#undef Y_QUARK_mul_add


//! mul_sub with casts
#define Y_QUARK_mul_sub(R) lhs[R] -= dot< typename LHS::type >::of( M[R], rhs )
Y_QUARK_V2(mul_sub,Y_QUARK_MUL_CHECK)
#undef Y_QUARK_mul_sub

//! mul_subp with casts
#define Y_QUARK_mul_subp(R) lhs[R] = dot< typename LHS::type >::of( M[R], rhs ) - lhs[R]
Y_QUARK_V2(mul_subp,Y_QUARK_MUL_CHECK)
#undef Y_QUARK_mul_subp
