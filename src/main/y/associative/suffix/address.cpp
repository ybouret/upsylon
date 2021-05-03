#include "y/associative/suffix/address.hpp"
#include "y/type/block/zset.hpp"
#include "y/os/endian.hpp"

namespace upsylon
{

    const void * suffix_address:: be(const void *addr) throw()
    {
        return swap_be_as(addr);
    }


    suffix_address:: ~suffix_address() throw()
    {
        _bzset(data);
    }

    suffix_address:: suffix_address(const suffix_address &other) throw() :
    memory::ro_buffer(),
    data( other.data )
    {
    }

    size_t suffix_address:: length() const throw()
    {
        return sizeof(data);
    }

    const void * suffix_address:: ro() const throw()
    {
        return & data;
    }

    const void *suffix_address:: addr()   const throw()
    {
        return be(data);
    }


}


