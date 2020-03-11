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

    namespace memory
    {
        class allocator;
    }

    //! base class for counting objects
    class counting
    {
    public:

        const size_t index; //!< index in 1..count
        const size_t count; //!< number of possible objects

        virtual       ~counting()    throw();     //!< cleanup
        bool           valid() const throw();     //!< index<=count after a start()
        void           start()       throw();     //!< set index to 1 and call start_()
        void           next()        throw();     //!< update index and call next_() if valid

        //! check dimension>0
        static size_t chkdim(const size_t);
        
        //! internal allocator instance
        static memory::allocator & mem_instance();

        //! internal allocator location
        static memory::allocator & mem_location() throw();

        //! convert with size overflow checking
        static bool    mpn2count(size_t &sz, const mpl::natural &n) throw();

    protected:
        static size_t *acquire_( size_t &bytes );                            //!< acquire a count of bytes=workspace*sizeof(size_t)
        static void    release_(size_t *&wksp, size_t &bytes) throw();       //!< release workspace
        explicit       counting(const size_t   n) throw();                   //!< setup count=n, index=0
        explicit       counting(const counting &) throw();                   //!< copy

        //! display arr[1..num]
        static std::ostream &display( std::ostream &, const size_t *arr, const size_t num );




        //! check status
        static
        void check_contents(const char     *id,
                            const counting &lhs, const void *l,
                            const counting &rhs, const void *r,
                            const size_t    length);
        
    private:
        Y_DISABLE_ASSIGN(counting);
        virtual void start_()    throw() = 0; //!< initialize first objects
        virtual void next_()     throw() = 0; //!< update next objects, index<=count

    };

};

#endif

