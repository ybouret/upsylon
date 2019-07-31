#include "y/sparse/matrix.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace sparse
    {
        matrix_key:: ~matrix_key() throw()
        {
            (size_t &)r = 0;
            (size_t &)c = 0;
        }

        matrix_key:: matrix_key( const size_t R, const size_t C ) throw() :
        r(R),c(C)
        {
        }

        matrix_key:: matrix_key( const matrix_key &k ) throw() : r(k.r), c(k.c) {}

        bool operator==(const matrix_key &lhs, const matrix_key &rhs) throw()
        {
            return (lhs.r==rhs.r) && (lhs.c==rhs.c);
        }

        bool operator!=(const matrix_key &lhs, const matrix_key &rhs) throw()
        {
            return (lhs.r!=rhs.r) || (lhs.c!=rhs.c);
        }

        bool operator<(const matrix_key &lhs, const matrix_key &rhs) throw()
        {
            if( lhs.r < rhs.r )
            {
                return true;
            }
            else if( rhs.r < lhs.r )
            {
                return false;
            }
            else
            {
                assert(lhs.r==rhs.r);
                return (lhs.c<rhs.c);
            }
        }

        std::ostream & operator<<( std::ostream &os, const matrix_key &k )
        {
            os << '(' << k.r << '.' << k.c << ')';
            return os;
        }
    }

    namespace sparse
    {
        matrix_info:: ~matrix_info() throw()
        {
            (size_t&)rows = 0;
            (size_t&)cols = 0;
            (size_t&)count = 0;
        }

        static const char fn[] = "sparse_matrix: ";

        matrix_info:: matrix_info( const size_t nr, const size_t nc ) :
        rows(nr),
        cols(nc),
        count(rows*cols)
        {
            if( ((rows<=0) || (cols<=0)) && (rows||cols) )
            {
                throw exception("%sbad dimensions %ux%u",fn,unsigned(rows),unsigned(cols));
            }
        }

        void matrix_info:: check_indices(const size_t r,const size_t c) const
        {
            if( (r<=0) || (r>rows) || (c<=0) || (c>cols) )
                throw exception("%sinvalid indices (%u,%u) not in [1:%u]x[1:%u]", fn, unsigned(r), unsigned(c), unsigned(rows), unsigned(cols) );
        }

        void matrix_info:: insert_failure(const sparse::matrix_key  &k) const
        {
            throw exception("%s: unexpected failure to insert item@(%u,%u)",fn,unsigned(k.r),unsigned(k.c));
        }

    }

}

