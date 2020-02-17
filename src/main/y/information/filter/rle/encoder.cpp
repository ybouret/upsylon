#include "y/information/filter/rle/encoder.hpp"

namespace upsylon {

    namespace information {

        namespace RLE {

            Encoder:: Encoder() throw() :
            Context(),
            flag( waiting ),
            count(0)
            {
            }

            Encoder:: ~Encoder() throw()
            {}

            void Encoder:: flush()
            {
                switch(flag)
                {
                    case waiting:
                        break;

                    case counting:
                        push_back( uint8_t(count) );
                        count=0;
                        break;
                }
                last=-1;
            }

            void Encoder:: reset() throw()
            {
                restart();
                flag  = waiting;
                count = 0;
            }



            void Encoder:: write(char C)
            {
                const uint8_t byte = uint8_t(C);
                const int     curr = int( byte );
                switch(flag)
                {
                    case waiting:
                        push_back( byte );
                        if(curr!=last)
                        {
                            last = curr;
                        }
                        else
                        {
                            last  = curr;
                            count = 0;
                            flag  = counting;
                        }
                        break;

                    case counting:
                        if(curr==last&&count<0xff)
                        {
                            ++count;
                        }
                        else
                        {
                            push_back(uint8_t(count));
                            count = 0;
                            last  = curr;
                            flag  = waiting;
                            push_back(curr);
                        }
                        break;
                }
            }

        }

    }

}
