//! \file

#ifndef Y_COUNTING_COMB_INCLUDED
#define Y_COUNTING_COMB_INCLUDED 1

#include "y/counting/counting.hpp"
#include "y/sequence/accessible.hpp"

namespace upsylon
{
    //! C++ style permutation
    class combination : public counting, public accessible<size_t>
    {
    public:
        typedef size_t     type;          //!< alias for atom::
        typedef type       mutable_type;  //!< alias for atom::
        typedef const type const_type;    //!< alias for atom::

        //! initialize state
        explicit combination(const size_t N, const size_t K);

        //! cleanup
        virtual ~combination() throw();

        //! copy full state
        combination(const combination &other);

        const size_t n;       //!< maximum value
        const size_t k;       //!< number of chosen values
     

        //! compute the number of combinations using mpn
        static mpl::natural compute(const size_t N, const size_t K, const with_mp_t &);

        //! compute the integral number of combinations, with size check
        static size_t      compute(const size_t N, const size_t K, const with_sz_t &);
        
        //! access C++ style
        virtual const size_t & operator[](const size_t j) const throw();

        //! access C  style
        const size_t & operator()(const size_t j) const throw();

        //! display
        inline friend std::ostream & operator<<(std::ostream &os, const combination &c) { return counting::display(os,c.comb,c.k); }

        //! check consistency
        static void memchk(const combination &lhs, const combination &rhs);

        //! size=k
        virtual size_t size() const throw();

    private:
        const size_t nmk;  //!< n-k
        size_t       wlen; //!< internal bytes
        size_t      *comb; //! [1..k]
        size_t      *base; //! [0..k-1]
        Y_DISABLE_ASSIGN(combination);
        virtual void onBoot()  throw(); //!< initialize first combination
        virtual void onNext()  throw(); //!< update to next one

    };
}

#endif

