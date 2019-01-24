#include "y/crypto/bc/operating.hpp"
#include "y/exception.hpp"
#include <cstring>

namespace upsylon
{
    namespace crypto
    {
        operating:: ~operating() throw()
        {
        }


        operating:: operating( ) throw()
        {

        }

        void operating:: flush_block(ciphers    &c,
                                     void       *output,
                                     const void *input,
                                     const size_t length ) throw()
        {
            c.flush(output,input,length);
        }

        void operating:: write( ciphers &c, void *output, const void *input, size_t length ) throw()
        {
            assert( !(0==output&&length>0) );
            assert( !(0==input &&length>0) );

            const size_t   blk = c.block_size;
            const uint8_t *src = static_cast<const uint8_t *>(input);
            uint8_t       *tgt = static_cast<uint8_t       *>(output);
            while(length>=blk)
            {
                write_block(c,tgt,src);
                tgt    += blk;
                src    += blk;
                length -= blk;
            }
            flush_block(c,tgt,src,length);
        }

    }
}
