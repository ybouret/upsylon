//! \file

template <typename LHS, typename MATRIX, typename RHS> static inline
void mul( LHS &lhs, const MATRIX &M, const RHS &rhs)
{
    assert(lhs.size()<=M.rows);
    assert(M.cols<=rhs.size());
    
    for(size_t i=M.rows;i>0;--i)
    {
        lhs[i] = static_cast< typename LHS::const_type>(dot(M[i],rhs));
    }
}
