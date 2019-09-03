//! \file

#ifndef Y_COUNTING_COMB_INCLUDED
#define Y_COUNTING_COMB_INCLUDED 1

#include "y/counting/counting.hpp"

namespace upsylon
{
    //! C++ style permutation
    class combination : public counting
    {
    public:
        //! initialize state
        explicit combination(const size_t N, const size_t K);

        //! cleanup
        virtual ~combination() throw();

        //! copy full state
        combination(const combination &other);

        const size_t n;       //!< maximum value
        const size_t k;       //!< number of chosen values
     

        //! compute the number of combinations using mpn
        static size_t compute_for(const size_t N, const size_t K);

        virtual void start()  throw(); //!< initialize
        virtual void next()   throw(); //!< update to next one

        //! access C++ style
        const size_t & operator[](const size_t j) const throw();

        //! display
        inline friend std::ostream & operator<<(std::ostream &os, const combination &c) { return counting::display(os,c.comb,c.k); }

        //! check consistency
        static void memchk(const combination &lhs, const combination &rhs);

    private:
        const size_t nmk;  //!< n-k
        size_t       wlen; //!< internal bytes
        size_t      *comb; //! [1..k]
        Y_DISABLE_ASSIGN(combination);
        
    };
}

#endif

