//! \file

#ifndef Y_JIVE_SCATTER_INCLUDED
#define Y_JIVE_SCATTER_INCLUDED 1

#include "y/jive/pattern.hpp"

namespace upsylon {

    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //! scatter table to dispatch and address for a given byte
        //
        //______________________________________________________________________
        class Scatter
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            //! basic linked node
            struct Node
            {
                const void *addr; //!< user's persitent address
                Node       *next; //!< next node for storage
            };

            //! basic slot to hold in-order nodes
            struct Slot
            {
                Node *head;                 //!< head
                Node *tail;                 //!< tail
                Node *push(Node *) throw(); //!< at end
                Node *pop() throw();        //!< remove head
            };

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Scatter();         //!< acquire 256 * sizeof(Slot)
            virtual ~Scatter() throw(); //!< release all

            //__________________________________________________________________
            //
            // generic function
            //__________________________________________________________________
            void record(const void *, const uint8_t);   //!< record address at code
            void record(const void *, const Leading &); //!< record address for all codes
            void remove(const void *addr) throw();      //!< remove from all slots

            

            //! display with optional callback
            void display(std::ostream&, void (*proc)(std::ostream&,const void*) ) const;

            const Slot  &operator[](const uint8_t code) const throw(); //!< get entry
            size_t       operator()(const uint8_t code) const throw(); //!< get entries

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Scatter);
            Slot   *slots;
        };



        

    }

}

#endif

