//! \fi;e

#ifndef Y_CRYPTO_RSA_KEY_FILE_INCLUDED
#define Y_CRYPTO_RSA_KEY_FILE_INCLUDED 1

#include "y/yap/natural.hpp"
#include "y/associative/hash/map.hpp"
namespace upsylon
{
    namespace crytpo
    {

        typedef hash_map<string,apn> named_keys;

        class key_file : public named_keys
        {
        public:
            explicit key_file(ios::istream &);
            virtual ~key_file() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(key_file);
        };

    }
}

#endif

