//! \file

#ifndef Y_COUNTING_COMB_INCLUDED
#define Y_COUNTING_COMB_INCLUDED 1

#include "y/os/platform.hpp"
#include <iosfwd>

namespace upsylon
{
    //! C++ style permutation
    class combination
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
        const size_t index;   //!< current combination index
        const size_t count;   //!< total number of combination

        //! compute the number of combinations using mpn
        static size_t compute_for(const size_t N, const size_t K);

        void start()  throw();         //!< initialize
        bool active() const throw();   //!< more to come ?
        void next()   throw();         //!< update to next one

        //! access C++ style
        const size_t & operator[](const size_t j) const throw();

        //! display
        friend std::ostream & operator<<(std::ostream &, const combination &c);

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

