
#include "y/container/matrix.hpp"
#include "y/exceptions.hpp"
#include "y/type/utils.hpp"
#include <cerrno>

namespace upsylon
{
    const matrix_transpose_t matrix_transpose = {};
    
    void matrix_data:: __kill() throw()
    {
        memory::global::location().release(workspace,(size_t&)allocated);
    }

    matrix_data:: ~matrix_data() throw()
    {
        __kill();
    }



    matrix_data:: matrix_data(const size_t nr, const size_t nc,const size_t item_size) :
    rows(nr),
    cols(nc),
    items(rows*cols),
    is_square(rows==cols),
    largest(max_of(rows,cols)),
    total_items(items+2*largest),
    data_offset(0),
    data_length( total_items*item_size                   ),
    rows_offset( memory::align(data_offset+data_length)  ),
    rows_length( rows * sizeof( lightweight_array<int> ) ),
    indx_offset( memory::align(rows_offset+rows_length)  ),
    indx_length( sizeof(size_t) * largest                ),
    allocated( memory::align(indx_offset+indx_length)    ),
    r_indx(),
    c_indx(),
    workspace( memory::global::instance().acquire((size_t&)allocated) )
    {
        assert(item_size>0);
        // sanity check
        if( (nr>0&&nc==0) || (nc>0&&nr==0) )
        {
            __kill();
            throw libc::exception(EDOM,"matrix invalid dimensions rows=%u,cols=%u", unsigned(rows), unsigned(cols));
        }

        hook();

    }

    void matrix_data:: hook() throw()
    {
        // hook indices
        size_t *ipos = memory::io::cast<size_t>(workspace,indx_offset);
        new ( &r_indx ) lightweight_array<size_t>(ipos,rows);
        new ( &c_indx ) lightweight_array<size_t>(ipos,cols);
    }

#define _XCH(FIELD) cswap(FIELD,other.FIELD)
    void matrix_data:: exchange( matrix_data &other ) throw()
    {
        // exchange fields
        _XCH(rows);
        _XCH(cols);
        _XCH(items);
        _XCH(is_square);
        _XCH(largest);
        _XCH(total_items);
        _XCH(data_offset);
        _XCH(data_length);
        _XCH(rows_offset);
        _XCH(rows_length);
        _XCH(indx_offset);
        _XCH(indx_length);
        _XCH(allocated);
        _XCH(workspace);
        // reset arrays
        hook();
        other.hook();
        assert( r_indx.size() == rows );
        assert( c_indx.size() == cols );
    }

}
