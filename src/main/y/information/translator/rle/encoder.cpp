#include "y/information/translator/rle/encoder.hpp"
#include "y/code/utils.hpp"

#define Y_RLE_DEBUG 1

#if defined(Y_RLE_DEBUG)
#define Y_RLE(CODE) do { CODE; } while(false)
#else
#define Y_RLE(OUT)
#endif

namespace upsylon {
    
    namespace information {
        
        namespace RLE {
            
            const char * Encoder:: name()   const throw() { return EncoderID; }
            const char * Encoder:: family() const throw() { return FMID;      }
            
            Encoder:: ~Encoder() throw()
            {
                memset(cache,0,sizeof(cache));
            }
            
            static inline
            bool checkValidCache(const uint8_t *cache,
                                 const size_t   count) throw()
            {
                assert(cache);
                assert(count>0);
                for(size_t i=1;i<count;++i)
                {
                    if(cache[i-1]==cache[i]) return false;
                }
                return true;
            }

            bool Encoder:: checkStatus() const throw()
            {
                switch (status)
                {
                    case waitForFirstByte:
                        return (-1==preceding) && (0==repeating) && (0==different);

                    case waitForRepeating:
                        return (preceding>=0) && (preceding<256) && (repeating>0) &&  (repeating<=MaxRepeating) && (0==different);

                    case waitForDifferent:
                        return (preceding>=0) && (preceding<256) && (0==repeating) && (different>0) && (different<=MaxDifferent) && checkValidCache(cache,different);
                }
                return false;
            }
            
            Encoder:: Encoder() : Parameters(), TranslatorQueue(2),
            status( waitForFirstByte ),
            preceding(-1),
            repeating( 0),
            different( 0),
            cache()
            {
                assert( checkStatus() || die("RLE.invalid constructor") );
            }
            
            void Encoder:: reset() throw()
            {
                // build an agnostic state
                free();
                status    = waitForFirstByte;
                preceding = -1;
                repeating =  0;
                different =  0;
                assert( checkStatus() || die("RLE.invalid reset") );
                
            }
            
            void Encoder:: flush()
            {
                // emit current state
                emit();
                assert( checkStatus() || die("RLE.invalid flush") );

            }


            void Encoder:: emitRepeating()
            {

                // debug check
                assert( checkStatus() || die("RLE.invalid emitRepeating.entry") );
                Y_RLE(std::cerr << "[RLE] emit repeating #" << repeating << " '" << visible_char[uint8_t(preceding)] << "'" << std::endl);

                // encode
                push_back( uint8_t(repeating-1) );
                push_back( uint8_t(preceding)   );

                // get back to agnostic state
                status    = waitForFirstByte;
                preceding = -1;
                repeating =  0;

                assert( checkStatus() || die("RLE.invalid emitRepeating.leave") );

            }


            void Encoder:: emitDifferent()
            {
                assert( checkStatus() || die("RLE.invalid emitDifferent.entry") );
                Y_RLE(std::cerr << "[RLE] emit different #" << different << std::endl);

                // debug check
                assert(different>0);
                assert(repeating<=0);
                assert(different<=MaxDifferent);
                assert(different+BehaviorCode<=255);

                // emit cache
                push_back( uint8_t(different+BehaviorCode) );
                put_all((const char *)cache,different);

                // get back to an agnostic state
                status    = waitForFirstByte;
                preceding = -1;
                different =  0;
                assert( checkStatus() || die("RLE.invalid emitDifferent.leave") );
            }

            void Encoder:: emit()
            {
                if(repeating>0)
                {
                    emitRepeating();
                }
                else if(different>0)
                {
                    emitDifferent();
                }
                else
                {
                    assert( checkStatus() || die("RLE.invalid call to emit") );
                }
                
            }
            
            void Encoder:: write(char C)
            {
                const uint8_t byte(C);
                const int     current = byte;
                


            }
            
            
            
            
        }
        
    }
    
}


