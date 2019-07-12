//! \file
#ifndef Y_IOS_RC_IO_INCLUDED
#define Y_IOS_RC_IO_INCLUDED 1

#include "y/ios/irstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/type/fourcc.hpp"
#include "y/hashing/sha1.hpp"
#include "y/associative/set.hpp"
#include "y/ptr/intr.hpp"
#include "y/string.hpp"

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

            static const uint32_t magic = Y_FOURCC('<','R', 'C', '>'); //!< magic header

            //! base class for write/loader
            class io
            {
            public:
                const string name;     //!< resource file name
                hfn_t        hash;     //!< for signing
                bool         verbose;  //!< to display

                virtual ~io() throw(); //!< destructor

            protected:
                explicit io(const string &filename, const bool v); //!< setup
                explicit io(const char   *filename, const bool v); //!< setup
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(io);
            };

            //! write resources at end of file
            class writer : public io
            {
            public:
                const bool closed; //!< check possible access

                explicit writer( const string &filename, const bool v=false); //!< setup
                explicit writer( const char   *filename, const bool v=false); //!< setup
                virtual ~writer() throw();                 //!< destructor


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

                //! append file content
                void append_file( const string &identifier, const string &filename );

                //! append_file wrapper
                void append_file( const char   *identfier, const char *filename );

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

            //! store information about current loaded resource
            class item : public counted_object
            {
            public:
                typedef intr_ptr<string,item> ptr; //!< alias
                typedef set<string,ptr>       db;  //!< alias

                const string   label; //!< unique label
                const offset_t start; //!< start offset
                const size_t   bytes; //!< data bytes

                virtual ~item() throw();           //!< destructor
                explicit item(const string   &id,
                              const offset_t at,
                              const size_t   len); //!< setup

                const string &key() const throw(); //!< label

                //! output
                friend std::ostream & operator<<( std::ostream &, const item &);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(item);
            };

            //! resource loader
            class loader : public io
            {
            public:
                explicit loader( const string &filename, const bool v=false ); //!< setup
                explicit loader( const char   *filename, const bool v=false ); //!< setup
                virtual ~loader() throw();                                    //!< destructor

                const  item::db & db() const throw();                 //!< get the database
                ios::irstream *load_stream( const string &id ) const; //!< create a new stream
                string         load_string( const string &id ) const; //!< create a new string


            private:
                Y_DISABLE_COPY_AND_ASSIGN(loader);
                mutable ios::irstream fp;
                item::db              items;
                void initialize();
                
            };

        };



    }
}

#endif

