//! \file
#ifndef Y_IOS_RC_IO_INLCUDED
#define Y_IOS_RC_IO_INCLUDED 1

#include "y/ios/irstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/type/fourcc.hpp"
#include "y/hashing/sha1.hpp"

namespace upsylon
{
    namespace ios
    {

        //! load resources appended to the end of a file
        struct rc
        {
            typedef uint32_t      uid_t; //!< for magic marker
            typedef uint64_t      key_t; //!< for signing data
            typedef uint64_t      len_t; //!< for global offset
            typedef hashing::sha1 hfn_t; //!< hash function

            static const uint32_t magic = Y_FOURCC('<','R', 'C', '>');
            class io
            {
            public:
                const string name; //!< resource file name
                hfn_t        hash; //!< for signing
                virtual ~io() throw();

            protected:
                explicit io(const string &filename);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(io);
            };

            class writer : public io
            {
            public:
                explicit writer( const string &filename );
                virtual ~writer() throw();

                const bool closed;

                void mark(); //!< write magic
                void sign(); //!< write current key

                //! append data
                /**
                 - magic
                 - identifier
                 - size
                 - data[size]
                 - sign
                 */
                void append_data( const string &identifier, const void *data, const size_t size );

                //! wrapper
                void append_data( const char   *identifier, const void *data, const size_t size );

                //! wrapper
                void append_data( const string &identifier, const char *text);

                //! wrapper
                void append_data( const char   *identifier, const char *text);

                //! wrapper
                void append_data( const string &identifier, const memory::ro_buffer &buf );

                //! wrapper
                void append_data( const char   *identifier, const memory::ro_buffer &buf );

                //! mark end of resource, and close it
                /**
                 - sz
                 - magic
                 */
                void finalize();

            private:
                ios::ocstream fp;
                len_t         sz;
                Y_DISABLE_COPY_AND_ASSIGN(writer);
            };


            class loader
            {
            };

        };



    }
}

#endif

