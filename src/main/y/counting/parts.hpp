//! \file

#ifndef Y_COUNTING_PARTS_INCLUDED
#define Y_COUNTING_PARTS_INCLUDED 1

#include "y/counting/part.hpp"

namespace upsylon {

    //--------------------------------------------------------------------------
    //
    //! use the counting API for integer partitions
    //
    //--------------------------------------------------------------------------
    class integer_partitions : public counting
    {
    public:
        explicit integer_partitions(const size_t n);    //!< initialize
        virtual ~integer_partitions() throw();          //!< cleanup
        integer_partitions(const integer_partitions &); //!< copy
        
        //! output internal state
        virtual std::ostream & show(std::ostream &) const;

        //! get internal state
        const accessible<size_t> & operator*() const throw();
        
    private:
        Y_DISABLE_ASSIGN(integer_partitions);
        integer_partition part;
        virtual void onBoot() throw();
        virtual void onNext();
    };
}

#endif
