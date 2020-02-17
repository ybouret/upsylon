#include "y/information/filter/rle/decoder.hpp"

namespace upsylon {

    namespace information {

        namespace RLE {

            Decoder:: Decoder() throw() :
            Context(),
            flag(wait_for_input)
            {

            }

            Decoder:: ~Decoder() throw()
            {
            }

            void Decoder:: reset() throw()
            {
                restart();
                flag = wait_for_input;
            }

            void Decoder:: flush()
            {
                
            }

            void Decoder:: write(char C)
            {
                const uint8_t byte = C;
                const int     curr = byte;
                switch(flag)
                {
                    case wait_for_input:
                        push_back(byte);
                        if(curr==last)
                        {
                            flag = wait_for_count;
                        }
                        else
                        {
                            last=curr;
                        }
                        break;

                    case wait_for_count:
                        break;
                }
            }
        }

    }

}

