#include "y/type/binary.hpp"
#include "y/type/block-zset.hpp"

#include <cstring>

namespace upsylon {

    binary_:: ~binary_() throw()
    {
        memset((void*)byte,0,size);
        _bzset(size);
    }

    binary_:: binary_( const size_t n, const uint8_t *p) throw() :
    size(n),
    byte(p)
    {
        assert(n>0);
        assert(p!=NULL);
    }

    void binary_:: fill_with(const void *p) throw()
    {
        assert(p!=NULL);
        memcpy((void*)byte,p,size);
    }

    bool operator==(const binary_ &lhs, const binary_ &rhs) throw()
    {
        if(lhs.size!=rhs.size)
        {
            return false;
        }
        else
        {
            size_t i=lhs.size;
            while(i-- > 0 )
            {
                if( lhs.byte[i] != rhs.byte[i] ) return false;
            }
            return true;
        }
    }

    bool operator!=(const binary_ &lhs, const binary_ &rhs) throw()
    {
        return ! (lhs==rhs);
    }

}

#include <iostream>
#include "y/code/utils.hpp"

namespace upsylon {

    std::ostream & operator<<( std::ostream &os, const binary_ &b)
    {
        for(size_t i=0;i<b.size;++i)
        {
            os << hexadecimal::lowercase[ b.byte[i] ];
        }
        return os;
    }

}
