//! \file
#ifndef Y_PARALLEL_INCLUDED
#define Y_PARALLEL_INCLUDED 1

#include "y/object.hpp"
#include "y/parops.hpp"
#include "y/memory/shack.hpp"

namespace upsylon
{

    typedef memory::shack parallel_cache; //!< local cache is necessary

    //__________________________________________________________________________
    //
    //
    //! information and operation for parallelism
    //
    //__________________________________________________________________________
    class parallel : public parallel_cache
    {
    public:
        //______________________________________________________________________
        //
        //! group of parallel contexts
        //______________________________________________________________________
        class group : public addressable<parallel>
        {
        public:
            //! cleanup
            virtual ~group() throw() {}

            //! make ONE type per context
            template <typename T> inline
            void make()
            {
                for(size_t i=size();i>0;--i) (*this)[i].make<T>();
            }

            //! make n types per context
            template <typename T> inline
            void make(const size_t n)
            {
                for(size_t i=size();i>0;--i) (*this)[i].make<T>(n);
            }


            //! access default first item
            template <typename T>
            T & _(const size_t indx) throw()
            {
                addressable<parallel> &self = *this;
                return                 self[indx]._<T>();
            }

            //! access default first const item
            template <typename T>
            const T & _(const size_t indx) const throw()
            {
                const accessible<parallel> &self = *this;
                return self[indx]._<T>();
            }

            //! access for multiple index
            template <typename T>
            T & _(const size_t i, const size_t j) throw()
            {
                addressable<parallel> &self = *this;
                return                 self[i]._<T>(j);
            }

            //! access for multiple index, const
            template <typename T>
            const T & _(const size_t i, const size_t j) const throw()
            {
                const accessible<parallel> &self = *this;
                return                      self[i]._<T>(j);
            }

            double efficiency(const double speed_up) const throw()
            {
                const accessible<parallel> &self = *this;
                return self[1].efficiency(speed_up);
            }

        protected:
            //! setup
            explicit group() throw()
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(group);
        };

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
        void split(T &user_length, T &user_offset) const throw()
        {
            parops::split_any(user_length,user_offset,size,rank);
        }

        //! get system ticks, thread-safe
        uint64_t     ticks(lockable &) const throw();
        
        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        const size_t   size;     //!< the family size
        const size_t   rank;     //!< 0..size-1
        const size_t   indx;     //!< 1..size
        const char     label[8]; //!< size.rank or "too big"
        

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________



    private:
        Y_DISABLE_COPY_AND_ASSIGN(parallel);
        void __format() throw();
    };
}

#endif

