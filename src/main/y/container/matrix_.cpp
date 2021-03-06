#include "y/container/matrix_.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/cswap.hpp"

#include "y/type/aliasing.hpp"
#include "y/exceptions.hpp"
#include "y/type/utils.hpp"
#include "y/type/standard.hpp"

#include "y/memory/allocator/global.hpp"

#include <cerrno>

namespace upsylon
{
    void matrix_:: __free() throw()
    {
        memory::global::location().release(workspace,aliasing::_(allocated));
    }

    matrix_:: ~matrix_() throw()
    {
        __free();
        _bzset(rows);
        _bzset(cols);
        _bzset(items);
        _bzset(is_square);
        _bzset(largest);
        _bzset(total_items);
    }


    matrix_:: matrix_(const size_t nr, const size_t nc,const size_t item_size) :
    rows(nr),
    cols(nc),
    items(rows*cols),
    is_square(rows==cols),
    largest(max_of(rows,cols)),
    total_items(items+2*largest),
    _data(0,total_items*item_size),
    _rows(_data,rows * sizeof( lightweight_array<int> ) ),
    _indx(_rows,sizeof(size_t) * largest ),
    allocated(   _indx.next_offset() ),
    r_indices(),
    c_indices(),
    workspace( memory::global::instance().acquire( aliasing::_(allocated) ) )
    {
        assert(item_size>0);
        // sanity check
        if( (nr>0&&nc==0) || (nc>0&&nr==0) )
        {
            __free();
            throw libc::exception(EDOM,"matrix invalid dimensions rows=%u,cols=%u", unsigned(rows), unsigned(cols));
        }

        // hook data
        hook();
    }

    void matrix_:: hook() throw()
    {
        // hook indices
        size_t *ipos = _indx.as<size_t>(workspace); //memory::io::cast<size_t>(workspace,_indx.offset);
        new ( &r_indices ) indices_type(ipos,rows);
        new ( &c_indices ) indices_type(ipos,cols);
    }

#define _XCH(FIELD) _cswap(FIELD,other.FIELD)

    void matrix_:: exchange( matrix_ &other ) throw()
    {
        // exchange fields
        _XCH(rows);
        _XCH(cols);
        _XCH(items);
        _XCH(is_square);
        _XCH(largest);
        _XCH(total_items);
        _XCH(_data);
        _XCH(_rows);
        _XCH(_indx);
        _XCH(allocated);
        _XCH(workspace);

        // reset arrays
        hook();
        other.hook();

        // sanity check
        assert( r_indices.size() == rows );
        assert( c_indices.size() == cols );
    }

    void matrix_:: get_item( const size_t item, size_t &r, size_t &c) const throw()
    {
        static const standard<size_t>::api::div_proc div_call = standard<size_t>::api::div_call;
        assert(item<items);
        standard<size_t>::div_type d = div_call(item,cols);
        r=static_cast<size_t>(++d.quot); assert(r>0); assert(r<=rows);
        c=static_cast<size_t>(++d.rem);  assert(c>0); assert(c<=cols);
        assert((r-1)*cols+(c-1)==item);
    }

    bool matrix_:: same_size_than( const matrix_ &other ) const throw()
    {
        return (rows==other.rows) && (cols==other.cols);
    }


}
