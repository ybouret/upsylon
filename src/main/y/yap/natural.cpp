
#include "y/yap/natural.hpp"
#include "y/os/endian.hpp"

namespace upsylon {

    namespace yap {

#define Y_APN_CHECK(EXPR) do { \
/**/ if(!(EXPR))\
/**/     {\
/**/         std::cerr <<  pfx << which << mid << #EXPR << sfx  << std::endl;\
/**/         return false;\
/**/     }\
/**/ } while(false)

        bool natural:: check(const natural &n, const char *which) throw()
        {
            assert(which);
            static const char pfx[] = "***[natural] '";
            static const char mid[] = "' : ";
            static const char sfx[] = " FAILURE!";
            //std::cerr << "..checking " << which << std::endl;
            Y_APN_CHECK(n.words==words_for(n.bytes));
            Y_APN_CHECK(n.count>=n.words);
            Y_APN_CHECK(n.count*word_size==n.width);
            Y_APN_CHECK(n.width == (size_t(1) << n.shift) );
            Y_APN_CHECK(n.bytes <= n.width );
            if(n.bytes>0)
            {
                Y_APN_CHECK(n.get(n.bytes-1)>0);
            }
            for(size_t remaining=n.bytes;remaining<n.width;++remaining)
            {
                Y_APN_CHECK(0==n.get(remaining));
            }
            return true;
        }


        natural:: ~natural() throw()
        {
            assert(check(*this,"self@~natural"));
            release(word,count,width,shift);
            bytes=0;
            words=0;
        }

        size_t natural:: words_for(const size_t bytes) throw()
        {
            return Y_ROUND_LN2(word_exp2,bytes)>>word_exp2;
        }

        word_type * natural:: acquire(size_t &count, size_t &width, size_t &shift)
        {
            static memory_allocator &mgr = instance();
            return mgr.acquire_field<word_type>(count,width,shift);
        }

        void natural:: release(word_type *w, size_t &count, size_t &width, size_t &shift) throw()
        {
            static memory_allocator &mgr = instance();
            mgr.release_field(w,count,width,shift);
        }

        natural::natural() : Y_APN_CTOR(0)
        {
            assert(check(*this,"self@init"));
        }
        
        natural:: natural(const size_t num_bytes, const as_capacity_t &) :
        Y_APN_CTOR(num_bytes)
        {
            assert(check(*this,"self@init(capa)"));
        }

        natural:: natural(const natural &other) :
        number(),
        bytes(other.bytes),
        words(other.words),
        count( words_for(bytes) ), width(0), shift(0),
        word( acquire(count,width,shift) )
        {
            assert(check(other,"other@copy"));
            for(size_t w=words;w>0;)
            {
                --w;
                word[w] = other.word[w];
            }
            assert(check(*this,"self@copy"));
        }

        natural:: natural(utype u) : Y_APN_CTOR( sizeof(utype) )
        {
            assert(count>=words_per_utype);

            for(size_t i=0;i<words_per_utype;++i)
            {
                word[i] = word_type(u);
                u >>= word_bits;
            }
            upgrade();
            assert(check(*this,"self@init(utype)"));

        }



        utype natural::lsw() const throw()
        {
            assert(check(*this,"self@lsw"));
            utype u = 0;
            for(size_t i=words_per_utype;i>0;)
            {
                u <<= word_bits;
                u |=  word[--i];
            }
            return u;
        }

        size_t natural:: bits() const throw()
        {
            if(bytes<=0)
            {
                return 0;
            }
            else
            {
                const size_t  bm1 = bytes-1;
                const uint8_t msb = get(bm1);
                assert(msb);
                return (bm1 << 3) + bits_table::count_for_byte[ msb ];
            }
        }

        size_t natural:: size() const throw()
        {
            return bytes;
        }
        
        size_t natural:: wc() const throw()
        {
            return words;
        }



        void natural:: update() throw()
        {
            size_t curr = bytes;
            size_t prev = curr-1;
            while(curr>0&&get(prev)<=0)
            {
                --curr;
                --prev;
            }
            bytes = curr;
            words = words_for(bytes);
            assert( check(*this,"self@update") );
        }

        void natural:: upgrade() throw()
        {
            bytes=width;
            update();
        }

        uint8_t & natural:: get(size_t i) const throw()
        {
            assert(i<width);
            uint8_t *addr = (uint8_t*) &word[i>>word_exp2];
            i &= word_mask;
#if Y_BYTE_ORDER == Y_LIT_ENDIAN
            return addr[i];
#else
            return addr[word_mask-i];
#endif
        }


        void natural:: xch(natural &other) throw()
        {
            assert(check(*this,"self@xch"));
            assert(check(other,"other@xch)"));

            cswap(bytes,other.bytes);
            cswap(words,other.words);
            cswap(count,other.count);
            cswap(width,other.width);
            cswap(shift,other.shift);
            cswap(word, other.word );
            
            assert(check(*this,"self@xch"));
            assert(check(other,"other@xch)"));
        }

        natural & natural:: operator=(const natural &other)
        {
            assert(check(*this,"self@="));
            assert(check(other,"other@=)"));
            natural tmp(other); xch(tmp);
            assert(check(*this,"self@="));
            return *this;
        }

        natural & natural:: operator=(const utype u)
        {
            assert(check(*this,"self@="));
            natural tmp(u); xch(tmp);
            assert(check(*this,"self@="));
            return *this;
        }

        const word_type * natural:: u2w(volatile utype &u, volatile size_t &n) throw()
        {
            volatile utype    tmp = 0;
            word_type        *w   = (word_type *)&tmp;
            //std::cerr << "u2w(" << u <<  ")" << std::endl;
            for(size_t i=0;i<words_per_utype;++i)
            {
                w[i] = word_type(u);
                u >>=  word_bits;
                //std::cerr << "w[" << i << "]=" << w[i] << std::endl;
            }

            n = words_per_utype;
            while(n>0)
            {
                const size_t m=n-1;
                //std::cerr << "testing w[" << n-1 << "]=" << w[n-1] << std::endl;
                if(w[m]>0)
                {
                    //std::cerr << "->n=" << n << std::endl;
                    break;
                }
                n=m;
            }
            //std::cerr << "->n=" << n << std::endl;


            u = tmp;
            return (word_type *) &u;
        }

    }

}

#include "y/code/utils.hpp"

namespace upsylon
{

    namespace yap
    {

        static inline void output_top_byte(std::ostream &os, const uint8_t b)
        {
            const uint8_t hi = (b&0xf0)>>4;
            if(hi)
            {
                os << hexadecimal::lowercase_word[hi];
            }
            os << hexadecimal::lowercase_word[b&0x0f];

        }

        std::ostream & operator<<(std::ostream &os, const natural &n)
        {
            if(n.bytes<=0)
            {
                os << '0';
            }
            else
            {
                size_t i=n.bytes;
                output_top_byte(os,n.get(--i));
                while(i>0)
                {
                    os << hexadecimal::lowercase[ n.get(--i) ];
                }
            }
            return os;
        }

    }

}

#include "y/randomized/bits.hpp"

namespace upsylon
{

    namespace yap
    {
        natural:: natural(randomized::bits &ran, const size_t nbit) : Y_APN_CTOR( Y_ROUND8(nbit)>>3 )
        {
            assert(check(*this,"self@ran"));
            if(nbit)
            {
                size_t ibit = nbit-1;  // must be set to 1
                size_t imsb = ibit>>3; // at this byte

                // prepare MSB
                {
                    uint8_t &b = get(imsb);
                    ibit      &= 7;
                    b = bits_table::value[ibit];
                    while(ibit-- > 0)
                    {
                        if(ran.choice()) b |= bits_table::value[ibit];
                    }
                }

                // fill
                for(size_t i=0;i<imsb;++i)
                {
                    get(i) = ran.full<uint8_t>();
                }

                // update status
                bytes = ++imsb;
                words = words_for(bytes);
                assert(check(*this,"self@ran"));
            }
            assert(bits()==nbit);
        }

    }

}

