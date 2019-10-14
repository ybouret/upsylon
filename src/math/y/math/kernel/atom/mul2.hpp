//! \file

//! sequential lhs = M * rhs
template <typename LHS, typename MATRIX, typename RHS> static inline
void mul( LHS &lhs, const MATRIX &M, const RHS &rhs)
{
    assert(lhs.size()<=M.rows);
    assert(M.cols<=rhs.size());
    
    for(size_t i=lhs.size();i>0;--i)
    {
        lhs[i] = static_cast< typename LHS::const_type>(dot(M[i],rhs));
    }
}

//! parallel lhs = M  * rhs
template <typename LHS, typename MATRIX, typename RHS> static inline
void mul( LHS &lhs, const MATRIX &M, const RHS &rhs, concurrent::for_each &loop)
{
    assert(lhs.size()<=M.rows);
    assert(M.cols<=rhs.size());
    
    if( M.cols >= M.rows )
    {
        // parallel over cols
        for(size_t i=M.rows;i>0;--i)
        {
            lhs[i] = static_cast< typename LHS::const_type >(dot(M[i],rhs,loop));
        }
    }
    else
    {
        // parallel over rows
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
                size_t        length = lhs.size();
                size_t        offset = 1;
                ctx.split(length, offset);
                while(length-- > 0)
                {
                    lhs[offset] = static_cast< typename LHS::const_type >( dot(M[offset],rhs) );
                    ++offset;
                }
            }
        };
        
        ops _ = { &lhs, &M, &rhs };
        loop.run( ops::call, &_ );
        
    }
}

//! SIMD kernel
#define Y_MK_ATOM_MUL_TRN(i) do {\
for(size_t j=nr;j>0;--j) {\
sum += static_cast< typename LHS::const_type >(M[j][i]) * static_cast< typename LHS::const_type >(rhs[j]);\
} } while(false)

//! sequential lhs = M' * rhs, sub-optimal
template <typename LHS, typename MATRIX, typename RHS> static inline
void mul_trn( LHS &lhs, const MATRIX &M, const RHS &rhs)
{
    assert(lhs.size()<=M.cols);
    assert(M.rows<=rhs.size());
    
    const size_t nr = M.rows;
    for(size_t i=lhs.size();i>0;--i)
    {
        typename LHS::mutable_type sum(0);
        Y_MK_ATOM_MUL_TRN(i);
        lhs[i] = sum;
    }
}


//! parallel lhs = M' * rhs, only on lhs.size()
template <typename LHS, typename MATRIX, typename RHS> static inline
void mul_trn( LHS &lhs, const MATRIX &M, const RHS &rhs, concurrent::for_each &loop )
{
    assert(lhs.size()<=M.cols);
    assert(M.rows<=rhs.size());
    
    mul_trn(lhs,M,rhs);
    {
        typename LHS::const_type __zero(0);
        loop.engine().build_from<typename LHS::mutable_type>(__zero);
    }

    struct ops
    {
        LHS          *lhs_;
        const MATRIX *M_;
        const RHS    *rhs_;

        static inline void call(void     *args,
                                parallel &ctx,
                                lockable &)
        {
            ops                        &self = *static_cast<ops *>(args);
            LHS                        &lhs  = *self.lhs_;
            const MATRIX               &M    = *self.M_;
            const RHS                  &rhs  = *self.rhs_;
            const size_t                nr   = M.rows;
            typename LHS::mutable_type &sum  = ctx.get<typename LHS::mutable_type>();
            size_t                      l    = lhs.size();
            size_t                      i    = 1;
            ctx.split(l, i);
            while(l-- > 0 )
            {
                Y_MK_ATOM_MUL_TRN(i);
                lhs[i] = sum;
                ++i;
            }
            
        }
    };

    ops params = { &lhs, &M, &rhs };
    loop.run( ops::call, &params );

}
