//! \file
#ifndef Y_IOS_RC_IO_INLCUDED
#define Y_IOS_RC_IO_INCLUDED 1

#include "y/ios/irstream.hpp"
#include "y/type/fourcc.hpp"
#include "y/hashing/sha1.hpp"

namespace upsylon
{
    namespace ios
    {

        //! load resources appended to the end of a file
        struct rc
        {
            static const uint32_t      MAGIC = Y_FOURCC('Y', '_' , 'R', 'C');
            typedef      uint64_t      len_t;
            typedef      uint64_t      key_t;
            typedef      hashing::sha1 hfn_t;

            class io
            {
            public:
                const uint32_t be_magic;
                const string   name;

                virtual ~io() throw();

                uint16_t id_be_length( const string &identifier) const;

            protected:
                explicit io(const string &filename);

                bool put_all( descriptor::type handle, const void *data, const size_t size);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(io);
            };

            class writer : public io
            {
            public:
                //! open file and go at the end
                explicit writer( const string &filename );
                virtual ~writer() throw();

                void append_file( const string &datafile, const string &identifier );
                void append_data( const void   *data, const size_t size, const string &identifier);
                void append_data( const void   *data, const size_t size, const char   *identifier);

                void append_data( const char   *text, const string &identifier);
                void append_data( const char   *text, const char   *identifier);

                void append_data( const memory::ro_buffer &, const string &identifier);
                void append_data( const memory::ro_buffer &, const char   *identifier);

                void mark();                  //!< emit BE MAGIC, update total
                void emit( const string &id); //!< emit length+id
                void sign(const key_t  k);    //!< emit BE k,     update total

                void finish();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(writer);
                local_file fp;
                len_t      total;

            public:
                hfn_t      hasher;

            };


        };


    }
}

#endif

