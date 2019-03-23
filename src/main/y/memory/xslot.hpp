//! \file
#ifndef Y_MEMORY_XSLOT_INCLUDED
#define Y_MEMORY_XSLOT_INCLUDED 1

#include "y/memory/global.hpp"
#include "y/type/traits.hpp"

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
            void swap_with( xslot_type &other ) throw(); //!< no-throw swap
            
        protected:
            void           *data;
            kill_proc       kill;

            explicit xslot_type() throw();
            void     would_kill() throw();

            template <typename U>
            static inline void kill_for( void *addr ) throw()
            {
                static_cast<U*>(addr)->~U();
            }

            template <typename T>
            void set_as() throw()
            {
                assert(0==kill);
                assert(data!=0);
                assert(size>=sizeof(T));
                kill=kill_for<typename type_traits<T>::mutable_type>;
            }

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

            //! build without argument
            template <typename T>
            inline T & build()
            {
                acquire( sizeof(T) );
                new (data) typename type_traits<T>::mutable_type();
                set_as<T>();
                return as<T>();
            }

            template <typename T>
            inline T & build_from(typename type_traits<T>::parameter_type arg)
            {
                acquire( sizeof(T) );
                new (data) typename type_traits<T>::mutable_type(arg);
                set_as<T>();
                return as<T>();
            }

            //! build with one argument
            template <typename T,typename U> inline
            T & build( typename type_traits<U>::parameter_type u )
            {
                acquire( sizeof(T) );
                new (data) typename type_traits<T>::mutable_type(u);
                set_as<T>();
                return as<T>();
            }

            //! type conversion
            template <typename T>
            T & as() throw()
            {
                assert(is_cplusplus()); assert(size>=sizeof(T));
                return *static_cast<T*>(data);
            }

            //! type conversion, const
            template <typename T>
            const T & as() const throw()
            {
                assert(is_cplusplus()); assert(size>=sizeof(T));
                return *static_cast<const T*>(data);
            }

            //! free and release memory
            inline void release() throw()
            {
                if(data)
                {
                    assert( ALLOCATOR::exists() );
                    would_kill();
                    ALLOCATOR::location().release(data,(size_t&)size);
                }
            }

            //! kill content but keep memory if enough
            inline void acquire(const size_t n)
            {
                if(n>size) {
                    xslot temp(n); swap_with(temp);
                } else {
                    free();
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
            template <typename T> inline T & get(const size_t indx) throw()
            {
                assert(size>=(indx+1)*sizeof(T));
                return *(static_cast<T*>(data)+indx);
            }

            //! get POD, assuming multiple, const
            template <typename T> inline const T & get(const size_t indx) const throw()
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

