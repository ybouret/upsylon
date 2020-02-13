
#include "y/information/filter/huffman.hpp"

namespace upsylon {

    namespace information {

        Huffman:: Encoder:: Encoder(const Mode m) :
        Context(m)
        {
        }

        Huffman:: Encoder:: ~Encoder() throw()
        {
        }
        
        void Huffman:: Encoder:: write( char C )
        {
            const uint8_t u = C;
            (void) emit(io,u);
            compile();
            buildTree();
        }

        void Huffman:: Encoder:: flush()
        {
            switch( mode )
            {
                case StreamMode: eos->emit(io); break;
                case BufferMode: break;
            }
            zfinish();

        }

        void Huffman:: Encoder:: reset() throw()
        {
            setupTree();
            free_all();
        }


    }

}

