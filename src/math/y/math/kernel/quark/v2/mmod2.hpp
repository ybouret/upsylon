
//! matrix mod2 for row/col/...
template <typename T>
struct mmod2
{
    Y_DECL_ARGS(T,type);                                     //!< aliases
    typedef typename real_for<mutable_type>::type real_type; //!< alias

    //! |M[r][...]|^2
    static inline
    real_type row_of( const matrix<T> &M, const size_t r )
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
    real_type col_of( const matrix<T> &M, const size_t c )
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

};

