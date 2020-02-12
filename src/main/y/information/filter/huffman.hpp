//! \file
#ifndef Y_INFORMATION_HUFFMAN_INCLUDED
#define Y_INFORMATION_HUFFMAN_INCLUDED 1

#include "y/information/qbits.hpp"
#include "y/information/filter/alphabet.hpp"
#include "y/ordered/priority-queue.hpp"

namespace upsylon {

    namespace information {


        class Huffman : public Alphabet
        {
        public:
            typedef core::priority_queue<Node,Node::FrequencyComparator> PrioQ;
            static const size_t Nodes = 2*Codes+1;

            explicit Huffman();
            virtual ~Huffman() throw();


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Huffman);
            PrioQ pq;

        };

    }

    
}

#endif
