//! \file

#ifndef Y_COUNTING_PART_INCLUDED
#define Y_COUNTING_PART_INCLUDED 1

#include "y/counting/counting.hpp"
#include "y/sequence/accessible.hpp"
#include "y/mpl/natural.hpp"

namespace upsylon {



    //! iterate on possible decomposition of a given integer
    class integer_partition : public accessible<size_t>
    {
    public:
        
        explicit integer_partition(const size_t n);
        virtual ~integer_partition() throw();

        const size_t n; //!< integer to part...
        const size_t m; //!< current size

        bool                   build_next() throw(); //!< compute next partition
        void                   initialize() throw(); //!< restart with singulet
        size_t                 outcomes()   throw(); //!< number of possible outcomes

        mpn                    configs( const counting::with_mp_t &) const;   //!< number of configurations for this outcome
        size_t                 configs( const counting::with_sz_t &) const;   //!< with overflow check

        virtual size_t         size()             const throw(); //!< m
        virtual const size_t & operator[](size_t) const;         //!< [1..m]
        static        size_t   outcomes(const size_t n);         //!< compute number of outcomes for a given size

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
        Y_DISABLE_COPY_AND_ASSIGN(integer_partition);

        size_t *now;  //!< 1..n   max
        size_t *tmp;  //!< 1..n   max
        size_t  wlen; //!< private bytes

    };




}

#endif


