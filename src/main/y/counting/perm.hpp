//! \file

#ifndef Y_COUNTING_PERM_INCLUDED
#define Y_COUNTING_PERM_INCLUDED 1

#include "y/counting/counting.hpp"
#include "y/sequence/addressable.hpp"
#include "y/iterate/linear.hpp"

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

        //! size=space=n
        virtual size_t size() const throw();
        
        //! and access, C-style
        const size_t & operator()(const size_t indx) const throw();

        //! show state
        virtual std::ostream & show(std::ostream &os) const;
        
        //______________________________________________________________________
        //
        // internal counting
        //______________________________________________________________________
        static yap::natural compute(const size_t N, const with_ap_t &); //!< multi-precision count
        static size_t       compute(const size_t N, const with_sz_t &); //!< check overflow
        

        //______________________________________________________________________
        //
        // helpers
        //______________________________________________________________________

        //! check
        static void memchk(const permutation &lhs, const permutation &rhs);

        //! C-style computation
        template <typename T, typename U>
        void apply(T *target, const U *source) const throw()
        {
            assert(NULL!=target);
            assert(NULL!=source);
            const permutation &self = *this;
            for(size_t i=0;i<space;++i)
            {
                target[i] = source[ self(i) ];
            }
        }

        //! C++ style computation
        template <typename T,typename U>
        void make( addressable<T> &target, const accessible<U> &source ) const
        {
            assert( this->size() <= target.size() );
            assert( this->size() <= source.size() );
            const accessible<size_t> &self = *this;
            for(size_t i=space;i>0;--i)
            {
                target[i] = source[ self[i] ];
            }
        }

        //! check
        static bool are_equal(const permutation &lhs, const permutation &rhs) throw();

        //! re-assign compatible state
        void reload( const permutation &p ) throw();

        typedef iterate::linear<size_t,iterate::forward> const_iterator;//!< iterator
        const_iterator begin() const throw();                           //!< begin
        const_iterator end()   const throw();                           //!< end
        
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

