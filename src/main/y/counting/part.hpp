//! \file

#ifndef Y_COUNTING_PART_INCLUDED
#define Y_COUNTING_PART_INCLUDED 1

#include "y/counting/perm.hpp"
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

            bool                   next() throw(); //!< compute next partition
            void                   init() throw(); //!< restart with singulet

            virtual size_t         size()             const throw();            //!< m
            virtual const size_t & operator[](size_t) const;                    //! [1..m]
            static        size_t   count_for(const size_t n);                   //!< compute the size

            size_t        permutations() const;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(builder);

            size_t *now;  //!< 1..n   max
            size_t *tmp;  //!< 1..n   max
            void   *wksp; 
            size_t  wlen;

        };

        class shaker : public accessible<size_t>
        {
        public:
            explicit shaker( const builder &config );
            virtual ~shaker() throw();
            const size_t m;

            virtual size_t         size()             const throw();            //!< m
            virtual const size_t & operator[](size_t) const;                    //! [1..m]

            bool next() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(shaker);
            permutation   perm;
            size_t        wlen;
            size_t       *indx;
            size_t       *jndx;
            const size_t *data;
        };


    };

}

#endif


