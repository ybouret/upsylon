
//! \file

#ifndef Y_MEMORY_GROOVES_INCLUDED
#define Y_MEMORY_GROOVES_INCLUDED 1

#include "y/sequence/accessible.hpp"
#include "y/memory/groove.hpp"

namespace upsylon {
    
    namespace memory {

        //______________________________________________________________________
        //
        //
        //! multiple grooves
        //
        //______________________________________________________________________
        class grooves : public accessible<groove>
        {
        public:
            explicit grooves() throw();
            virtual ~grooves() throw();

            void           acquire(const size_t n); //!< size() == n of null_type
            void           release() throw();       //!< release all memory
            void           dismiss() throw();       //!< dismiss all grooves, memory is untouched

            virtual size_t         size()                   const throw(); //!< accessible: number of grooves
            virtual groove       & operator[](const size_t)       throw(); //!< accessible: [1..size()]
            virtual const groove & operator[](const size_t) const throw(); //!< accessible: [1..size()]

            //! clear memory and build n same items with default constructor
            template <typename T> inline
            void make(const size_t n)
            {
                acquire(n); assert( size()==n );
                for(size_t i=0;i<n;++i) grv[i].make<T>();
            }

            //! clear memory and make two items
            template <typename T, typename U> inline
            void make()
            {
                acquire(2); assert( size()==2 );
                grv[0].make<T>();
                grv[1].make<U>();
            }


            //! clear memory and make three items
            template <typename T, typename U, typename V> inline
            void make()
            {
                acquire(3); assert( size()==3 );
                grv[0].make<T>();
                grv[1].make<U>();
                grv[2].make<V>();
            }

            //! access with cast
            template <typename T> inline
            T & get(const size_t indx) throw()
            {
                grooves &self = *this;
                groove  &g    = self[indx];
                return g.get<T>();
            }

            //! access with cast, const
            template <typename T> inline
            const T & get(const size_t indx) const throw()
            {
                const grooves &self = *this;
                const groove  &g    = self[indx];
                return g.get<T>();
            }

            //! access with check
            template <typename T> inline
            T & as(const size_t indx) throw()
            {
                grooves &self = *this;
                groove  &g    = self[indx];
                return g.as<T>();
            }

            //! access with check, const
            template <typename T> inline
            const T & as(const size_t indx) const throw()
            {
                const grooves &self = *this;
                const groove  &g    = self[indx];
                return g.as<T>();
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(grooves);
            groove *itm;
            groove *grv;
            size_t  num;
        };
    }

}

#endif

