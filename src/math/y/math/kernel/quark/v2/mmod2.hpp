
//! matrix mod2 for row/col/...
template <typename T>
struct mmod2
{
    Y_DECL_ARGS(T,type);                                     //!< aliases
    typedef typename real_for<mutable_type>::type real_type; //!< alias

    //! |M[r][...]|^2
    static inline
    real_type row_of(const matrix<T> &M, const size_t r)
    {
        real_type ans = 0;
        for(size_t j=M.cols;j>0;--j)
        {
            ans += __mod2( M[r][j] );
        }
        return ans;
    }

    //! |M[...][c]|^2
    static inline
    real_type col_of(const matrix<T> &M, const size_t c)
    {
        real_type ans = 0;
        for(size_t i=M.rows;i>0;--i)
        {
            ans += __mod2( M[i][c] );
        }
        return ans;
    }

    //! sum of both
    static inline
    real_type both_of( const matrix<T> &M, const size_t r, const size_t c )
    {
        return row_of(M,r)+col_of(M,c);
    }

    //! SEQUENTIAL: |A|^2
    static inline
    real_type of(const matrix<T> &A)
    {
        typename matrix<T>::const_row arr = A.as_array();
        return mod2<T>::of(arr);
    }

    //! PARALLEL: |A|^2
    static inline
    real_type of(const matrix<T> &A, concurrent::for_each &loop)
    {
        typename matrix<T>::const_row arr = A.as_array();
        return mod2<T>::of(arr,loop);
    }

    //! SEQUENTIAL: |A-B|^2
    static inline
    real_type of(const matrix<T> &A, const matrix<T> &B)
    {
        typename matrix<T>::const_row  arr = A.as_array(), brr = B.as_array();
        return mod2<T>::of(arr,brr);
    }

    //! PARALLEL: |A-B|^2
    static inline
    real_type of(const matrix<T> &A, const matrix<T> &B, concurrent::for_each &loop)
    {
        typename matrix<T>::const_row  arr = A.as_array(), brr = B.as_array();
        return mod2<T>::of(arr,brr,loop);
    }

};

