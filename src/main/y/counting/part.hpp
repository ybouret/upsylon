//! \file

#ifndef Y_COUNTING_PART_INCLUDED
#define Y_COUNTING_PART_INCLUDED 1

#include "y/counting/counting.hpp"
#include "y/sequence/accessible.hpp"

namespace upsylon {

    struct partition
    {
        class builder : public accessible<size_t>
        {
        public:

            explicit builder(const size_t n);
            virtual ~builder() throw();

            const size_t n; //!< integer to part...
            const size_t m; //!< current size

            virtual size_t         size()             const throw(); //!< m
            virtual const size_t & operator[](size_t) const;         //! [1..m]

            bool next() throw();
            void init() throw();

            static size_t count_for(const size_t n);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(builder);

            size_t *now;  //!< 1..n   max
            size_t *tmp;  //!< 1..n   max
            size_t *arr;  //!< 0..n-1 max
            void   *wksp; 
            size_t  wlen;

            void update() throw();
        };
    };

}

#endif


