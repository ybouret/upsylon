#ifndef Y_BLOCK_CIPHER_INCLUDED
#define Y_BLOCK_CIPHER_INCLUDED 1

#include "y/hashing/digest.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon
{

    namespace crypto
    {

        //! block cipher interface
        class block_cipher : public counted_object
        {
        public:
            typedef intr_ptr<string,block_cipher> pointer; //!< alias

            //! to label standard action
            enum action
            {
                encrypting, //!< standard forward way
                decrypting  //!< standard reverse way
            };
            static const char *action_text( const action a) throw();

            //! destructor
            virtual ~block_cipher() throw();

            const string          name;
            virtual size_t        size() const throw() = 0; //!< block size
            virtual void          crypt( void *output, const void *input ) throw() = 0; //!< blocks of this->size() !

            const string & key() const throw(); //!< for associative

        protected:
            explicit block_cipher(const char   *id);
            explicit block_cipher(const string &id);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(block_cipher);
        };

    }

}

#endif
