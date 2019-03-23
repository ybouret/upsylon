//! \file
#ifndef Y_MEMORY_XSLOT_INCLUDED
#define Y_MEMORY_XSLOT_INCLUDED 1

#include "y/memory/global.hpp"
#include "y/type/traits.hpp"

namespace upsylon
{
    namespace memory
    {
        //! base class for xslot
        class xslot_type
        {
        public:
            typedef void (*kill_proc)(void *); //!< alias

            virtual ~xslot_type() throw();             //! destruct
            void     free() throw();                   //!< kill/clear data
            bool     is_cplusplus() const throw();     //!< if kill!=NULL
            void     swap_with( xslot_type &) throw(); //!< no-throw swap

            const size_t    size; //!< available bytes

        protected:
            void           *data; //!< first available byte
            kill_proc       kill; //!< cleaning content

            explicit xslot_type() throw(); //!< initialize
            void     would_kill() throw(); //!< check and kill

            //! template function to kill data content
            template <typename U>
            static inline void kill_for( void *addr ) throw()
            {
                static_cast<U*>(addr)->~U();
            }

            //! register kill function
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

        //! slot of any data
        template <typename ALLOCATOR=global>
        class xslot : public xslot_type
        {
        public:
            inline virtual ~xslot() throw() { release();}      //!< release content
            inline explicit xslot() throw() : xslot_type() {}  //!< initialize

            //! initialize with data
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

            //! copy constructor
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

            //! kill content but keep memory if enough bytes
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

