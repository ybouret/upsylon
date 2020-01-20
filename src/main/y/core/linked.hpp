#ifndef Y_CORE_LINKED_INCLUDED
#define Y_CORE_LINKED_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    namespace core {

        //! base class for linked data structure
        class linked
        {
        public:
            virtual ~linked() throw();      //!< cleanup
            bool is_vacant() const throw(); //!< helper to check zero size
            bool has_nodes() const throw(); //!< helper to check positive size

            const size_t size;              //!< number of nodes/items...

        protected:
            explicit linked() throw(); //!< setup

            void increase_size() throw(); //!< ++size
            void decrease_size() throw(); //!< --size
            void force_no_size() throw(); //!< size=0

        private:
            Y_DISABLE_COPY_AND_ASSIGN(linked);
        };

    }

}


#endif

