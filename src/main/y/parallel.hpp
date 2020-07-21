//! \file
#ifndef Y_PARALLEL_INCLUDED
#define Y_PARALLEL_INCLUDED 1

#include "y/object.hpp"
#include "y/parops.hpp"
#include "y/memory/groove.hpp"
#include "y/memory/marker.hpp"

namespace upsylon
{


    //__________________________________________________________________________
    //
    //
    //! information and operation for parallelism
    //
    //__________________________________________________________________________
    class parallel
    {
    public:
        //______________________________________________________________________
        //
        // types and definition
        //______________________________________________________________________
        typedef memory::groove cache_type; //!< cache for extra data
        static const memory::storage::model L2model = memory::storage::shared; //!< inner cache

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~parallel() throw();                                 //!< destructor
        explicit parallel() throw();                                 //!< sequential setup
        explicit parallel(const size_t sz, const size_t rk) throw(); //!< parallel

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        double efficiency(const double speed_up) const; //!< compute efficiency, two significative figures

        //! get the work portion according to rank/size
        template <typename T> inline
        void split(T &length, T &offset) const throw()
        {
            parops::split_any(length,offset,size,rank);
        }

        //! get system ticks, thread-safe
        uint64_t     ticks(lockable &) const throw();

        //______________________________________________________________________
        //
        // L1 cache
        //______________________________________________________________________

        //! forwarding call to L1 cache
        template <typename T> T &       get()       throw() { return L1.get<T>(); }

        //! forwarding call to L1 cache
        template <typename T> T &       get(const size_t indx)       throw() { return L1.get<T>(indx); }

        //! forwarding call to L1 cache, const
        template <typename T> const T & get() const throw() { return L1.get<T>(); }

        //! forwarding call to L1 cache, const
        template <typename T> const T & get(const size_t indx) const throw() { return L1.get<T>(indx); }


        //______________________________________________________________________
        //
        // L2 cache
        //______________________________________________________________________


        //! forwarding call to L2 cache
        template <typename T> T &       aux()       throw() { return L2.get<T>(); }

        //! forwarding call to L2 cache
        template <typename T> T &       aux(const size_t indx)       throw() { return L2.get<T>(indx); }

        //! forwarding call to L2 cache, const
        template <typename T> const T & aux() const throw() { return L2.get<T>(); }

        //! forwarding call to L2 cache, const
        template <typename T> const T & aux(const size_t indx) const throw() { return L2.get<T>(indx); }

        //! L2 solo with default constructor
        template <typename T> inline
        T & mk()
        {
            return L2.solo<T>(L2model);
        }

        //! L2 solo with args
        template <typename T, typename U> inline
        T & mk( typename type_traits<U>::parameter_type args)
        {
            return L2.solo<T,U>(L2model,args);
        }



        
        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        cache_type     L1;       //!< primary   cache for outer computations
        cache_type     L2;       //!< secondary cache for inner computations
        const size_t   size;     //!< the family size
        const size_t   rank;     //!< 0..size-1
        const size_t   indx;     //!< 1..size
        const char     label[8]; //!< size.rank or "too big"
        

    private:
        Y_DISABLE_COPY_AND_ASSIGN(parallel);
        void __format() throw();
    };
}

#endif

