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

    class counting
    {
    public:
        virtual ~counting() throw();



        const size_t index;
        const size_t count;

        bool valid() const throw(); //!< index<=count after a start()

        virtual void start() throw() = 0;
        virtual void next()  throw() = 0;
        
    protected:
        static size_t *acquire_( size_t &bytes );                      //!< acquire a count of bytes=workspace*sizeof(size_t)
        static void    release_(size_t *&wksp, size_t &bytes) throw(); //!< release workspace

        explicit counting(const size_t   n) throw(); //!< setup count=n, index=0
        explicit counting(const counting &) throw(); //!< copy

        static bool mpn2count(size_t &sz, const mpl::natural &n);

        static std::ostream &display( std::ostream &, const size_t *arr, const size_t num );

    private:
        Y_DISABLE_ASSIGN(counting);
    };

};

#endif

