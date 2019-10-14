//! \file

//! SIMD kernel
#define Y_MK_ATOM_MMUL()                               \
typename MATRIX::mutable_type sum(0);                  \
for(size_t k=np;k>0;--k)                               \
{                                                      \
sum +=                                                 \
Y_MK_ATOM_CAST(MATRIX,LHS,lhs[i][k])                   \
*                                                      \
Y_MK_ATOM_CAST(MATRIX,RHS,rhs[k][j]);                  \
}                                                      \
M[i][j] = sum;

//! sequential M = lhs*rhs
template <typename MATRIX, typename LHS, typename RHS> static inline
void mmul(MATRIX &M, const LHS &lhs, const RHS &rhs )
{
    assert(lhs.rows==M.rows);
    assert(rhs.cols==M.cols);
    assert(lhs.cols==rhs.rows);

    const size_t nr = M.rows;
    const size_t nc = M.cols;
    const size_t np = lhs.cols;

    for(size_t i=nr;i>0;--i)
    {
        for(size_t j=nc;j>0;--j)
        {
            Y_MK_ATOM_MMUL();
        }
    }
}

//! paralellizing macro, level 3
#define Y_MK_ATOM3_PAR(NAME,PROLOG)             \
struct ops                                      \
{                                               \
MATRIX    *M_;                                  \
const LHS *lhs_;                                \
const RHS *rhs_;                                \
static inline void call(void *args,             \
parallel &ctx,                                  \
lockable &)                                     \
{                                               \
ops         &self  = *static_cast<ops *>(args); \
MATRIX      &M     = *self.M_;                  \
const LHS   &lhs   = *self.lhs_;                \
const RHS   &rhs   = *self.rhs_;                \
PROLOG;                                         \
size_t      length = M.items;                   \
size_t      offset = 0;                         \
ctx.split(length,offset);                       \
while(length-- > 0 )                            \
{                                               \
const size_t i=1+(offset/nc);                   \
const size_t j=1+(offset%nc);                   \
Y_MK_ATOM_##NAME();                             \
++offset;                                       \
}                                               \
}                                               \
};                                              \
ops params = { &M, &lhs, &rhs };                \
loop.run( & ops::call, &params )


//! parallel M = lhs * rhs
template <typename MATRIX, typename LHS, typename RHS> static inline
void mmul(MATRIX &M, const LHS &lhs, const RHS &rhs, concurrent::for_each &loop )
{
    assert(lhs.rows==M.rows);
    assert(rhs.cols==M.cols);
    assert(lhs.cols==rhs.rows);

    Y_MK_ATOM3_PAR(MMUL,const size_t nc=M.cols;const size_t np=lhs.cols);

}

#undef Y_MK_ATOM_MMUL

//! SIMD kernel
#define Y_MK_ATOM_MMUL_RTRN() do {\
typename LHS::const_type tmp =dot(lhs[i],rhs[i]); \
M[i][j] = Y_MK_ATOM_CAST(MATRIX,LHS,tmp); } while(false)


//! sequential M = lhs * rhs'
template <typename MATRIX, typename LHS, typename RHS> static inline
void mmul_rtrn(MATRIX &M, const LHS &lhs, const RHS &rhs )
{
    assert(lhs.rows==M.rows);
    assert(rhs.rows==M.rows);
    assert(lhs.cols==rhs.cols);
    const size_t nc = M.cols;
    for( size_t  i  = M.rows;i>0;--i)
    {
        for(size_t j=nc;j>0;--j)
        {
            Y_MK_ATOM_MMUL_RTRN();
        }
    }
}


//! parallel M = lhs * rhs'
template <typename MATRIX, typename LHS, typename RHS> static inline
void mmul_rtrn(MATRIX &M, const LHS &lhs, const RHS &rhs, concurrent::for_each &loop)
{
    assert(lhs.rows==M.rows);
    assert(rhs.rows==M.rows);
    assert(lhs.cols==rhs.cols);
    Y_MK_ATOM3_PAR(MMUL_RTRN,const size_t nc=M.cols);
}

#undef Y_MK_ATOM_MMUL_RTRN

//! SIMD kernel
#define Y_MK_ATOM_MMUL_LTRN()                          \
typename MATRIX::mutable_type sum(0);                  \
for(size_t k=np;k>0;--k)                               \
{                                                      \
sum +=                                                 \
Y_MK_ATOM_CAST(MATRIX,LHS,lhs[k][i])                   \
*                                                      \
Y_MK_ATOM_CAST(MATRIX,RHS,rhs[k][j] );                 \
}                                                      \
M[i][j] = sum;

//! sequential M = lhs' * rhs
template <typename MATRIX, typename LHS, typename RHS> static inline
void mmul_ltrn(MATRIX &M, const LHS &lhs, const RHS &rhs )
{
    assert(lhs.cols==M.rows);
    assert(lhs.rows==rhs.rows);
    assert(rhs.cols==M.cols);

    const size_t nc = M.cols;
    const size_t np = lhs.rows;
    for( size_t  i  = M.rows;i>0;--i)
    {
        for(size_t j=nc;j>0;--j)
        {
            Y_MK_ATOM_MMUL_LTRN();
        }
    }
}

//! parallel M = lhs' * rhs
template <typename MATRIX, typename LHS, typename RHS> static inline
void mmul_ltrn(MATRIX &M, const LHS &lhs, const RHS &rhs, concurrent::for_each &loop )
{
    assert(lhs.cols==M.rows);
    assert(lhs.rows==rhs.rows);
    assert(rhs.cols==M.cols);
    Y_MK_ATOM3_PAR(MMUL_LTRN,const size_t nc=M.cols;const size_t np=lhs.rows);

}

#undef Y_MK_ATOM_MMUL_LTRN

#undef Y_MK_ATOM3_PAR
