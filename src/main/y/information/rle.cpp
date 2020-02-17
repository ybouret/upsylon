#include "y/information/rle.hpp"

namespace upsylon {

    namespace information {

        namespace RLE {

            Context:: Context() throw() : last(-1)
            {
            }

            Context:: ~Context() throw()
            {
                last=-1;
            }

            void Context:: restart() throw()
            {
                last = -1;
                free();
            }
        }

        namespace RLE {

            Encoder:: Encoder() throw() : Context(), flag( waiting )
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
                        push_back(count);
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
                            push_back(count);
                            count = 0;
                            last  = curr;
                            push_back(curr);
                            flag  = waiting;
                        }
                        break;
                }
            }

        }


    }
}
