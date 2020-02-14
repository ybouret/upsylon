
#include "y/information/filter/huffman/encoder.hpp"

namespace upsylon {

    namespace information {

        namespace Huffman {

            Encoder:: Encoder(const Mode m) :
            Context(m)
            {
            }

            Encoder:: ~Encoder() throw()
            {
            }

            void  Encoder:: write( char C )
            {
                const uint8_t u = C;
                (void) send(io,u);
                compile();
                buildTree();
            }

            void Encoder:: flush()
            {
                switch( mode )
                {
                    case StreamMode: eos->emit(io);
                        break;

                    case BufferMode:
                        break;
                }
                zfinish();

            }

            void  Encoder:: reset() throw()
            {
                setupTree();
                free_all();
            }

        }

    }

}

