//! \file

#ifndef Y_FS_DISK_BUFFER_INCLUDED
#define Y_FS_DISK_BUFFER_INCLUDED 1

#include "y/fs/disk-file.hpp"

namespace upsylon
{
    namespace ios
    {


        class disk_buffer_ : public counted_object
        {
        public:
            virtual   ~disk_buffer_() throw();
            uint8_t     *entry;
            const size_t bytes;

        protected:
            explicit  disk_buffer_() throw();

            size_t      allocated;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(disk_buffer_);
        };

        typedef arc_ptr<disk_buffer_> shared_disk_buffer;

        template <typename ALLOCATOR = memory::global >
        class disk_buffer : public disk_buffer_
        {
        public:
            inline virtual ~disk_buffer() throw()
            {
                ALLOCATOR::location().release_bytes(entry,allocated);
            }

            inline explicit disk_buffer(const size_t n) : disk_buffer_()
            {
                allocated       = n;
                entry           = ALLOCATOR::instance().acquire_bytes(allocated);
                (size_t &)bytes = n;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(disk_buffer);
        };


        class disk_stream
        {
        public:
            virtual ~disk_stream() throw();


        protected:
            offset_t           pos;
            shared_disk_buffer buf;

            explicit disk_stream( const disk_file &df, const shared_disk_buffer &sdb );
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(disk_stream);
        };

        
    }
}

#endif

