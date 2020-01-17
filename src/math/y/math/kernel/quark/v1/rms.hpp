//!\file

//!   root mean square
template <typename T>
struct rms
{
    Y_DECL_ARGS(T,type);                                     //!< aliases
    typedef typename real_for<mutable_type>::type real_type; //!< alias

    //! SEQUENTIAL: sqrt(|lhs|^2/lhs.size())
    template <typename LHS> static inline
    real_type of(LHS &lhs)
    {
        const size_t n   = lhs.size();
        if(n>0)
        {
            real_type tmp = mod2<T>::of( lhs );
            tmp /= n;
            return sqrt_of(tmp);
        }
        else
        {
            return 0;
        }
    }

    //! PARALLEL: sqrt(|lhs|^2/lhs.size())
    template <typename LHS> static inline
    real_type of(LHS &lhs, concurrent::for_each &loop )
    {
        const size_t n   = lhs.size();
        if(n>0)
        {
            real_type tmp = mod2<T>::of(lhs,loop);
            tmp /= n;
            return sqrt_of(tmp);
        }
        else
        {
            return 0;
        }
    }

    //! SEQUENTIAL: rms(lhs-rhs)
    template <typename LHS, typename RHS> static inline
    real_type of(LHS &lhs, RHS &rhs)
    {
        const size_t n   = lhs.size();
        if(n>0)
        {
            real_type tmp = mod2<T>::of( lhs, rhs );
            tmp /= n;
            return sqrt_of(tmp);
        }
        else
        {
            return 0;
        }
    }

    //! PARALLEL: rms(lhs-rhs)
    template <typename LHS, typename RHS> static inline
    real_type of(LHS &lhs, RHS &rhs, concurrent::for_each &loop)
    {
        const size_t n   = lhs.size();
        if(n>0)
        {
            real_type tmp = mod2<T>::of(lhs,rhs,loop);
            tmp /= n;
            return sqrt_of(tmp);
        }
        else
        {
            return 0;
        }
    }
};
