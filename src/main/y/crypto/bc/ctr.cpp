

#include "y/crypto/bc/ctr.hpp"


namespace upsylon
{
    namespace crypto
    {

        ctr::context:: context() throw() :
        counter(NULL),
        crypted(NULL)
        {
        }

        ctr::context:: ~context() throw()
        {

        }

        void ctr::context:: kill() throw()
        {
            crypted = NULL;
            counter = NULL;
        }

        void ctr::context:: make( const size_t block_size )
        {
            assert(block_size>0);
            if( counter.is_empty() ||  block_size!=counter->size)
            {
                kill();
                try
                {
                    counter = new digest(block_size);
                    crypted = new digest(block_size);
                }
                catch(...)
                {
                    kill();
                    throw;
                }
            }
        }

        void ctr::context:: start( ciphers &c, const digest &IV )
        {
            const size_t block_size = c.block_size;
            make(block_size);
            *counter = IV;
            c.encrypter->crypt(crypted->rw(),counter->ro());
            c.last_plain.ldz();
            c.last_crypt = *crypted;
        }

        void  ctr::context:: next( ciphers &c ) throw()
        {
            assert(counter.is_valid());
            assert(crypted.is_valid());
            assert(counter->size==crypted->size);
            
            counter->_inc();
            c.encrypter->crypt(crypted->rw(),counter->ro());

        }

        
    }

}

namespace upsylon
{
    namespace crypto
    {
        const char ctr::name[] = "CTR";

        ctr::encrypter:: encrypter() : context(), operating()
        {
        }

        ctr::encrypter:: ~encrypter() throw()
        {
        }

        void ctr:: encrypter:: write_block( ciphers &c, void *output, const void *input ) throw()
        {
            next(c);
            c.load_plain(input);
            c.last_crypt._xor(c.last_plain,*crypted);
            c.send_crypt(output);
        }

        void ctr::encrypter:: initialize( ciphers &c, const digest &IV )
        {
            start(c,IV);
        }

    }
}


namespace upsylon
{
    namespace crypto
    {
        ctr::decrypter:: decrypter() : context(), operating()
        {
        }

        ctr::decrypter:: ~decrypter() throw()
        {
        }

        void ctr:: decrypter:: write_block( ciphers &c, void *output, const void *input ) throw()
        {
            next(c);
            c.load_crypt(input);
            c.last_plain._xor(c.last_crypt,*crypted);
            c.send_plain(output);
        }

        void ctr:: decrypter:: initialize( ciphers &c, const digest &IV )
        {
            start(c,IV);
        }

    }
}
