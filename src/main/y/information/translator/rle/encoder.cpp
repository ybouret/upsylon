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
            
            
            bool Encoder:: isJustWaiting() const throw()
            {
                return (-1 == last) && (0==repeating) && (0==different);
            }
            
            Encoder:: Encoder() : Parameters(), TranslatorQueue(2),
            last(-1),
            repeating(0),
            different(0),
            cache()
            {
                //----------------------------------------------------------
                //
                // start in a perfectly agnostic state
                //
                //----------------------------------------------------------
                assert(isJustWaiting());
            }
            
            void Encoder:: reset() throw()
            {
                // build an agnostic state
                last      = -1;
                repeating =  0;
                different =  0;
                assert(isJustWaiting());
                
            }
            
            void Encoder:: flush()
            {
                // emit current state
                emit();
            }
            
            void Encoder:: emit()
            {
                if(repeating>0)
                {
                    //----------------------------------------------------------
                    // we were in a repeating state
                    //----------------------------------------------------------
                    
                    // debug check
                    assert(different<=0);
                    assert(repeating<=BlockLength);
                    assert(last>=0);
                    assert(last<256);
                    
                    // encode
                    push_back( uint8_t(repeating-1) );
                    push_back( last );
                    
                    // get back to agnostic state
                    last      = -1;
                    repeating =  0;
                    
                    assert( isJustWaiting() );
                }
                else if(different>0)
                {
                    //----------------------------------------------------------
                    // we were in a different state
                    //----------------------------------------------------------
                    // debug check
                    assert(repeating<=0);
                    assert(different<=CacheLength);
                    assert(different+BlockSwitch<=255);
                    
                    // emit cache
                    push_back( uint8_t(different+BlockSwitch) );
                    put_all((const char *)cache,different);
                    
                    // get back to an agnostic state
                    last     = -1;
                    different=  0;
                    assert( isJustWaiting() );

                }
                else
                {
                    assert(isJustWaiting());
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


