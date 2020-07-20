//! \file

#ifndef Y_COUNTING_PERM_INCLUDED
#define Y_COUNTING_PERM_INCLUDED 1

#include "y/counting/counting.hpp"
#include "y/sequence/addressable.hpp"

namespace upsylon {
    

    //--------------------------------------------------------------------------
    //
    //! C/C++ style permutation
    //
    //--------------------------------------------------------------------------
    class permutation : public counting, public accessible<size_t>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef size_t     type;          //!< alias
        typedef type       mutable_type;  //!< alias
        typedef const type const_type;    //!< alias

        //______________________________________________________________________
        //
        // C++ and definitions
        //______________________________________________________________________
        explicit permutation(const size_t N);  //!< setup
        virtual ~permutation() throw();        //!< cleanup
        permutation(const permutation &other); //!< copy

        //______________________________________________________________________
        //
        // accessible interface
        //______________________________________________________________________
        //! access, C++ style
        virtual const size_t & operator[](const size_t indx) const throw();

        //! size=n
        virtual size_t size() const throw();

        //! and access, C-style
        const size_t & operator()(const size_t indx) const throw();

        //! show state
        virtual std::ostream & show(std::ostream &os) const;
        
        //______________________________________________________________________
        //
        // internal counting
        //______________________________________________________________________
        static mpl::natural compute(const size_t N, const with_mp_t &); //!< multi-precision count
        static size_t       compute(const size_t N, const with_sz_t &); //!< check overflow

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const size_t  n; //!< maximum index

        //______________________________________________________________________
        //
        // helpers
        //______________________________________________________________________

        //! check
        static void memchk(const permutation &lhs, const permutation &rhs);

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

        //! C++ style computation
        template <typename T,typename U>
        void make( addressable<T> &target, const accessible<U> &source ) const
        {
            assert( this->size()  == target.size() );
            assert( target.size() == source.size() );
            const accessible<size_t> &self = *this;
            for(size_t i=n;i>0;--i)
            {
                target[i] = source[ self[i] ];
            }
        }


    private:
        size_t       wlen;
        size_t      *perm; //!< [1:n]
        size_t      *addr; //!< [0:n-1]
        
        Y_DISABLE_ASSIGN(permutation);
        virtual void onBoot() throw(); //!< index=1, perm=[1..n]
        virtual void onNext() throw(); //!< find next one
    };
    
    
    
}

#endif

