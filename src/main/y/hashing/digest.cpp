#include "y/hashing/digest.hpp"
#include "y/code/utils.hpp"
#include "y/string.hpp"
#include "y/exception.hpp"
#include "y/memory/pooled.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon
{

    bool digest:: UPPER_CASE = false;
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
object(),                      \
memory::rw_buffer(),           \
counted(),                     \
ios::serializable(),           \
size(SZ),                      \
blen(size),                    \
byte( __digest_acquire(blen) )

    digest:: digest( const size_t n, const uint8_t b) :
    Y_DIGEST_CTOR(n)
    {
        memset(byte,b,size);
    }

    digest:: ~digest() throw()
    {
        assert( memory::pooled::exists() );
        static memory::allocator &mgr = memory::pooled::location();
        ldz();
        {
            void *p = byte;
            mgr.release(p,blen);
        }
        byte = 0;
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
        if( digest::UPPER_CASE )
        {
            for(size_t i=0;i<size;++i)
            {
                os << hexadecimal::uppercase[ byte[i] ];
            }
        }
        else
        {
            for(size_t i=0;i<size;++i)
            {
                os << hexadecimal::lowercase[ byte[i] ];
            }
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
        if(this!=&other)
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

    digest & digest:: operator=( const memory::ro_buffer &other) throw()
    {
        const memory::ro_buffer &self = *this;
        if( &self != &other )
        {
            const size_t olen = other.length();
            if(size<=olen)
            {
                memcpy(byte,other.ro(),size);
            }
            else
            {
                memcpy(byte,other.ro(),olen);
                memset(byte+olen,0,size-olen);
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

    digest digest:: hex(const char *txt, const size_t len)
    {
        assert(!(0==txt&&len>0));
        if(len&0x1)
        {
            size_t w = (len+1)>>1;
            digest d(w);
            d[0] = __digest_hex2dec(txt[0]);
            ++txt;
            size_t i=1;
            for(--w;w>0;--w,++i)
            {
                d[i] = __digest_word(txt);
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
                d[i] = __digest_word(txt);
            }
            return d;
        }
    }

    digest digest:: hex(const char *text)
    {
        return hex(text,length_of(text));
    }

    digest digest::hex( const string &s )
    {
        return hex(*s,s.size());
    }
    

    bool operator==( const digest &lhs, const digest &rhs) throw()
    {
        if(lhs.size==rhs.size)
        {
            for(size_t i=0;i<lhs.size;++i)
            {
                if(lhs[i]!=rhs[i]) return false;
            }
            return true;
        }
        else
        {
            return false;
        }
    }

    bool operator!=( const digest &lhs, const digest &rhs) throw()
    {
        if(lhs.size==rhs.size)
        {
            for(size_t i=0;i<lhs.size;++i)
            {
                if(lhs[i]!=rhs[i]) return true;
            }
            return false;
        }
        else
        {
            return true;
        }
    }


    void digest:: _xor( const digest &other ) throw()
    {
        assert(size==other.size);
        for(size_t i=0;i<size;++i)
        {
            byte[i] ^= other.byte[i];
        }
    }


    void digest:: _xor( const digest &lhs, const digest &rhs) throw()
    {
        assert(size==lhs.size);
        assert(size==rhs.size);
        for(size_t i=0;i<size;++i)
        {
            byte[i] = lhs.byte[i] ^ rhs.byte[i];
        }
    }

    void digest:: _swp( digest &other ) throw()
    {
        assert(size==other.size);
        for(size_t i=0;i<size;++i)
        {
            cswap(byte[i],other.byte[i]);
        }
    }

    void digest:: _inc(const uint8_t delta) throw()
    {
        unsigned sum = delta;
        for(size_t i=0;i<size;++i)
        {
            sum    += byte[i];
            byte[i] = (sum&0xff);
            sum >>= 8;
        }
    }

    void digest:: _inc() throw()
    {
        _inc(1);
    }


    void digest:: _set( const digest &other ) throw()
    {
        assert(size==other.size);
        memcpy(byte,other.byte,size);
    }

    const char digest:: CLASS_NAME[] = "digest";

    const char * digest:: className() const throw()
    {
        return CLASS_NAME;
    }

    size_t digest:: serialize( ios::ostream &fp ) const
    {
        return fp.emit_block(*this);
    }

    digest digest:: load(ios::istream &fp, size_t *nr)
    {
        if(nr     ) *nr = 0;
        const size_t sz = fp.read_upack<size_t>(nr);
        digest       md(sz,0); assert(sz==md.size);
        fp.input(md.byte,md.size);
        if(nr) (*nr) += sz;
        return md;
    }

    bool digest:: equals_hex(const char   *txt, const size_t len) const
    {
        const digest &lhs = *this;
        const digest  rhs = hex(txt,len);
        return lhs==rhs;
    }

    bool digest:: equals_hex(const char   *s) const
    {
        const digest &lhs = *this;
        const digest  rhs = hex(s);
        return lhs==rhs;
    }

    bool digest:: equals_hex(const string &s) const
    {
        const digest &lhs = *this;
        const digest  rhs = hex(s);
        return lhs==rhs;
    }

}


#include "y/randomized/bits.hpp"

namespace upsylon
{

    void digest:: rand() throw()
    {
        static randomized::bits & ran = randomized::bits::crypto();
        Y_LOCK( randomized::bits::access() );
        for(size_t i=0;i<size;++i)
        {
            byte[i] = ran.full<uint8_t>();
        }
    }
}
