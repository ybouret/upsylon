//! \file

#ifndef Y_FS_DISK_ISTREAM_INCLUDED
#define Y_FS_DISK_ISTREAM_INCLUDED 1

#include "y/fs/disk-buffer.hpp"
#include "y/ios/istream.hpp"

namespace upsylon
{
    namespace ios
    {
        class disk_istream : public istream
        {
        public:
            virtual ~disk_istream() throw();
            explicit disk_istream(const readable_disk_file &src,
                                  const shared_disk_buffer &sdb);

            size_t available() const throw();
            virtual void store(char  C);
            virtual bool query(char &C);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(disk_istream);
            readable_disk_file rdf;
            ios::offset_t      pos;
            shared_disk_buffer buf;
            uint8_t           *curr;
            uint8_t           *next;
            const uint8_t     *last;

            void defrag() throw();
            void push1();

            bool load();

        };


    }

}

#endif

