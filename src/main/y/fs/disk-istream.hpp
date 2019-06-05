//! \file

#ifndef Y_FS_DISK_ISTREAM_INCLUDED
#define Y_FS_DISK_ISTREAM_INCLUDED 1

#include "y/fs/disk-buffer.hpp"
#include "y/ios/istream.hpp"

namespace upsylon
{
    namespace ios
    {
        //! input stream from a disk file and a buffer
        class disk_istream : public istream, public disk_stream
        {
        public:
            //! destructor
            virtual ~disk_istream() throw();

            //! constructor
            explicit disk_istream(const readable_disk_file &src,
                                  const shared_disk_buffer &sdb);

            virtual void store(char  C); //!< istream interface
            virtual bool query(char &C); //!< istream interface

        private:
            Y_DISABLE_COPY_AND_ASSIGN(disk_istream);
            readable_disk_file rdf;
            
            void push1(); //!< try to make some space to store something
            bool load();  //!< read data from file

        };


    }

}

#endif

