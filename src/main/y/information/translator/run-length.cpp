
#include "y/information/translator/run-length.hpp"

namespace upsylon {
    
    namespace Information {
        
        namespace RunLength {
            
            const char Framework:: FMID[] = "RunLength";
            
            Framework:: ~Framework() throw()
            {
            }
            
            Framework:: Framework() throw()
            {
            }
            
            void Framework:: EmitRepeatingTo(sequence<char>  &S,
                                             IOBits          &Q,
                                             const uint8_t    byte,
                                             const size_t     size,
                                             QEncoder        &byteEncoder,
                                             QEncoder        &sizeEncoder)
            {
                assert(size>0);
                assert(size<=MaxRepeating);
                sizeEncoder.writeBits(Q,uint8_t(size-1));
                byteEncoder.writeBits(Q,byte);
                Q.compile(S);
            }
            
            void Framework:: EmitDifferentTo(sequence<char> &S,
                                             IOBits         &Q,
                                             const uint8_t *cache,
                                             const uint8_t  count,
                                             QEncoder      &characterEncoder,
                                             QEncoder      &repeatingEncoder,
                                             QEncoder      &differentEncoder)
            {
                assert(NULL!=cache);
                assert(count>0);
                assert(count<=MaxDifferent);
                assert(count+BehaviorCode<=255);
                switch(count)
                {
                    case 1:
                        EmitRepeatingTo(S,Q,cache[0],1,characterEncoder,repeatingEncoder);
                        break;
                        
                    default:
                        differentEncoder.writeBits(Q, uint8_t(count+BehaviorCode) );
                        for(size_t i=0;i<count;++i)
                        {
                            characterEncoder.writeBits(Q,cache[i]);
                        }
                        Q.compile(S);
                }
            }
            
        }
        
    }
    
}
