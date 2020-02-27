//! \file
#ifndef Y_INFORMATION_MTF_INCLUDED
#define Y_INFORMATION_MTF_INCLUDED 1

#include "y/information/modulation.hpp"
#include "y/core/list.hpp"

namespace upsylon {

    namespace Information {

        //! move to front base class
        class MoveToFront : public Modulation
        {
        public:
            virtual        ~MoveToFront() throw(); //!< cleanup
            virtual void    reset()          throw(); //!< rebuild state

            void duplicate( const MoveToFront &m ) throw(); //!< duplicate ordering

            //! basic node for list
            struct node_t
            {
                node_t *next; //!< for list
                node_t *prev; //!< for list
                uint8_t data; //!< initial node index
            };
            
        protected:
            explicit MoveToFront() throw();     //!< setup

            core::list_of<node_t>  list_;      //!< fast list
            node_t                 node_[256]; //!< built-in nodes

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MoveToFront);
            void build() throw();
        };

        //! move to front encoder
        class MoveToFrontEncoder : public MoveToFront
        {
        public:
            static const char   CLID[];                       //!< class identifier
            virtual const char *name() const         throw(); //!< CLID
            virtual            ~MoveToFrontEncoder() throw(); //!< cleanup
            explicit            MoveToFrontEncoder() throw(); //!< setup
            virtual uint8_t     fetch(const uint8_t) throw(); //!< encode
            virtual Modulation *clone()              const;   //!< a new mtf with same nodes ordering

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MoveToFrontEncoder);
        };

        //! move to front decoder
        class MoveToFrontDecoder : public MoveToFront
        {
        public:
            static const char   CLID[];                       //!< class identifier
            virtual const char *name() const         throw(); //!< CLID
            virtual            ~MoveToFrontDecoder() throw(); //!< cleanup
            explicit            MoveToFrontDecoder() throw(); //!< setup
            virtual uint8_t     fetch(const uint8_t) throw(); //!< decode
            virtual Modulation *clone()              const;   //!< a new mtf with same nodes ordering

        private:
            Y_DISABLE_COPY_AND_ASSIGN(MoveToFrontDecoder);
        };

    }

}


#endif
