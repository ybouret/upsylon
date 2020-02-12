#include "y/information/filter/huffman.hpp"
//#include "y/code/utils.hpp"

namespace upsylon {

    namespace information {

        Huffman:: ~Huffman() throw()
        {
        }

        Huffman:: Huffman() :
        Alphabet(Nodes,StreamMode,Codes*sizeof(void*)),
        pq((Node **)extra,Codes)
        {
        }
        
    }
}
