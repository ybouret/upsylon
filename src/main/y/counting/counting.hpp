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

    //--------------------------------------------------------------------------
    //
    //! base class for counting objects
    /** upon creation counting objects should be ready to go!
     */
    //
    //--------------------------------------------------------------------------
    class counting
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef type2type<mpl::natural> with_mp_t; //!< to use multiple precision
        typedef type2type<size_t>       with_sz_t; //!< to use integral size
        static  const with_mp_t         with_mp;   //!< alias
        static  const with_sz_t         with_sz;   //!< alias


        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        virtual              ~counting()    throw();          //!< cleanup
        bool                  good()  const throw();          //!< index<=count after a boot()
        void                  boot();                         //!< set index to 1 and call onBoot()
        void                  next();                         //!< update index and call   onNext() if valid
        virtual std::ostream &show(std::ostream &) const = 0; //!< display C++ style

        //! boot and forward, return remaining objects
        /**
         the index is updated is there is some work to do,
         otherwise it remains at 1
         */
        size_t                boot(const size_t global_size,
                                   const size_t global_rank);


        //! make a full loop
        void unwind();
        

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const size_t index; //!< index in 1..count
        const size_t count; //!< number of possible configurations
        const size_t space; //!< dimensions of a configuration

        //______________________________________________________________________
        //
        // static methods/helpers
        //______________________________________________________________________
        static size_t               chkdim(const size_t);                          //!< check dimension>0
        static memory::allocator &  mem_instance();                                //!< internal allocator instance
        static memory::allocator &  mem_location() throw();                        //!< internal allocator location
        static size_t *             acquire_(size_t &bytes);                       //!< acquire a count of bytes=workspace*sizeof(size_t)
        static void                 release_(size_t *&wksp,size_t &bytes) throw(); //!< release workspace

        //! generic display
        inline friend std::ostream &operator<<(std::ostream &os, const counting &cnt)
        {
            return cnt.show(os);
        }

        //! fill a compatible config (a.k.a addressable<type>)
        template <typename COUNTING, typename CONFIG>
        static inline void fill_space( CONFIG &config, COUNTING &self ) throw()
        {
            for(size_t i=self.space;i>0;--i)
            {
                config[i] = self[i];
            }
        }

        //! fill a compatible frame (a.k.a matrix<type>, field2D<type>)
        template <typename COUNTING, typename FRAME>
        static inline void fill_frame( FRAME &frame, COUNTING &self ) throw()
        {
            for( self.boot(); self.good(); self.next() )
            {
                fill_space( frame[self.index], self );
            }
        }
        
    protected:
        explicit       counting(const size_t   n, const size_t s) throw();   //!< setup count=n, index=0, space=s
        explicit       counting(const counting &) throw();                   //!< copy

        //! display arr[1..num]
        static std::ostream &display(std::ostream &, const size_t *arr, const size_t num );

        //! check status
        static
        void check_contents(const char     *id,
                            const counting &lhs, const void *l,
                            const counting &rhs, const void *r,
                            const size_t    length);
        
    private:
        Y_DISABLE_ASSIGN(counting);
        virtual void          onBoot() = 0; //!< initialize first objects
        virtual void          onNext() = 0; //!< update next objects, index<=count



    };

};

#endif

