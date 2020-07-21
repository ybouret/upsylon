//! \file
#ifndef Y_COUNTING_SYMM_PAIR_INCLUDED
#define Y_COUNTING_SYMM_PAIR_INCLUDED 1

#include "y/counting/counting.hpp"
#include "y/sequence/accessible.hpp"

namespace upsylon {

    //--------------------------------------------------------------------------
    //
    //! symmetric pair iterating
    //
    //--------------------------------------------------------------------------
    class symm_pair : public counting, public accessible<size_t>
    {
    public:
        static const size_t extent = 2; //!< space/size
        
        //----------------------------------------------------------------------
        // C++
        //----------------------------------------------------------------------
        explicit symm_pair(const size_t w);               //!< setup with width>0
        explicit symm_pair(const symm_pair&) throw();     //!< hard copy
        virtual ~symm_pair() throw();                     //!< cleanup
        virtual  std::ostream &show(std::ostream&) const; //!< display
        
        //----------------------------------------------------------------------
        // accessible interface
        //----------------------------------------------------------------------
        virtual size_t         size()                   const throw(); //!< 2
        virtual const size_t & operator[](const size_t) const throw(); //!< access
        
        //----------------------------------------------------------------------
        // members
        //----------------------------------------------------------------------
        const size_t width; //!< symmetric pair among [width:width]
        const size_t lower; //!< 1<=lower<=upper<=n
        const size_t upper; //!< 1<=lower<=upper<=n
        
        //----------------------------------------------------------------------
        // helpers
        //----------------------------------------------------------------------
        static mpl::natural compute(const size_t N, const with_mp_t &); //!< multi-precision count
        static size_t       compute(const size_t N, const with_sz_t &); //!< check overflow

    private:
        Y_DISABLE_ASSIGN(symm_pair);
        void update() throw();

        virtual void  onBoot() throw();
        virtual void  onNext() throw();
        const size_t *getArr() const throw();
    };

}

#endif

