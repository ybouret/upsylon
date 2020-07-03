//! \file
#ifndef Y_MEMORY_XSLOT_INCLUDED
#define Y_MEMORY_XSLOT_INCLUDED 1

#include "y/memory/global.hpp"
#include "y/type/traits.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace memory
    {
        //----------------------------------------------------------------------
        //
        //! base class for xslot, a smart slot for data
        //
        //----------------------------------------------------------------------
        class xslot_type
        {
        public:
            //__________________________________________________________________
            //
            // public types and definitions
            //__________________________________________________________________
            typedef void (*kill_proc)(void *); //!< alias

            //__________________________________________________________________
            //
            // public methods
            //__________________________________________________________________
            virtual ~xslot_type() throw();             //! destruct
            void     free() throw();                   //!< kill/clear data
            bool     is_cplusplus() const throw();     //!< if kill!=NULL
            void     swap_with(xslot_type &) throw();  //!< no-throw swap
            bool     is_zeroed() const throw();        //!< check zeroed memory

            //__________________________________________________________________
            //
            // public members
            //__________________________________________________________________
            const size_t    size; //!< available bytes

            //! memory capacity helper
            template <typename T> inline
            bool has_bytes_for() const throw() { return size >= sizeof(T); }

            //! memory capacity helper
            template <typename T> inline
            bool has_bytes_for(const size_t n) const throw() { return size >= n*sizeof(T); }

        protected:
            //__________________________________________________________________
            //
            // protected members
            //__________________________________________________________________
            void           *data; //!< first available byte
            kill_proc       kill; //!< cleaning content

            //__________________________________________________________________
            //
            // protected methods
            //__________________________________________________________________
            explicit xslot_type() throw(); //!< initialize
            void     would_kill() throw(); //!< check and kill if necessary

            //! template function to kill data content
            template <typename U> static inline
            void kill_as( void *addr ) throw()
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
                kill = kill_as<typename type_traits<T>::mutable_type>;
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
                ALLOCATOR &mgr    = ALLOCATOR::instance();
                aliasing::_(size) = n;
                data = mgr.acquire( aliasing::_(size) );
            }

            //! build without argument
            template <typename T>
            inline T & build()
            {
                acquire( sizeof(T) );
                new (data) typename type_traits<T>::mutable_type();
                set_as<T>();
                return get<T>();
            }

            //! copy constructor
            template <typename T>
            inline T & build_from(typename type_traits<T>::parameter_type arg)
            {
                acquire( sizeof(T) );
                new (data) typename type_traits<T>::mutable_type(arg);
                set_as<T>();
                return get<T>();
            }

            //! build with one argument
            template <typename T,typename U> inline
            T & build( typename type_traits<U>::parameter_type u )
            {
                acquire( sizeof(T) );
                new (data) typename type_traits<T>::mutable_type(u);
                set_as<T>();
                return get<T>();
            }


            //! free and release memory
            inline void release() throw()
            {
                if(data)
                {
                    assert( ALLOCATOR::exists() );
                    would_kill();
                    ALLOCATOR::location().release(data,aliasing::_(size));
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

            //! kill content but keep memory if enough bytes for one object
            template <typename T> inline void acquire_for() { acquire( sizeof(T) ); }

            //! kill content but keep memory if enough bytes for n object(s)
            template <typename T> inline void acquire_for(const size_t n) { acquire( n*sizeof(T) ); }

            //! get POD
            template <typename T> inline T & get() throw()
            {
                assert(size>=sizeof(T));
                return *static_cast<T*>(data);
            }

            //! get POD, const
            template <typename T> inline const T & get() const throw()
            {
                assert(size>=sizeof(T));
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

