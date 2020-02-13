#include "y/information/filter/huffman.hpp"

namespace upsylon {

    namespace information {

        Huffman::Decoder:: ~Decoder() throw()
        {
        }

        Huffman:: Decoder:: Decoder(const Mode m) :
        Context(m),
        flag(wait_for_byte),
        curr(root)
        {
        }

        void Huffman:: Decoder:: reset() throw()
        {
            setupTree();
            free_all();
            flag = wait_for_byte;
            curr = root;
        }

        void Huffman:: Decoder:: decode()
        {
        }


        void Huffman:: Decoder:: write(char C)
        {
            io.push(C);
            decode();
        }

    }

}

