
//! \file

#ifndef Y_COUNTING_PERM_INCLUDED
#define Y_COUNTING_PERM_INCLUDED 1

#include "y/counting/counting.hpp"

namespace upsylon
{

    class permutation : public counting
    {
    public:

        explicit permutation(const size_t N);
        virtual ~permutation() throw();
        
        const size_t n;

        static size_t compute_for(const size_t N);

        virtual void start() throw();
        virtual void next()  throw();
        
        //! display
        inline friend std::ostream & operator<<(std::ostream &os, const permutation &p) { return counting::display(os,p.perm,p.n); }

    private:
        size_t  wlen;
        size_t *perm;

        Y_DISABLE_COPY_AND_ASSIGN(permutation);
    };

}

#endif

