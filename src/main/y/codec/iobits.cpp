
#include "y/codec/iobits.hpp"

namespace upsylon
{
    iobits:: iobits() throw() : iobit::list(), pool()
    {
    }

    iobits:: ~iobits() throw()
    {
    }

    const uint64_t iobits::ibits[64] =
    {
        Y_U64(0x1), Y_U64(0x2), Y_U64(0x4), Y_U64(0x8), Y_U64(0x10), Y_U64(0x20), Y_U64(0x40), Y_U64(0x80),
        Y_U64(0x100), Y_U64(0x200), Y_U64(0x400), Y_U64(0x800), Y_U64(0x1000), Y_U64(0x2000), Y_U64(0x4000), Y_U64(0x8000),
        Y_U64(0x10000), Y_U64(0x20000), Y_U64(0x40000), Y_U64(0x80000), Y_U64(0x100000), Y_U64(0x200000), Y_U64(0x400000), Y_U64(0x800000),
        Y_U64(0x1000000), Y_U64(0x2000000), Y_U64(0x4000000), Y_U64(0x8000000), Y_U64(0x10000000), Y_U64(0x20000000), Y_U64(0x40000000), Y_U64(0x80000000),
        Y_U64(0x100000000), Y_U64(0x200000000), Y_U64(0x400000000), Y_U64(0x800000000), Y_U64(0x1000000000), Y_U64(0x2000000000), Y_U64(0x4000000000), Y_U64(0x8000000000),
        Y_U64(0x10000000000), Y_U64(0x20000000000), Y_U64(0x40000000000), Y_U64(0x80000000000), Y_U64(0x100000000000), Y_U64(0x200000000000), Y_U64(0x400000000000), Y_U64(0x800000000000),
        Y_U64(0x1000000000000), Y_U64(0x2000000000000), Y_U64(0x4000000000000), Y_U64(0x8000000000000), Y_U64(0x10000000000000), Y_U64(0x20000000000000), Y_U64(0x40000000000000), Y_U64(0x80000000000000),
        Y_U64(0x100000000000000), Y_U64(0x200000000000000), Y_U64(0x400000000000000), Y_U64(0x800000000000000), Y_U64(0x1000000000000000), Y_U64(0x2000000000000000), Y_U64(0x4000000000000000), Y_U64(0x8000000000000000)
    };

    void iobits:: push(const uint64_t value,
                       const size_t   nbits)
    {
        assert(nbits<=8*sizeof(uint64_t));
        size_t ibit = nbits;
        while(ibit-->0)
        {
            push( 0 != ( value & ibits[ibit] ) );
        }
    }


    std::ostream & iobits::display(std::ostream & os,
                                   const uint64_t value,
                                   const size_t   nbits)
    {
        assert(nbits<=8*sizeof(uint64_t));
        size_t ibit = nbits;
        while(ibit-->0)
        {
            const bool tst = ( 0 != ( value & ibits[ibit] ) );
            os << ( tst ? '1' : '0');
        }
        return os;
    }
}


