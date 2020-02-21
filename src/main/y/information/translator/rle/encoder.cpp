#include "y/information/translator/rle/encoder.hpp"

namespace upsylon {

    namespace information {

        namespace RLE {

            const char * Encoder:: name()   const throw() { return EncoderID; }
            const char * Encoder:: family() const throw() { return FMID;      }

            Encoder:: ~Encoder() throw()
            {
                memset(cache,0,sizeof(cache));
            }

            Encoder:: Encoder() : Parameters(), TranslatorQueue(2),
            last(-1),
            repeating(0),
            different(0),
            cache()
            {
            }

            void Encoder:: reset() throw()
            {
                last      = -1;
                repeating =  0;
                different =  0;
            }

            void Encoder:: flush()
            {
                emit();
            }

            void Encoder:: emit()
            {
                if(repeating>0)
                {
                    assert(different<=0);
                    assert(repeating<=BlockLength);
                    assert(last>=0);
                    assert(last<256);
                    push_back( uint8_t(repeating-1) );
                    push_back( last );
                    last      = -1;
                    repeating =  0;
                }
                else
                {
                    if(different>0)
                    {
                        assert(repeating<=0);
                        assert(different<=CacheLength);
                        assert(different+BlockSwitch<=255);
                        push_back( uint8_t(different+BlockSwitch) );
                        put_all((const char *)cache,different);
                        last=-1;
                        different=0;
                    }
                }
            }

            void Encoder:: write(char C)
            {
                const uint8_t byte(C);
                const int     curr = byte;
                if(last<0)
                {
                    assert(different<=0);
                    last = (cache[0] = curr);
                    repeating = 1;
                }
                else
                {
                    if(curr==last)
                    {
                        assert(different<=0);
                        ++repeating;
                    }
                    else
                    {
                        assert(different<CacheLength);
                        repeating = 0;
                        cache[++different] = curr;
                    }
                }

            }




        }

    }

}


