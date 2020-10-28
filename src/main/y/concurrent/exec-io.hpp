
//! \file
#ifndef Y_CONCURRENT_EXEC_IO_INCLUDED
#define Y_CONCURRENT_EXEC_IO_INCLUDED 1

#include "y/concurrent/executor.hpp"

namespace upsylon
{
    namespace concurrent
    {

        //______________________________________________________________________
        //
        //
        //! meta array of  items per context
        //
        //______________________________________________________________________
        template <typename T>
        class meta_array_of : public addressable<T>
        {
        public:
            //! cleanup
            inline virtual ~meta_array_of() throw() {}

            //! interface
            inline virtual size_t size() const throw() { return h.size(); }

        protected:
            //! setup
            inline explicit meta_array_of( executor &ex ) throw() :
            addressable<T>(),
            h(ex),
            c(h)
            {
            }

            addressable<parallel>      &h; //!< host
            const accessible<parallel> &c; //!< const host

        private:
            Y_DISABLE_COPY_AND_ASSIGN(meta_array_of);
        };

        //______________________________________________________________________
        //
        //
        //!  meta  array of  for item per context
        //
        //______________________________________________________________________
        template <typename T>
        class array_of : public meta_array_of<T>
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            Y_DECL_ARGS(T,type); //!< aliases

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            inline explicit array_of( executor &ex ) throw() : meta_array_of<T>(ex) {}

            //! cleanup
            inline virtual ~array_of() throw() {}

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________

            // const access
            inline const_type & operator[](const size_t i) const throw()
            {
                return this->c[i].template _<type>();
            }

            //! access
            inline type      & operator[](const size_t i) throw()
            {
                return this->h[i].template _<type>();
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(array_of);
        };


        //______________________________________________________________________
        //
        //
        //!  meta  array of  j^th item per context
        //
        //______________________________________________________________________
        template <typename T>
        class xarray_of : public meta_array_of<T>
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            Y_DECL_ARGS(T,type); //!< aliases

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! setup
            inline explicit xarray_of( executor &ex, const size_t indx ) throw() : meta_array_of<T>(ex), j(indx) {}

            //! cleanup
            inline virtual ~xarray_of() throw() {}

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________

            //! const access
            inline const_type & operator[](const size_t i) const throw()
            {
                return this->c[i].template _<type>(j);
            }

            //! access
            inline type      & operator[](const size_t i) throw()
            {
                return this->h[i].template _<type>(j);
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(xarray_of);
            const size_t j;
        };








    }

}

#endif
