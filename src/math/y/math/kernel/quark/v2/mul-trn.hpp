//! \file


#define Y_QUARK_MUL_TRN_CHECK assert(lhs.size()>=M.cols); assert(rhs.size()>=M.rows);

#define Y_QUARK_MUL_TRN_SUM(COL)    \
typename LHS::mutable_type sum = 0; \
for(size_t r=M.rows;r>0;--r) {      \
    sum += Y_QUARK_CAST(LHS,matrix<T>,M[r][COL]) * Y_QUARK_CAST(LHS,RHS,rhs[r]);\
}

#define Y_QUARK_mul_trn(COL) Y_QUARK_MUL_TRN_SUM(COL); lhs[COL] = sum
Y_QUARK_V2_TRN(mul_trn,Y_QUARK_MUL_TRN_CHECK)
#undef Y_QUARK_mul_trn

#define Y_QUARK_mul_add_trn(COL) Y_QUARK_MUL_TRN_SUM(COL); lhs[COL] += sum
Y_QUARK_V2_TRN(mul_add_trn,Y_QUARK_MUL_TRN_CHECK)
#undef Y_QUARK_mul_add_trn

#define Y_QUARK_mul_sub_trn(COL) Y_QUARK_MUL_TRN_SUM(COL); lhs[COL] -= sum
Y_QUARK_V2_TRN(mul_sub_trn,Y_QUARK_MUL_TRN_CHECK)
#undef Y_QUARK_mul_sub_trn


#define Y_QUARK_mul_subp_trn(COL) Y_QUARK_MUL_TRN_SUM(COL); lhs[COL] = sum - lhs[COL]
Y_QUARK_V2_TRN(mul_subp_trn,Y_QUARK_MUL_TRN_CHECK)
#undef Y_QUARK_mul_subp_trn
