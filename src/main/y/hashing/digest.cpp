#include "y/hashing/digest.hpp"
#include "y/code/utils.hpp"
#include "y/exception.hpp"

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
#if 0
        size_t i=size;
        while(i-->0)
        {
            os << hexadecimal::lowercase[ byte[i] ];
        }
#endif
        for(size_t i=0;i<size;++i)
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

    static inline
    uint8_t __digest_hex2dec(const char c)
    {
        const int value = hexadecimal::to_decimal(c);
        if(value<0) throw exception("digest::hexadecimal(invalid char '%s')", visible_char[ uint8_t(c) ]);
        return uint8_t(value);
    }

    static inline uint8_t __digest_word(const char * &text)
    {
        assert(text);
        const uint8_t hi = __digest_hex2dec(*(text++));
        const uint8_t lo = __digest_hex2dec(*(text++));
        return (hi<<4) | lo;
    }

    digest digest:: hexadecimal(const char *text)
    {
        const size_t len = length_of(text);
        if(len&0x1)
        {
            size_t w = (len+1)>>1;
            digest d(w);
            d[0] = __digest_hex2dec(text[0]);
            ++text;
            size_t i=1;
            for(--w;w>0;--w,++i)
            {
                d[i] = __digest_word(text);
            }
            return d;
        }
        else
        {
            size_t w = len>>1;
            digest d(w);
            size_t i=0;
            for(;w>0;--w,++i)
            {
                d[i] = __digest_word(text);
            }
            return d;
        }
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
