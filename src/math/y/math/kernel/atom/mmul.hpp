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

template <typename LHS, typename MATRIX, typename RHS> static inline
void mul( LHS &lhs, const MATRIX &M, const RHS &rhs, concurrent::for_each &loop)
{
    assert(lhs.size()<=M.rows);
    assert(M.cols<=rhs.size());
    
    if( M.cols >= M.rows )
    {
        for(size_t i=M.rows;i>0;--i)
        {
            lhs[i] = static_cast< typename LHS::const_type >(dot(M[i],rhs,loop));
        }
    }
    else
    {
        struct ops
        {
            LHS          *lhs_;
            const MATRIX *M_;
            const RHS    *rhs_;
            
            static inline void call(void *args, parallel &ctx, lockable &)
            {
                ops          &self   = *static_cast<ops*>(args);
                LHS          &lhs    = *self.lhs_;
                const MATRIX &M      = *self.M_;
                const RHS    &rhs    = *self.rhs_;
                size_t        length = M.rows;
                size_t        offset = 1;
                ctx.split(length, offset);
                while(length-- > 0)
                {
                    lhs[offset] = dot(M[offset],rhs);
                    ++offset;
                }
            }
        };
        
        ops _ = { &lhs, &M, &rhs };
        loop.run( ops::call, &_ );
        
    }
}
