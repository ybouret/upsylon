
#include "y/information/translator/qrl.hpp"

namespace upsylon {
    
    namespace Information {

        namespace QRL {

            Parameters:: ~Parameters() throw()
            {
            }

            Parameters:: Parameters() throw()
            {
            }

            const char Parameters:: FMID[] = "RLE";

            void Parameters:: EmitRepeatingTo(sequence<char> &Q,
                                              const uint8_t   b,
                                              const size_t    n)
            {
                assert(n>0);
                assert(n<=MaxRepeating);
                Q.push_back( uint8_t(n-1) );
                Q.push_back( b );
            }

            void Parameters::  EmitDifferentTo(sequence<char> &Q,
                                               const uint8_t  *cache,
                                               const size_t    count)
            {
                assert(NULL!=cache);
                assert(count>0);
                assert(count<=MaxDifferent);
                assert(count+BehaviorCode<=255);
                switch( count)
                {
                    case 1:
                        EmitRepeatingTo(Q,cache[0],1);
                        break;

                    default:
                        Q.push_back( uint8_t(count+BehaviorCode) );
                        for(size_t i=0;i<count;++i)
                        {
                            Q.push_back(cache[i]);
                        }
                }
            }

        }

    }
}
