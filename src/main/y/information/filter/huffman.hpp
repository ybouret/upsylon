//! \file
#ifndef Y_INFORMATION_HUFFMAN_INCLUDED
#define Y_INFORMATION_HUFFMAN_INCLUDED 1

#include "y/information/qbits.hpp"
#include "y/information/filter/alphabet.hpp"
#include "y/ordered/priority-queue.hpp"

namespace upsylon {

    namespace information {
        
        //! Huffman Codec
        namespace Huffman
        {
            //! common operation: build the tree and the codes
            class Context : public Alphabet
            {
            public:
                //! prioririty queue to build tree
                typedef core::priority_queue<Node,Node::Priority> PrioQ;

                virtual    ~Context()       throw(); //!< cleanup
                const Node &getRoot() const throw(); //!< get the root node

            protected:
                explicit Context(const Mode operating); //!< setup
                void     buildTree() throw();           //!< build the tree
                void     setupTree() throw();           //!< initialize alphabet and build tree
                
                Node    *root; //!< the never NULL root!

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Context);
                PrioQ  pq;
                
            };

          
         

        }


    }

    
}

#endif
