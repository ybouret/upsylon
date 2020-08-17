
#include "y/yap/natural.hpp"
#include "y/os/endian.hpp"

namespace upsylon {

    namespace yap {

        natural:: ~natural() throw()
        {
            release(word,count,width,shift);
            bytes=0;
            words=0;
        }

        size_t natural:: words_for(const size_t bytes) throw()
        {
            return Y_ROUND_LN2(word_exp2,bytes);
        }

        number::word_type * natural:: acquire(size_t &count, size_t &width, size_t &shift)
        {
            static memory_allocator &mgr = instance();
            return mgr.acquire_field<word_type>(count,width,shift);
        }

        void natural:: release(word_type *w, size_t &count, size_t &width, size_t &shift) throw()
        {
            static memory_allocator &mgr = instance();
            mgr.release_field(w,count,width,shift);
        }

        natural::natural() : Y_APN_CTOR(0) {}
        
        natural:: natural(const size_t num_bytes, const as_capacity_t &) : Y_APN_CTOR(num_bytes) {}

        natural:: natural(const natural &other) :
        number(),
        bytes(other.bytes),
        words(other.words),
        count( words_for(bytes) ), width(0), shift(0),
        word( acquire(count,width,shift) )
        {
            for(size_t w=words;w>0;)
            {
                --w;
                word[w] = other.word[w];
            }
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


