//! \file
#ifndef Y_MEMORY_CSLOT_INCLUDED
#define Y_MEMORY_CSLOT_INCLUDED 1

#include "y/memory/global.hpp"
#include "y/type/args.hpp"
#include <cstring>

namespace upsylon
{
    namespace memory
    {
        template <typename ALLOCATOR = global>
        class cslot
        {
        public:
            const size_t size;

            //! constructor, be sure to activate allocator
            inline explicit cslot(const size_t n=0) : size(n), data( ALLOCATOR::instance().acquire( (size_t &)size )) { }

            //! destructor, release memory
            inline virtual ~cslot() throw() { release(); }

            //! acquire or clear memory memory
            inline void acquire(const size_t n)
            {
                if(n>size)
                {
                    release();
                    data = ALLOCATOR::instance().acquire( ((size_t&)size = n ) );
                }
                else
                {
                    memset(data,0,n);
                }
            }

            //! release memory
            inline void release() throw() { ALLOCATOR::location().release(data,(size_t&)size); }

            //! local access
            template <typename T>
            inline T & get() throw()
            { assert(size>=sizeof(T)); assert(data); return *static_cast<T*>(data); }

            //! local access, const
            template <typename T>
            inline T & get() const throw()
            { assert(size>=sizeof(T)); assert(data); return *static_cast<const T*>(data); }

            template <typename T>
            inline T & get(const size_t i) throw()
            {
                assert(size>=sizeof(T)*(i+1));
                assert(data);
                return *(static_cast<T*>(data)+i);
            }

            template <typename T>
            inline const T & get(const size_t i) const throw()
            {
                assert(size>=sizeof(T)*(i+1));
                assert(data);
                return *(static_cast<const T*>(data)+i);
            }

            //! initialize with on the fly allocation
            template <typename T> inline
            void set( const typename type_traits<T>::parameter_type value )
            {
                acquire(sizeof(T));
                get<T>() = value;
            }


        private:
            void        *data;
            Y_DISABLE_COPY_AND_ASSIGN(cslot);



        };
    }
}

#endif

