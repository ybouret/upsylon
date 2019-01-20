//! \file
#ifndef Y_HASHING_HASH_MAC_INCLUDED
#define Y_HASHING_HASH_MAC_INCLUDED 1

#include "y/hashing/digest.hpp"
#include "y/hashing/function.hpp"

namespace upsylon
{
    namespace hashing
    {

        class hash_mac
        {
        public:
            explicit hash_mac( function &H, const memory::ro_buffer &key );

            virtual ~hash_mac() throw();

            void set( function &H ) throw();
            void get( function &H, void *output, size_t outlen ) throw();

        private:
            const     size_t L;
            const     size_t B;
            digest    key_;
            digest    tmp_;
            digest    end_;
            Y_DISABLE_COPY_AND_ASSIGN(hash_mac);
        };

        template <typename HFN>
        class hmac : public function
        {
        public:
            explicit hmac( const memory::ro_buffer &key ) :
            function( HFN:: __length, HFN:: __window ),
            hfn_(),
            mac_( hfn_, key ),
            name_( string("HMAC-") + hfn_.name() )
            {
            }

            virtual ~hmac() throw() {}

            virtual void set() throw() { mac_.set( hfn_ ); }
            virtual void run( const void *buffer, size_t buflen ) throw() { hfn_.run(buffer,buflen); }
            virtual void get( void *output, size_t outlen ) throw() { mac_.get( hfn_, output, outlen); }
            virtual const char *name() const throw() { return *name_; }

        private:
            HFN          hfn_;
            hash_mac     mac_;
            const string name_;
            Y_DISABLE_COPY_AND_ASSIGN(hmac);
        };

    }


}

#endif
