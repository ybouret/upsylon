//! \file
#ifndef Y_COUNTING_INCLUDED
#define Y_COUNTING_INCLUDED 1

#include "y/os/platform.hpp"
#include <iosfwd>

namespace upsylon
{

    namespace mpl
    {
        class natural; //!< forward declaration
    }

    //! base class for counting objects
    class counting
    {
    public:

        const size_t index; //!< index in 1..count
        const size_t count; //!< number of possible objects

        //----------------------------------------------------------------------
        // virtual interface
        //----------------------------------------------------------------------
        virtual void start()    throw() = 0; //!< initialize index and first objectg
        virtual void next()     throw() = 0; //!< update index and next object
        virtual     ~counting() throw();     //!< cleanup

        //----------------------------------------------------------------------
        // non-virtual interface
        //----------------------------------------------------------------------
        bool valid() const throw(); //!< index<=count after a start()

    protected:
        static size_t *acquire_( size_t &bytes );                      //!< acquire a count of bytes=workspace*sizeof(size_t)
        static void    release_(size_t *&wksp, size_t &bytes) throw(); //!< release workspace
        explicit       counting(const size_t   n) throw();             //!< setup count=n, index=0
        explicit       counting(const counting &) throw();             //!< copy
        static bool    mpn2count(size_t &sz, const mpl::natural &n);   //!< convert with size checking

        //! display arr[1..num]
        static std::ostream &display( std::ostream &, const size_t *arr, const size_t num );

    private:
        Y_DISABLE_ASSIGN(counting);
    };

};

#endif

