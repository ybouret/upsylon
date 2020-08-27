
//! \file
#ifndef Y_CORE_NODES_INCLUDED
#define Y_CORE_NODES_INCLUDED 1

#include "y/type/traits.hpp"

namespace upsylon {

    namespace core {

        //______________________________________________________________________
        //
        //
        //! wrapper to make a singly linked node from a class
        //
        //______________________________________________________________________
        template <typename T>
        class singly : public T
        {
        public:
            explicit singly() : T(), next(0) {} //!< initialize, default
            virtual ~singly() throw()        {} //!< cleanup
            singly  *next;                      //!< for linked

            //! for 1-arg constructors
            template <typename U> static inline
            singly *from(typename type_traits<U>::parameter_type args)
            {
                return new singly(args);
            }


        private:
            template <typename V> explicit singly(const V &args) : T(args), next(0) {}

            Y_DISABLE_COPY_AND_ASSIGN(singly);
        };

        //______________________________________________________________________
        //
        //
        //! wrapper to make a doubly linked node from a class
        //
        //______________________________________________________________________
        template <typename T>
        class doubly : public T
        {
        public:
            explicit doubly() : T(), next(0), prev(0) {} //!< initialize, default
            virtual ~doubly() throw()                 {} //!< cleanup
            doubly  *next;                               //!< for linked
            doubly  *prev;                               //!< for linked

            //! for 1-arg constructors
            template <typename U> static inline
            doubly *from(typename type_traits<U>::parameter_type args)
            {
                return new doubly(args);
            }


        private:
            template <typename V> explicit doubly(const V &args) : T(args), next(0), prev(0) {}

            Y_DISABLE_COPY_AND_ASSIGN(doubly);
        };

    }

}

#endif

