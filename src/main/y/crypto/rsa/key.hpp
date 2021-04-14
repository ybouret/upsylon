
//! \file

#ifndef Y_CRYPTO_RSA_KEY_INCLUDED
#define Y_CRYPTO_RSA_KEY_INCLUDED 1

#include "y/yap/natural.hpp"

namespace upsylon
{
    namespace crytpo
    {
        //______________________________________________________________________
        //
        //
        //! base class for rsa_key
        //
        //______________________________________________________________________
        class rsa_key : public counted, public object
        {
        public:
            virtual ~rsa_key() throw(); //!< cleanup
            

        protected:
            explicit rsa_key() throw(); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(rsa_key);
        };

    }

}

#endif
