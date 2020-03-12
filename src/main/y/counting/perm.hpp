
//! \file

#ifndef Y_COUNTING_PERM_INCLUDED
#define Y_COUNTING_PERM_INCLUDED 1

#include "y/counting/counting.hpp"
#include "y/sequence/accessible.hpp"
#include "y/mpl/natural.hpp"

namespace upsylon {
    
    //! permutations
    class permutation : public counting, public accessible<size_t>
    {
    public:
        typedef size_t     type;          //!< alias for atom::
        typedef type       mutable_type;  //!< alias for atom::
        typedef const type const_type;    //!< alias for atom::

        explicit permutation(const size_t N);  //!< setup
        virtual ~permutation() throw();        //!< cleanup
        permutation(const permutation &other); //!< copy
        
        const size_t  n; //!< n

        static mpn    compute(const size_t N, const with_mp_t &); //!< multi-precision count
        static size_t compute(const size_t N, const with_sz_t &); //!< check overflow
        
        
        //! display
        inline friend std::ostream & operator<<(std::ostream &os, const permutation &p)
        { return counting::display(os,p.perm,p.n); }
        
        //! check
        static void memchk(const permutation &lhs, const permutation &rhs);
        
        //! access, C++ style
        virtual const size_t & operator[](const size_t indx) const throw();
        
        //! access, C-style
        const size_t & operator()(const size_t indx) const throw();

        //! size=n
        virtual size_t size() const throw();  

        //! C-style computation
        template <typename T>
        void apply(T *target, const T *source) const throw()
        {
            assert(NULL!=target);
            assert(NULL!=source);
            const permutation &self = *this;
            for(size_t i=0;i<n;++i)
            {
                target[i] = source[ self(i) ];
            }
        }


    private:
        const size_t nm1;
        size_t       wlen;
        size_t      *perm; //!< [1:n]
        size_t      *addr; //!< [0:n-1]
        
        Y_DISABLE_ASSIGN(permutation);
        virtual void start_() throw(); //!< index=1, perm=[1..n]
        virtual void next_()  throw(); //!< find next one
    };
    
    
    
}

#endif

