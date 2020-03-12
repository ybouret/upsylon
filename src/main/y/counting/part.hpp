//! \file

#ifndef Y_COUNTING_PART_INCLUDED
#define Y_COUNTING_PART_INCLUDED 1

#include "y/counting/counting.hpp"
#include "y/sequence/accessible.hpp"

namespace upsylon {


    //! iterate on possible decomposition of a given integer
    class integer_partition : public accessible<size_t>
    {
    public:

        explicit integer_partition(const size_t n);
        virtual ~integer_partition() throw();

        const size_t n; //!< integer to part...
        const size_t m; //!< current size

        bool                   build_next() throw(); //!< compute next partition
        void                   initialize() throw(); //!< restart with singulet
        size_t                 outcomes()   throw(); //!< number of possibilities

        virtual size_t         size()             const throw(); //!< m
        virtual const size_t & operator[](size_t) const;         //!< [1..m]
        static        size_t   count_for(const size_t n);        //!< compute the size

    private:
        Y_DISABLE_COPY_AND_ASSIGN(integer_partition);

        size_t *now;  //!< 1..n   max
        size_t *tmp;  //!< 1..n   max
        size_t  wlen; //!< private bytes

    };




}

#endif


