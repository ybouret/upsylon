#include "y/information/filter/rle/decoder.hpp"
#include "y/exception.hpp"

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
                switch( flag )
                {
                    case wait_for_input:
                        last = -1;
                        break;

                    case wait_for_count:
                        flag = wait_for_input;
                        throw exception("UNRLE: flushing while waiting for #byte");
                        break;
                }
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
                        if(last<=0) throw exception("UNRLE: invalid negative last value");
                        for(int i=0;i<curr;++i)
                        {
                            push_back(last);
                        }
                        last = -1;
                        flag = wait_for_input;
                        break;
                }
            }
        }

    }

}

