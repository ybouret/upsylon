//! \file

#ifndef Y_FS_ON_DISK_INCLUDED
#define Y_FS_ON_DISK_INCLUDED 1

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
            concurrent::mutex        access;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(on_disk);
        };

        class disk_file : public on_disk::pointer
        {
        public:


        private:
            Y_DISABLE_ASSIGN(disk_file);
        };
    }
}

#endif

