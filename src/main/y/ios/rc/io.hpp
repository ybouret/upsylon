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
            typedef      uint32_t      uid_t;
            static const uid_t         MAGIC = Y_FOURCC('Y', '_' , 'R', 'C');
            typedef      uint64_t      len_t; //!< data length type
            typedef      uint64_t      key_t; //!< hashing key type
            typedef      hashing::sha1 hfn_t; //!< hashing function alias
            typedef      uint16_t      ssz_t; //!< type for identifier length
            
            class io
            {
            public:
                const string   name;

                virtual ~io() throw();

                ssz_t id_be_length( const string &identifier) const;

            protected:
                explicit io(const string &filename);

                bool put_all( descriptor::type handle, const void *data, const size_t size);
                
                template <typename U> inline
                bool put_all( descriptor::type handle, const U value )
                {
                    const U be_value = swap_be(value);
                    return put_all( handle, &be_value, sizeof(be_value) );
                }
                
                
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
                void append_file( const string &datafile, const char   *identifier );
                void append_file( const char   *datafile, const char   *identifier );

                void append_data( const void   *data, const size_t size, const string &identifier);
                void append_data( const void   *data, const size_t size, const char   *identifier);

                void append_data( const char   *text, const string &identifier);
                void append_data( const char   *text, const char   *identifier);

                void append_data( const memory::ro_buffer &, const string &identifier);
                void append_data( const memory::ro_buffer &, const char   *identifier);

                void mark();                  //!< emit  MAGIC, update total
                void emit( const string &id); //!< emit length+id
                void sign();                  //!< emit key

                void finish(); //!< emit total+magic

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

