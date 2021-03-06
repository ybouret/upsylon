//! \file

#ifndef Y_COUNTING_COMB_INCLUDED
#define Y_COUNTING_COMB_INCLUDED 1

#include "y/counting/counting.hpp"
#include "y/sequence/addressable.hpp"
#include "y/iterate/linear.hpp"

namespace upsylon
{
    //--------------------------------------------------------------------------
    //
    //! C/C++ style combination
    //
    //--------------------------------------------------------------------------
    class combination : public counting, public accessible<size_t>
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
        // C++
        //______________________________________________________________________

        //! initialize state, space=K
        explicit combination(const size_t N, const size_t K);

        //! cleanup
        virtual ~combination() throw();

        //! copy full state
        combination(const combination &other);

        //! show state
        virtual std::ostream & show(std::ostream &os) const;
        
        //______________________________________________________________________
        //
        // accessible interface
        //______________________________________________________________________
        //! access C++ style
        virtual const size_t & operator[](const size_t j) const throw();

        //! size=k=space
        virtual size_t size() const throw();

        //! and access C-style
        const size_t & operator()(const size_t j) const throw();

        //______________________________________________________________________
        //
        // internal counting
        //______________________________________________________________________
        //! compute the number of combinations using mpn
        static yap::natural compute(const size_t N, const size_t K, const with_ap_t &);

        //! compute the integral number of combinations, with size check
        static size_t       compute(const size_t N, const size_t K, const with_sz_t &);
        
        //! check consistency
        static void memchk(const combination &lhs, const combination &rhs);

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const size_t n;       //!< maximum value, k=space

        //! C-style apply
        /**
         \param target [0..space-1]
         \param source [0..n-1]
         */
        template <typename T, typename U> inline
        void apply(T *target, const U *source)
        {
            assert(target);
            assert(source);
            for(size_t i=0;i<space;++i) target[i] = source[ base[i] ];
        }

        //! C++ style computation
        template <typename T,typename U>
        void make( addressable<T> &target, const accessible<U> &source ) const
        {
            assert( space <= target.size() );
            assert( n     <= source.size() );
            for(size_t i=space;i>0;--i)
            {
                target[i] = source[ comb[i] ];
            }
        }
        
        typedef iterate::linear<size_t,iterate::forward> const_iterator; //!< iterator
        const_iterator begin() const throw();                            //!< begin
        const_iterator end()   const throw();                            //!< end

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

