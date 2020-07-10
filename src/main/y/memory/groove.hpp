//! \file

#ifndef Y_MEMORY_GROOVE_INCLUDED
#define Y_MEMORY_GROOVE_INCLUDED 1

#include "y/type/args.hpp"
#include "y/core/inode.hpp"
#include "y/object.hpp"
#include <typeinfo>

namespace upsylon {

    namespace memory {

        //______________________________________________________________________
        //
        //
        //! handle user's memory
        //
        //______________________________________________________________________
        class groove : public object, public inode<groove>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~groove() throw(); //!< release all
            explicit groove() throw(); //!< empty groove
            explicit groove(size_t n); //!< flat  groove, for POD

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void free() throw();    //!< destroy and zero, keep allocated
            void release() throw(); //!< free and release memory
            void acquire(size_t n); //!< free and check enough memory

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t    bytes; //!< maximum usable bytes

        protected:
            void            *address;          //!< address of data
            void           (*destroy)(void *); //!< destructor if C++
            std::type_info  *type_id;          //!< type_id    if C++


        private:
            Y_DISABLE_COPY_AND_ASSIGN(groove);
        };



    }

}

#endif

