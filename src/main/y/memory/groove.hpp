//! \file

#ifndef Y_MEMORY_GROOVE_INCLUDED
#define Y_MEMORY_GROOVE_INCLUDED 1

#include "y/type/args.hpp"
#include "y/object.hpp"
#include "y/type/self-destruct.hpp"
#include <typeinfo>

namespace upsylon {

    namespace memory {

        //______________________________________________________________________
        //
        //
        //! handle user's memory, designed for one object
        //
        //______________________________________________________________________
        class groove
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            ~groove() throw(); //!< release all
            groove() throw(); //!< empty groove

            //__________________________________________________________________
            //
            // management methods
            //__________________________________________________________________
            void free()    throw(); //!< destroy and zero, keep allocated
            void release() throw(); //!< destroy and zero, free allocated
            void acquire(size_t n); //!< destroy and zero, check memory

            bool                  cpp() const throw(); //!< if C++
            const std::type_info &tid() const throw(); //!< typeid or null_type


            //__________________________________________________________________
            //
            // building methods
            //__________________________________________________________________

            //! make with default constructor
            template <typename T> inline
            void make()
            {
                acquire(sizeof(T));
                new (address) T();
                setup<T>();
            }

            //! make with copy constructor
            template <typename T> inline
            void make(const T &args)
            {
                acquire(sizeof(T));
                new (address) T(args);
                setup<T>();
            }

            template <typename T>
            T & get() throw()
            {
                assert(bytes>=sizeof(T));
                return *static_cast<T*>(address);
            }

            template <typename T>
            const T & get() const throw()
            {
                assert(bytes>=sizeof(T));
                return *static_cast<const T*>(address);
            }

            template <typename T>
            T & as() throw()
            {
                assert(type_id);
                assert(*type_id==typeid(T));
                return get<T>();
            }

            template <typename T>
            const T & as() const throw()
            {
                assert(type_id);
                assert(*type_id==typeid(T));
                return get<T>();
            }


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t    bytes; //!< maximum usable bytes

        protected:
            void                  *address;          //!< address of data
            void                 (*destroy)(void *); //!< destructor if C++
            const std::type_info  *type_id;          //!< type_id    if C++


        private:
            Y_DISABLE_COPY_AND_ASSIGN(groove);
            template <typename T> static inline
            void __destroy(void *addr) throw()
            {
                assert(addr);
                self_destruct( *static_cast<T*>(addr) );
            }

            template <typename T> void setup() throw()
            {
                assert(0==destroy);
                assert(0==type_id);
                type_id = &typeid(T);
                destroy = __destroy<T>;
            }
        };

       


    }

}

#endif

