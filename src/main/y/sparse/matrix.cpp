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


    }

    namespace sparse
    {
        matrix_info:: ~matrix_info() throw()
        {
            (size_t&)rows = 0;
            (size_t&)cols = 0;

        }

        static const char fn[] = "sparse_matrix: ";

        matrix_info:: matrix_info( const size_t nr, const size_t nc ) :
        rows(nr),
        cols(nc)
        {
            if( ((rows<=0) || (cols<=0)) && (rows||cols) )
            {
                throw exception("%sbad dimensions %ux%u",fn,unsigned(rows),unsigned(cols));
            }
        }

    }

}

