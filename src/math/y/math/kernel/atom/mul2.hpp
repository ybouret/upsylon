//! \file

//! sequential operators
template <
typename LHS,
typename MATRIX,
typename RHS,
typename MUL_OP> static inline
void mul2__( LHS &lhs, const MATRIX &M, const RHS &rhs, MUL_OP &op )
{
    assert(lhs.size()<=M.rows);
    assert(M.cols<=rhs.size());
    
    for(size_t i=lhs.size();i>0;--i)
    {
        typename MATRIX::const_type tmp = dot(M[i],rhs);
        op(lhs[i],tmp);
    }
}

//! sequential lhs = M * rhs
template <typename LHS, typename MATRIX, typename RHS> static inline
void mul( LHS &lhs, const MATRIX &M, const RHS &rhs)
{
    mul2__( lhs, M, rhs, mulop::__set_v2<typename LHS::mutable_type,typename MATRIX::mutable_type>);
}


//! sequential lhs += M * rhs
template <typename LHS, typename MATRIX, typename RHS> static inline
void mul_add( LHS &lhs, const MATRIX &M, const RHS &rhs)
{
    mul2__( lhs, M, rhs, mulop::__add_v2<typename LHS::mutable_type,typename MATRIX::mutable_type>);
}

//! sequential lhs -= M * rhs
template <typename LHS, typename MATRIX, typename RHS> static inline
void mul_sub( LHS &lhs, const MATRIX &M, const RHS &rhs)
{
    mul2__( lhs, M, rhs, mulop::__sub_v2<typename LHS::mutable_type,typename MATRIX::mutable_type>);
}

//!
template <typename LHS, typename MATRIX, typename RHS, typename MUL_OP> static inline
void mul2__( LHS &lhs, const MATRIX &M, const RHS &rhs, concurrent::for_each &loop, MUL_OP &op)
{
    assert(lhs.size()<=M.rows);
    assert(M.cols<=rhs.size());
    
    if( M.cols >= M.rows )
    {
        // parallel over cols
        for(size_t i=M.rows;i>0;--i)
        {
            typename MATRIX::const_type tmp = dot(M[i],rhs,loop);
            op(lhs[i],tmp);
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
            MUL_OP       *op_;
            
            static inline void call(void *args, parallel &ctx, lockable &)
            {
                ops          &self   = *static_cast<ops*>(args);
                LHS          &lhs    = *self.lhs_;
                const MATRIX &M      = *self.M_;
                const RHS    &rhs    = *self.rhs_;
                MUL_OP       &op     = *self.op_;
                size_t        length = lhs.size();
                size_t        offset = 1;
                ctx.split(length, offset);
                while(length-- > 0)
                {
                    typename MATRIX::const_type tmp = dot(M[offset],rhs);
                    op(lhs[offset],tmp);
                    ++offset;
                }
            }
        };
        
        ops _ = { &lhs, &M, &rhs, &op };
        loop.run( ops::call, &_ );
        
    }
}

//! parallel lhs = M  * rhs
template <typename LHS, typename MATRIX, typename RHS> static inline
void mul( LHS &lhs, const MATRIX &M, const RHS &rhs, concurrent::for_each &loop)
{
    mul2__(lhs,M,rhs,loop,mulop::__set_v2<typename LHS::mutable_type,typename MATRIX::mutable_type>);
}


//! parallel lhs += M  * rhs
template <typename LHS, typename MATRIX, typename RHS> static inline
void mul_add( LHS &lhs, const MATRIX &M, const RHS &rhs, concurrent::for_each &loop)
{
    mul2__(lhs,M,rhs,loop,mulop::__add_v2<typename LHS::mutable_type,typename MATRIX::mutable_type>);
}

//! parallel lhs -= M  * rhs
template <typename LHS, typename MATRIX, typename RHS> static inline
void mul_sub( LHS &lhs, const MATRIX &M, const RHS &rhs, concurrent::for_each &loop)
{
    mul2__(lhs,M,rhs,loop,mulop::__sub_v2<typename LHS::mutable_type,typename MATRIX::mutable_type>);
}



//! SIMD kernel
#define Y_MK_ATOM_MUL_TRN(i) do {\
for(size_t j=nr;j>0;--j) {\
sum += Y_MK_ATOM_CAST(LHS,MATRIX,M[j][i]) * Y_MK_ATOM_CAST(LHS,RHS,rhs[j]);\
} } while(false)

//! sequential op(lhs,M' * rhs)
template <typename LHS, typename MATRIX, typename RHS, typename MUL_OP> static inline
void mul_trn_( LHS &lhs, const MATRIX &M, const RHS &rhs, MUL_OP &op)
{
    assert(lhs.size()<=M.cols);
    assert(M.rows<=rhs.size());
    
    const size_t nr = M.rows;
    for(size_t i=lhs.size();i>0;--i)
    {
        typename LHS::mutable_type sum(0);
        Y_MK_ATOM_MUL_TRN(i);
        op(lhs[i],sum);
    }
}

//! sequential lhs = M' * rhs, sub-optimal
template <typename LHS, typename MATRIX, typename RHS> static inline
void mul_trn( LHS &lhs, const MATRIX &M, const RHS &rhs)
{
    mul_trn_( lhs, M, rhs, mulop::__set_v2<typename LHS::mutable_type,typename MATRIX::mutable_type>);
}

//! sequential lhs += M' * rhs, sub-optimal
template <typename LHS, typename MATRIX, typename RHS> static inline
void mul_add_trn( LHS &lhs, const MATRIX &M, const RHS &rhs)
{
    mul_trn_( lhs, M, rhs, mulop::__add_v2<typename LHS::mutable_type,typename MATRIX::mutable_type>);
}

//! sequential lhs -= M' * rhs, sub-optimal
template <typename LHS, typename MATRIX, typename RHS> static inline
void mul_sub_trn( LHS &lhs, const MATRIX &M, const RHS &rhs)
{
    mul_trn_( lhs, M, rhs, mulop::__sub_v2<typename LHS::mutable_type,typename MATRIX::mutable_type>);
}


//! parallel op(lhs,M' * rhs) only on lhs.size()
template <typename LHS, typename MATRIX, typename RHS, typename MUL_OP> static inline
void mul_trn_( LHS &lhs, const MATRIX &M, const RHS &rhs, concurrent::for_each &loop, MUL_OP &op )
{
    assert(lhs.size()<=M.cols);
    assert(M.rows<=rhs.size());
    
    {
        typename LHS::const_type __zero(0);
        loop.engine().build_from<typename LHS::mutable_type>(__zero);
    }
    
    struct ops
    {
        LHS          *lhs_;
        const MATRIX *M_;
        const RHS    *rhs_;
        MUL_OP       *op_;
        
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
            MUL_OP                     &op   = *self.op_;
            size_t                      l    = lhs.size();
            size_t                      i    = 1;
            ctx.split(l, i);
            while(l-- > 0 )
            {
                Y_MK_ATOM_MUL_TRN(i);
                op(lhs[i],sum);
                ++i;
            }
            
        }
    };
    
    ops params = { &lhs, &M, &rhs , &op };
    loop.run( ops::call, &params );
    
}


//! parallel lhs = M' * rhs
template <typename LHS, typename MATRIX, typename RHS> static inline
void mul_trn( LHS &lhs, const MATRIX &M, const RHS &rhs, concurrent::for_each &loop )
{
    mul_trn_( lhs, M, rhs, loop, mulop::__set_v2<typename LHS::mutable_type,typename MATRIX::mutable_type>);
}

//! parallel lhs += M' * rhs
template <typename LHS, typename MATRIX, typename RHS> static inline
void mul_add_trn( LHS &lhs, const MATRIX &M, const RHS &rhs, concurrent::for_each &loop )
{
    mul_trn_( lhs, M, rhs, loop, mulop::__add_v2<typename LHS::mutable_type,typename MATRIX::mutable_type>);
}

//! parallel lhs -= M' * rhs
template <typename LHS, typename MATRIX, typename RHS> static inline
void mul_sub_trn( LHS &lhs, const MATRIX &M, const RHS &rhs, concurrent::for_each &loop )
{
    mul_trn_( lhs, M, rhs, loop, mulop::__sub_v2<typename LHS::mutable_type,typename MATRIX::mutable_type>);
}
