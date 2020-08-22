//! \file

#ifndef Y_COUNTING_PART_INCLUDED
#define Y_COUNTING_PART_INCLUDED 1

#include "y/counting/counting.hpp"
#include "y/sequence/accessible.hpp"
//#include "y/yap/natural.hpp"

namespace upsylon {

    //--------------------------------------------------------------------------
    //
    //! iterate on possible decomposition(s) of a given integer
    //
    //--------------------------------------------------------------------------
    class integer_partition : public accessible<size_t>
    {
    public:

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        explicit integer_partition(const size_t n);   //!< setup n>0
        virtual ~integer_partition() throw();         //!< cleanup
        integer_partition(const integer_partition &); //!< copy
        //______________________________________________________________________
        //
        // accessible interface
        //______________________________________________________________________
        virtual size_t         size()             const throw(); //!< m
        virtual const size_t & operator[](size_t) const;         //!< [1..m]

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        bool                   build_next() throw(); //!< compute next partition
        void                   initialize() throw(); //!< restart with singulet
        size_t                 outcomes()   throw(); //!< number of possible outcomes

        //! number of configurations for this outcome
        /**
         The number of combinations without repetitions
         */
        yap::natural           configs( const counting::with_ap_t &) const;
        size_t                 configs( const counting::with_sz_t &) const;   //!< with overflow check

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const size_t n; //!< integer to part...
        const size_t m; //!< current outcome size


        //______________________________________________________________________
        //
        // helpers
        //______________________________________________________________________

        //! compute number of outcomes for a given size
        static size_t outcomes(const size_t n);

        //! compute the Bell's number
        template <typename T> static inline
        T Bell(const size_t value)
        {
            static const type2type<T> kind = {};
            T ans(0);
            integer_partition parts(value);
            do
            {
                ans += parts.configs( kind );
            } while( parts.build_next() );
            return ans;
        }

    private:
        Y_DISABLE_ASSIGN(integer_partition);

        size_t *now;  //!< 1..n   max
        size_t *tmp;  //!< 1..n   max
        size_t  wlen; //!< private bytes
    };

  



}

#endif


