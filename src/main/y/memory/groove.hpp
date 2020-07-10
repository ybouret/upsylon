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
        //! handle user's memory
        //
        //______________________________________________________________________
        class groove
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~groove() throw(); //!< release all
            explicit groove() throw(); //!< empty groove

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

        //______________________________________________________________________
        //
        //
        //! multiple grooves
        //
        //______________________________________________________________________
        class grooves
        {
        public:
            explicit grooves() throw();
            virtual ~grooves() throw();

            virtual size_t size() const throw();

            void           free() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(grooves);
            groove *grv;
            size_t  num;
        };


    }

}

#endif

