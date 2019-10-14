//! \file

//! SIMD kernel
#define Y_MK_ATOM_MMUL() \
typename MATRIX::mutable_type sum(0); \
for(size_t k=np;k>0;--k) \
{ \
    sum += static_cast<typename MATRIX::const_type>( lhs[i][k] ) * static_cast<typename MATRIX::const_type>( rhs[k][j] );\
}\
M[i][j] = sum;

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

template <typename MATRIX, typename LHS, typename RHS> static inline
void mmul(MATRIX &M, const LHS &lhs, const RHS &rhs, concurrent::for_each &loop )
{
    assert(lhs.rows==M.rows);
    assert(rhs.cols==M.cols);
    assert(lhs.cols==rhs.rows);

    struct ops
    {
        MATRIX    *M_;
        const LHS *lhs_;
        const RHS *rhs_;

        static inline void call(void     *args,
                                parallel &ctx,
                                lockable &)
        {
            ops         &self  = *static_cast<ops *>(args);
            MATRIX      &M     = *self.M_;
            const LHS   &lhs   = *self.lhs_;
            const RHS   &rhs   = *self.rhs_;
            const size_t nc    = M.cols;
            const size_t np    = lhs.cols;
            size_t      length = M.items;
            size_t      offset = 0;
            ctx.split(length,offset);
            while(length-- > 0 )
            {
                {
                    const size_t i=1+(offset/nc);
                    const size_t j=1+(offset%nc);
                    Y_MK_ATOM_MMUL();
                }
                ++offset;
            }

        }
    };
    ops params = { &M, &lhs, &rhs };
    loop.run( & ops::call, &params );

}

#undef Y_MK_ATOM_MMUL

