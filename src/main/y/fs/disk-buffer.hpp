//! \file

#ifndef Y_FS_DISK_BUFFER_INCLUDED
#define Y_FS_DISK_BUFFER_INCLUDED 1

#include "y/fs/disk-file.hpp"

namespace upsylon
{
    namespace ios
    {

        //! interface for a disk buffer
        class disk_buffer_ : public counted_object
        {
        public:
            virtual      ~disk_buffer_() throw();  //!< cleanup
            uint8_t      *entry;                   //!< first byte address
            const size_t  bytes;                   //!< effective size
        protected:                                 //|
            explicit      disk_buffer_() throw();  //!< constructor
            size_t        allocated;               //!< from allocator
            static size_t check(size_t) throw();   //!< check positive size
        private:
            Y_DISABLE_COPY_AND_ASSIGN(disk_buffer_);
        };

        //! dynamic shared disk buffer
        typedef arc_ptr<disk_buffer_> shared_disk_buffer;

        //! memory dependent implementation
        template <typename ALLOCATOR = memory::global >
        class disk_buffer : public disk_buffer_
        {
        public:
            //! release memory
            inline virtual ~disk_buffer() throw()
            {
                ALLOCATOR::location().release_bytes(entry,allocated);
            }

            //! acquire memory
            inline explicit disk_buffer(const size_t n) : disk_buffer_()
            {
                allocated       = check(n);
                entry           = ALLOCATOR::instance().acquire_bytes(allocated);
                (size_t &)bytes = n;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(disk_buffer);
        };


        //! base class for a disk strean
        class disk_stream
        {
        public:
            virtual ~disk_stream() throw(); //!< destructor
            size_t   used() const throw();  //!< used bytes of data
            void     defrag() throw();      //!< move used byte to entry
        protected:                          //|
            offset_t           pos;         //!< current position
            shared_disk_buffer buf;         //!< shared disk buffer
            uint8_t           *curr;        //!< current address
            uint8_t           *next;        //!< end of data address
            const uint8_t     *last;        //!< first invalid address

            //! setup from a disk file and a buffer
            explicit disk_stream(const disk_file          &df,
                                 const shared_disk_buffer &sdb );
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(disk_stream);
        };

        
    }
}

#endif

