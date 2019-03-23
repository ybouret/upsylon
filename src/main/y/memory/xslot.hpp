//! \file
#ifndef Y_MEMORY_XSLOT_INCLUDED
#define Y_MEMORY_XSLOT_INCLUDED 1

#include "y/memory/global.hpp"

namespace upsylon
{
    namespace memory
    {
        class xslot_type
        {
        public:
            typedef void (*kill_proc)(void *); //!< alias

            virtual ~xslot_type() throw();
            void free() throw(); //!< kill/clear data

            const size_t    size;
            
        protected:
            void           *data;
            kill_proc       kill;

            explicit xslot_type() throw();
            void     would_kill() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(xslot_type);
        };

        template <typename ALLOCATOR=global>
        class xslot : public xslot_type
        {
        public:
            
            inline virtual ~xslot() throw() { release();}

            inline explicit xslot() throw() : xslot_type() {}

            inline explicit xslot(const size_t n) : xslot_type()
            {
                ALLOCATOR &mgr = ALLOCATOR::instance();
                (size_t &)size = n;
                data = mgr.acquire( (size_t&) size );
            }



            inline void release() throw()
            {
                if(data)
                {
                    assert( ALLOCATOR::exists() );
                    would_kill();
                    ALLOCATOR::location().release(data,(size_t&)size);
                }
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(xslot);

        };

    }
}

#endif

