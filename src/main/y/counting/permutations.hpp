
//! \file

#ifndef Y_COUNTING_PERMUTATIONS_INCLUDED
#define Y_COUNTING_PERMUTATIONS_INCLUDED 1

#include "y/counting/perm.hpp"

namespace upsylon {


        class permutations
        {
        public:
            virtual ~permutations() throw();
            explicit permutations() throw();

            void setup(const size_t *group,
                       const size_t  groups);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(permutations);

        };



}

#endif

