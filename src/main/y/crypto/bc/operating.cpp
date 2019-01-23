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

        static inline
        size_t check_sizes(const block_cipher::pointer &lhs, const block_cipher::pointer &rhs )
        {
            const size_t lsz = lhs->size();
            const size_t rsz = rhs->size();
            if(lsz!=rsz)
            {
                throw exception("crypto::operating sizes mismatch: (#%s=%u) != (#%s=%u)", *(lhs->name), unsigned(lsz), *(rhs->name), unsigned(rsz) );
            }
            return lsz;
        }

        operating:: operating( const block_cipher::pointer &e, const block_cipher::pointer &d) :
        encrypter(e),
        decrypter(d),
        block_size( check_sizes(encrypter,decrypter) ),
        last_plain( block_size ),
        last_crypt( block_size ),
        workspace(  block_size )
        {
            // checking
            encrypter->crypt(last_crypt.rw(),last_plain.ro());
            decrypter->crypt(workspace.rw(), last_crypt.ro());
            if( !(workspace==last_plain) )
            {
                throw exception("crypto::operating mismatching '%s' and '%s'", *(encrypter->name), *(decrypter->name) );
            }
        }

        void operating:: encrypt_block(void *output, const void *input) throw()
        {
            assert(output); assert(input);
            memcpy( last_plain.rw(), input,  block_size );
            encrypter->crypt(output,input);
            memcpy( last_crypt.rw(), output, block_size );
        }

        void operating:: decrypt_block(void *output, const void *input) throw()
        {
            assert(output); assert(input);
            memcpy( last_crypt.rw(), input,  block_size );
            decrypter->crypt(output,input);
            memcpy( last_plain.rw(), output, block_size );
        }



    }
}
