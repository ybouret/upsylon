
#include "y/crypto/bc/ciphers.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace crypto
    {

        ciphers:: ~ciphers() throw()
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

        ciphers:: ciphers( const block_cipher::pointer &enc, const block_cipher::pointer &dec) :
        encrypter(enc),
        decrypter(dec),
        block_size(check_sizes(encrypter,decrypter)),
        last_plain(block_size),
        last_crypt(block_size)
        {
            encrypter->crypt( last_crypt.rw(), last_plain.ro());
            decrypter->crypt( last_plain.rw(), last_crypt.ro());
            for(size_t i=0;i<block_size;++i)
            {
                if( last_plain[i] != 0 ) throw exception("ciphers mismatch '%s'<->'%s'", *(encrypter->name), *(decrypter->name) );
            }
        }

        void ciphers:: sync_crypt() throw()
        {
            encrypter->crypt( last_crypt.rw(), last_plain.ro() );
        }

        void ciphers:: sync_plain() throw()
        {
            decrypter->crypt( last_plain.rw(), last_crypt.ro() );
        }

        void ciphers:: save_plain(const void *data) throw()
        {
            assert(data);
            memcpy(last_plain.rw(),data,block_size);
        }

        void ciphers:: save_crypt(const void *data) throw()
        {
            assert(data);
            memcpy(last_crypt.rw(),data,block_size);
        }


        void ciphers:: flush( void *output, const void *input, const size_t length ) throw()
        {
            assert( !(0==output&&length>0) );
            assert( !(0==input &&length>0) );
            assert(length<block_size);

            build_flush_mask(length);
            const uint8_t *p = static_cast<const uint8_t *>(input);
            uint8_t       *q = static_cast<uint8_t       *>(output);

            for(size_t i=0;i<length;++i)
            {
                q[i] = p[i] ^ last_crypt[i];
            }
        }


        void ciphers:: build_flush_mask(const size_t length) throw()
        {
            assert(length<block_size);
            for(size_t i=length;i<block_size;++i)
            {
                last_plain[i] = last_crypt[i];
            }
            encrypter->crypt(last_crypt.rw(), last_plain.ro() );
        }

        void ciphers:: intialize() throw()
        {
            last_plain.ldz();
            sync_crypt();
        }

        void ciphers:: initialize(const digest &IV) throw()
        {
            last_plain = IV;
            sync_crypt();
        }


    }
}
