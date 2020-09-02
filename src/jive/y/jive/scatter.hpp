#ifndef Y_JIVE_SCATTER_INCLUDED
#define Y_JIVE_SCATTER_INCLUDED 1

#include "y/jive/pattern.hpp"

namespace upsylon {

    namespace Jive
    {
        class Scatter
        {
        public:
            static const size_t MaxAlpha = 256;              //!< all different byte
            static const size_t MaxLoad  = 4;                //!< load factor per slot
            static const size_t MaxSlots = MaxAlpha/MaxLoad; //!< max number of slots

            class Node : public Object
            {
            public:
                const Pattern *motif;
                Node          *next;
                Node          *prev;
                const uint8_t  code;

                explicit Node(const Motif &, const uint8_t) throw();
                virtual ~Node() throw();

                typedef core::list_of_cpp<Node> List;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);
            };

            typedef Node::List Slot;

            explicit Scatter();
            virtual ~Scatter() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Scatter);
            size_t nodes; //!< total nodes
            size_t slots; //!< current slots, a power of two
            size_t smask; //!< slots-1
            size_t bytes; //!< memory bytes
            void  *entry; //!< memory entry
            Slot  *slot;  //!< at entry

        };


    }

}

#endif

