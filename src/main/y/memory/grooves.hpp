//! \file

#ifndef Y_MEMORY_GROOVES_INCLUDED
#define Y_MEMORY_GROOVES_INCLUDED 1

#include "y/memory/groove.hpp"
#include "y/sequence/addressable.hpp"

namespace upsylon {

    namespace memory {

        //______________________________________________________________________
        //
        //
        //! flexible cache multiple grooves
        //
        //______________________________________________________________________
        class grooves : public addressable<groove>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! setup with collection memory model
            explicit grooves(const storage::model = storage::shared) throw();

            //! cleanup
            virtual ~grooves() throw();

            //__________________________________________________________________
            //
            // accessible interface
            //__________________________________________________________________
            virtual size_t size() const throw();                            //!< current number of grooves
            virtual groove       & operator[](const size_t) throw();        //!< [1..size()]
            virtual const groove & operator[](const size_t) const throw();  //!< [1..size()], const

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void make(const size_t n);                //!< ensure exactly n clean grooves
            void free() throw();                      //!< free content, keep clean grooves
            void release() throw();                   //!< release all memory
            void update(const storage::model);        //!< update with a valid model on a releases state
            groove       & operator*()       throw(); //!< get first groove for size()>0
            const groove & operator*() const throw(); //!< get first groove for size()>0
            const groove & self() const throw();      //!< for information

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________

        private:
            Y_DISABLE_COPY_AND_ASSIGN(grooves);
            groove              *shift;
            groove               front;
        public:
            const storage::model model; //!< current memory model
        };

    }

}

#endif

