//! \file

#ifndef Y_FS_DISK_FILE_INCLUDED
#define Y_FS_DISK_FILE_INCLUDED 1

#include "y/fs/local/file.hpp"
#include "y/ptr/arc.hpp"
#include "y/core/chainable.hpp"
#include "y/hashing/function.hpp"

namespace upsylon
{
    
    
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! base class to have a shared disk file pointer
        //
        //______________________________________________________________________
        class on_disk : public counted_object, public local_file
        {
        public:
            typedef arc_ptr<on_disk> pointer; //!< shared on_disk pointer

            virtual ~on_disk() throw();                                     //!< destructor
            explicit on_disk( const string &filename, const unsigned mode); //!< open a local file
            explicit on_disk( const char   *filename, const unsigned mode); //!< open wrapper
            explicit on_disk( const cstdin_t  &);                           //!< open wrapper
            explicit on_disk( const cstderr_t &);                           //!< open wrapper
            explicit on_disk( const cstdout_t &);                           //!< open wrapper
            concurrent::mutex access;                                       //!< mutex

        private:
            Y_DISABLE_COPY_AND_ASSIGN(on_disk);
            friend class disk_file;
        };


        //______________________________________________________________________
        //
        //
        //! disk file pointer
        //
        //______________________________________________________________________
        class disk_file : public on_disk::pointer
        {
        public:
            virtual ~disk_file() throw();                //!< destructor
        protected:                                       //|
            disk_file( const disk_file & ) throw();      //!< shared copy
            disk_file(const string  &, const unsigned ); //!< setup
            disk_file(const char    *, const unsigned ); //!< setup wrapper
            disk_file(const cstdin_t &);                 //!< setup wrapper
            disk_file(const cstdout_t &);                //!< setup wrapper
            disk_file(const cstderr_t &);                //!< setup wrapper

            descriptor::type & fd();                     //!< get file descriptor


        private:
            Y_DISABLE_ASSIGN(disk_file);
        public:
            //! direct copy using BUFSIZ memory block
            static void copy( const string &target, const string &source, const bool append);

            //! direct loading using BUFSIZ memory block
            static size_t load( chainable<char> &target, const string &source );

            //! wrapper
            static size_t load( chainable<uint8_t> &target, const string &source );

            
           
        };

        //______________________________________________________________________
        //
        //
        //! random access disk file
        //
        //______________________________________________________________________
        class rw_disk_file : public disk_file
        {
        public:
            virtual ~rw_disk_file() throw();                //!< destructor
            explicit rw_disk_file(const string &filename);  //!< open
            explicit rw_disk_file(const char   *filename);  //!< open
            rw_disk_file( const rw_disk_file & ) throw();   //!< shared copy

            size_t get(void       *data, const size_t size); //!< get at most size bytes
            size_t put(const void *data, const size_t size); //!< put at most size bytes

        private:
            Y_DISABLE_ASSIGN(rw_disk_file);
        };

        //______________________________________________________________________
        //
        //
        //! read only disk file
        //
        //______________________________________________________________________
        class readable_disk_file : public disk_file
        {
        public:
            virtual ~readable_disk_file() throw();                  //!< destructor
            explicit readable_disk_file( const string &filename );  //!< open
            explicit readable_disk_file( const char   *filename );  //!< open wrapper
            explicit readable_disk_file( const cstdin_t & );        //!< open cstdin
            readable_disk_file(const readable_disk_file &) throw(); //!< shared copy
            readable_disk_file(const rw_disk_file &) throw();       //!< shared copy
            size_t get( void *data, const size_t size );            //!< get at most size bytes

        private:
            Y_DISABLE_ASSIGN(readable_disk_file);
            static void run_hash( hashing::function &, readable_disk_file &_);
            
        public:
            //! update hashing filename/cstdin
            template <typename CHANNEL> static inline
            void hash_with(hashing::function &H, const CHANNEL &channel)
            {
                readable_disk_file src(channel);
                run_hash(H,src);
            }

            //! single full hashing of filename/cstdin
            template <typename CHANNEL> static inline
            digest md( hashing::function &H, const CHANNEL &channel)
            {
                H.set();
                hash_with(H,channel);
                return H.md();
            }
            
            
        };

        //______________________________________________________________________
        //
        //
        //! write only disk file
        //
        //______________________________________________________________________
        class writable_disk_file : public disk_file
        {
        public:
            virtual ~writable_disk_file() throw();                                    //!< destructor
            explicit writable_disk_file( const string &filename, const bool append ); //!< open to write, truncate if not append
            explicit writable_disk_file( const char   *filename, const bool append ); //!< open wrapper
            explicit writable_disk_file( const cstdout_t &);                          //!< open stdout
            explicit writable_disk_file( const cstderr_t &);                          //!< open stderr
            writable_disk_file(const writable_disk_file &other) throw();              //!< shared copy
            writable_disk_file(const rw_disk_file       &other) throw();              //!< shared copy
            size_t put(const void *data, const size_t size);                          //!< put at most size bytes
            void   put_all(const void *data, const size_t size);                      //!< put all or error
        private:
            Y_DISABLE_ASSIGN(writable_disk_file);
        };

        
    }
}

#endif

