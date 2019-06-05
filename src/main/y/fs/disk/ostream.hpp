
//! \file

#ifndef Y_FS_DISK_OSTREAM_INCLUDED
#define Y_FS_DISK_OSTREAM_INCLUDED 1

#include "y/fs/disk/buffer.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon
{
    namespace ios
    {
        //! disk output stream
        class disk_ostream : public ostream, public disk_stream
        {
        public:
            virtual ~disk_ostream() throw(); //!< cleanup
            //! setup
            explicit disk_ostream(const writable_disk_file &src,
                                  const shared_disk_buffer &sdb);

            virtual void write( char C ); //!< ostream interface
            virtual void flush();         //!< ostream interface
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(disk_ostream);
            writable_disk_file wdf;
        };

    }

}


#endif

