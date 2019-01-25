

#include "y/crypto/bc/cbc.hpp"



namespace upsylon
{
    namespace crypto
    {
        const char cbc::name[] = "CBC";

        cbc::encrypter:: encrypter() : operating()
        {
        }

        cbc::encrypter:: ~encrypter() throw()
        {
        }

        void cbc:: encrypter:: initialize( ciphers &c, const digest &IV ) throw()
        {
            c.initialize_crypt(IV);
        }

        void cbc:: encrypter:: write_block( ciphers &c, void *output, const void *input ) throw()
        {
            c.load_plain(input);                         // last_plain = P_i
            c.temporary._xor(c.last_plain,c.last_crypt); // temporary  = P_i ^ C_{i-1}
            c.encrypter->crypt(output,c.temporary.ro()); // output     = E(P_i^C_{i-1})
            c.load_crypt(output);
        }


    }
}


namespace upsylon
{
    namespace crypto
    {
        cbc::decrypter:: decrypter() : operating()
        {
        }

        cbc::decrypter:: ~decrypter() throw()
        {
        }

        void cbc:: decrypter:: initialize( ciphers &c, const digest &IV ) throw()
        {
            c.initialize_crypt(IV);
        }


        void cbc:: decrypter:: write_block( ciphers &c, void *output, const void *input ) throw()
        {
            c.load_temp_(input);                                     // tmp = input
            c.decrypter->crypt(c.last_plain.rw(),c.temporary.ro());  // P_i = D( tmp )
            c.last_plain._xor(c.last_crypt);                         // P_i = D( tmp ) ^ C_{i-1}
            c.last_crypt = c.temporary;                              // C_{i-1} = tmp (=input)
            c.send_plain(output);
        }


    }
}
