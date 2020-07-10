
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
        class grooves : public collection
        {
        public:
            explicit grooves() throw();
            virtual ~grooves() throw();

            void           acquire(const size_t n); //!< size() == n
            virtual size_t size() const throw();    //!< size
            void           free() throw();          //!< free all
            void           release() throw();       //!< release all

            groove       & operator[](const size_t)       throw(); //!< in [0..size()-1]
            const groove & operator[](const size_t) const throw(); //!< in [0..size()-1]

            //! clear memory and build first items, default constructor
            template <typename T>
            void make(const size_t n)
            {
                acquire(n); assert( size()==n );
                for(size_t i=0;i<n;++i) grv[i].make<T>();
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

