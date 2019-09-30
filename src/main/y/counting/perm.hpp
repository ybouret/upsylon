
//! \file

#ifndef Y_COUNTING_PERM_INCLUDED
#define Y_COUNTING_PERM_INCLUDED 1

#include "y/counting/counting.hpp"

namespace upsylon
{

    //! permutations
    class permutation : public counting
    {
    public:
        explicit permutation(const size_t N);  //!< setup
        virtual ~permutation() throw();        //!< cleanup
        permutation(const permutation &other); //!< copy

        const size_t  n; //!< n!
        static size_t compute_for(const size_t N); //!< check overflow


        //! display
        inline friend std::ostream & operator<<(std::ostream &os, const permutation &p)
        { return counting::display(os,p.perm,p.n); }

        //! check
        static void memchk(const permutation &lhs, const permutation &rhs);
        
        //! get
        inline size_t operator[]( const size_t indx ) const throw()
        {
            assert(indx>0); assert(indx<=n);
            const size_t ans = perm[indx];
            assert(ans>=1); assert(ans<=n);
            return perm[indx];
        }
        
        
    private:
        const size_t nm1;
        size_t       wlen;
    protected:
        size_t      *perm;
    private:
        Y_DISABLE_ASSIGN(permutation);
        virtual void start_() throw(); //!< index=1, perm=[1..n]
        virtual void next_()  throw(); //!< find next one
    };

}

#endif

