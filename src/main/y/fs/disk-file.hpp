//! \file

#ifndef Y_FS_DISK_FILE_INCLUDED
#define Y_FS_DISK_FILE_INCLUDED 1

#include "y/fs/local-file.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace ios
    {
        //! base class to have a shared disk file pointer
        class on_disk : public counted_object, public local_file
        {
        public:
            typedef arc_ptr<on_disk> pointer;

            virtual ~on_disk() throw(); //!< destructor
            explicit on_disk( const string &filename, const unsigned mode);
            explicit on_disk( const char   *filename, const unsigned mode);
            concurrent::mutex        access;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(on_disk);
            friend class disk_file;
        };

        //! disk file pointer
        class disk_file : public on_disk::pointer
        {
        public:
            virtual ~disk_file() throw();                //!< destructor
        protected:                                       //|
            disk_file( const disk_file & ) throw();      //!< shared copy
            disk_file(const string  &, const unsigned ); //!< setup
            disk_file(const char    *, const unsigned ); //!< setup wrapper
            descriptor::type & fd();

        private:
            Y_DISABLE_ASSIGN(disk_file);
        };

        //! read only disk file
        class readable_disk_file : public disk_file
        {
        public:
            virtual ~readable_disk_file() throw();                  //!< destructor
            explicit readable_disk_file( const string &filename );  //!< open
            explicit readable_disk_file( const char   *filename );  //!< open wrapper
            readable_disk_file(const readable_disk_file &) throw(); //!< shared copy

            size_t get( void *data, const size_t size );

        private:
            Y_DISABLE_ASSIGN(readable_disk_file);
        };

        //! write only disk file
        class writable_disk_file : public disk_file
        {
        public:
            virtual ~writable_disk_file() throw();                                    //!< destructor
            explicit writable_disk_file( const string &filename, const bool append ); //!< open to write, truncate if not append
            explicit writable_disk_file( const char   *filename, const bool append ); //!< open wrapper

            size_t put(const void *data, const size_t size);

        private:
            Y_DISABLE_ASSIGN(writable_disk_file);
        };

        
    }
}

#endif

