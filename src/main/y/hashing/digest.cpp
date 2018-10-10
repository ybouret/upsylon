#include "y/hashing/digest.hpp"
#include "y/code/utils.hpp"

namespace upsylon
{

    static inline
    uint8_t * __digest_acquire( size_t &blen )
    {
        static memory::allocator &mgr = memory::pooled::instance();
        if(blen)
        {
            return static_cast<uint8_t *>( mgr.acquire(blen) );
        }
        else
        {
            return NULL;
        }
    }

    void digest:: ldz() throw()
    {
        memset(byte,0,size);
    }

#define Y_DIGEST_CTOR(SZ)      \
memory::rw_buffer(),           \
counted(),                     \
size(SZ),                      \
blen(size),                    \
byte( __digest_acquire(blen) )

    digest:: digest( const size_t n ) :
    Y_DIGEST_CTOR(n)
    {
    }

    digest:: ~digest() throw()
    {
        assert( memory::pooled::exists() );
        static memory::allocator &mgr = memory::pooled::location();
        ldz();
        mgr.release( *(void **)&byte,blen);
    }

    const void * digest:: ro()     const throw() { return byte; }
    size_t       digest:: length() const throw() { return size; }

    uint8_t & digest:: operator[]( const size_t i ) throw()
    {
        assert(i<size); return byte[i];
    }

    const uint8_t & digest:: operator[]( const size_t i ) const throw()
    {
        assert(i<size); return byte[i];
    }

    std::ostream & digest:: display( std::ostream &os ) const
    {
        size_t i=size;
        while(i-->0)
        {
            os << hexadecimal::lowercase[ byte[i] ];
        }
        return os;
    }


    digest::digest( const digest &other ) :
    Y_DIGEST_CTOR(other.size)
    {
        memcpy(byte,other.byte,size);
    }

    digest & digest:: operator=( const digest &other ) throw()
    {
        if( this!=&other)
        {
            if(size<=other.size)
            {
                memcpy(byte,other.byte,size);
            }
            else
            {
                assert(other.size<size);
                memcpy(byte,other.byte,other.size);
                memset(byte+other.size,0,size-other.size);
            }
        }
        return *this;
    }
}

#include "y/randomized/bits.hpp"

namespace upsylon
{
    void digest:: rand() throw()
    {
        static randomized::bits & ran = randomized::bits::crypto();

        for(size_t i=0;i<size;++i)
        {
            byte[i] = ran.full<uint8_t>();
        }
    }

}
