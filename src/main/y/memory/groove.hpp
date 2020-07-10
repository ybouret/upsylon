//! \file

#ifndef Y_MEMORY_GROOVE_INCLUDED
#define Y_MEMORY_GROOVE_INCLUDED 1

#include "y/type/args.hpp"
#include "y/type/self-destruct.hpp"
#include <typeinfo>
#include <iosfwd>

namespace upsylon {

    namespace memory {

        //______________________________________________________________________
        //
        //
        //! handle user's memory, designed for one (small) object
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
            groove()  throw(); //!< empty groove

            //__________________________________________________________________
            //
            // management methods
            //__________________________________________________________________
            void dismiss() throw(); //!< destroy, zero, preserve bytes
            void release() throw(); //!< destroy, zero, discard  bytes
            void prepare(size_t n); //!< make n bytes

            //__________________________________________________________________
            //
            // context methods
            //__________________________________________________________________
            bool                  cpp() const throw(); //!< if C++
            const std::type_info &tid() const throw(); //!< typeid or null_type
            bool                  is0() const throw(); //!< all zero

            //! bytes >= sizeof(T)
            template <typename T> inline
            bool may_hold() const throw()
            {
                return bytes >= sizeof(T);
            }


            //__________________________________________________________________
            //
            // building methods
            //__________________________________________________________________

            //! make with default constructor
            template <typename T> inline
            void make()
            {
                prepare(sizeof(T));
                new (address) T();
                setup<T>();
            }

            //! make with copy constructor
            template <typename T> inline
            void make(const T &args)
            {
                prepare(sizeof(T));
                new (address) T(args);
                setup<T>();
            }

            //__________________________________________________________________
            //
            // access methods
            //__________________________________________________________________

            //! direct cast
            template <typename T>
            T & get() throw()
            {
                assert(may_hold<T>());
                return *static_cast<T*>(address);
            }

            //! direct cast, const
            template <typename T>
            const T & get() const throw()
            {
                assert(may_hold<T>());
                return *static_cast<const T*>(address);
            }

            //! cast with type checking
            template <typename T>
            T & as()
            {
                static const std::type_info &against = typeid(T);
                check_type(against);
                return get<T>();
            }

            //! cast with type checking, const
            template <typename T>
            const T & as() const 
            {
                static const std::type_info &against = typeid(T);
                check_type(against);
                return get<T>();
            }

            //! type checking, raise exception on error
            void check_type( const std::type_info & ) const;

            //! output, mostly for debug
            friend std::ostream & operator<<(std::ostream &, const groove&);

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

