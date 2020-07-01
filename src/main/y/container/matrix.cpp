
#include "y/container/matrix.hpp"
#include "y/exceptions.hpp"
#include "y/type/utils.hpp"
#include "y/type/cswap.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/standard.hpp"

#include <cerrno>

namespace upsylon
{
    const matrix_transpose_t matrix_transpose = {};
    
    void matrix_data:: __kill() throw()
    {
        memory::global::location().release(workspace,aliasing::_(allocated));
    }

    matrix_data:: ~matrix_data() throw()
    {
        __kill();
        _bzset(rows);
        _bzset(cols);
        _bzset(items);
        _bzset(is_square);
        _bzset(largest);
        _bzset(total_items);
        _bzset(data_offset);
        _bzset(data_length);
        _bzset(rows_offset);
        _bzset(rows_length);
        _bzset(indx_offset);
        _bzset(indx_length);
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
    allocated(   memory::align(indx_offset+indx_length)  ),
    r_indices(),
    c_indices(),
    workspace( memory::global::instance().acquire( aliasing::_(allocated) ) )
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
        new ( &r_indices ) indices_type(ipos,rows);
        new ( &c_indices ) indices_type(ipos,cols);
    }

#define _XCH(FIELD) _cswap(FIELD,other.FIELD)
    
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
        assert( r_indices.size() == rows );
        assert( c_indices.size() == cols );
    }

    void matrix_data:: get_item( const size_t item, size_t &r, size_t &c) const throw()
    {
        assert(item<items);
        const standard<size_t>::div_type d = standard<size_t>::api::div_call(item,cols);
        //const ldiv_t d = ldiv(long(item),long(cols));
        r=d.quot+1; assert(r>0); assert(r<=rows);
        c=d.rem +1; assert(c>0); assert(c<=cols);
        assert((r-1)*cols+(c-1)==item);
    }

    bool matrix_data:: same_size_than( const matrix_data &other ) const throw()
    {
        return (rows==other.rows) && (cols==other.cols);
    }

}
