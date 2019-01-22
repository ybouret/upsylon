#include "block-cipher.hpp"

namespace upsylon {
    
    namespace crypto {
        
        block_cipher:: ~block_cipher() throw()
        {
        }
        
        block_cipher:: block_cipher(const string &id) : name(id)
        {
        }

        block_cipher:: block_cipher(const char *id) : name(id)
        {
        }

        const char * block_cipher:: action_text( const action a) throw()
        {
            switch(a)
            {
                case encrypting: return "ENC";
                case decrypting: return "DEC";
            }
            return "UNK";
        }

        const string & block_cipher:: key() const throw()
        {
            return name;
        }

    }
    
}
