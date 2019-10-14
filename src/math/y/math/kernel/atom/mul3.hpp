//! \file

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
            typename MATRIX::mutable_type sum(0);
            for(size_t k=np;k>0;--k)
            {
                sum += static_cast<typename MATRIX::const_type>( lhs[i][k] ) * static_cast<typename MATRIX::const_type>( rhs[k][j] );
            }
            M[i][j] = sum;
        }
    }

}
