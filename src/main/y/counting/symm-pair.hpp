//! \file
#ifndef Y_COUNTING_SYMM_PAIR_INCLUDED
#define Y_COUNTING_SYMM_PAIR_INCLUDED 1

#include "y/counting/counting.hpp"

namespace upsylon {

    //--------------------------------------------------------------------------
    //
    //! symmetric pair iterating
    //
    //--------------------------------------------------------------------------
    class symm_pair : public counting
    {
    public:
        //----------------------------------------------------------------------
        // C++
        //----------------------------------------------------------------------
        explicit symm_pair(const size_t dimension); //!< setup with dimension>0
        virtual ~symm_pair() throw();               //!< cleanup

        //----------------------------------------------------------------------
        // members
        //----------------------------------------------------------------------
        const size_t n; //!< symmetric pair among [n:n]
        const size_t i; //!< n>=i>=j>=1
        const size_t j; //!< 1<=j<=i<=n

        //----------------------------------------------------------------------
        // helpers
        //----------------------------------------------------------------------
        static mpl::natural compute(const size_t N, const with_mp_t &); //!< multi-precision count
        static size_t       compute(const size_t N, const with_sz_t &); //!< check overflow

    private:
        Y_DISABLE_COPY_AND_ASSIGN(symm_pair);
        void update() throw();

        virtual void onBoot() throw();
        virtual void onNext() throw();
        
    };

}

#endif

