
#include "y/container/matrix.hpp"
#include "y/exceptions.hpp"

namespace upsylon
{
    matrix_dims:: ~matrix_dims() throw()
    {
    }



    matrix_dims:: matrix_dims(const size_t nr, const size_t nc,const size_t item_size) :
    rows(nr),
    cols(nc),
    items(rows*cols),
    is_square(rows==cols)
    {
        assert(item_size>0);
        if( (nr>0&&nc==0) || (nc>0&&nr==0) )
        {
            throw libc::exception(EDOM,"matrix invalid dimensions rows=%u,cols=%u", unsigned(rows), unsigned(cols));
        }
    }



}
