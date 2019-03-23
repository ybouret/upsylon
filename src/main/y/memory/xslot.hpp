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

            bool is_cplusplus() const throw(); //!< if kill!=NULL

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

            //! get POD
            template <typename T> inline T & get() throw()
            {
                assert(size>=sizeof(T));
                assert(!is_cplusplus()||die("use as<>"));
                return *static_cast<T*>(data);
            }

            //! get POD, const
            template <typename T> inline const T & get() const throw()
            {
                assert(size>=sizeof(T));
                assert(!is_cplusplus()||die("use as<>"));
                return *static_cast<T*>(data);
            }

            //! get POD, assuming multiple
            template <typename T> inline T & at(const size_t indx) throw()
            {
                assert(size>=(indx+1)*sizeof(T));
                return *(static_cast<T*>(data)+indx);
            }

            //! get POD, assuming multiple, const
            template <typename T> inline const T & at(const size_t indx) const throw()
            {
                assert(size>=(indx+1)*sizeof(T));
                return *(static_cast<T*>(data)+indx);
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(xslot);

        };

    }
}

#endif

