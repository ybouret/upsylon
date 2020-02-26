#include "y/information/translator/rle/encoder.hpp"
 
#define Y_RLE_DEBUG 1

#if defined(Y_RLE_DEBUG)
#include "y/code/utils.hpp"
#define Y_RLE(CODE) do { CODE; } while(false)
#else
#define Y_RLE(CODE)
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
            


            
            Encoder:: Encoder() : Parameters(), TranslatorQueue(2),
            status( waitForFirstByte ),
            preceding(-1),
            repeating( 0),
            different( 0),
            cache()
            {
            }
            
            void Encoder:: reset() throw()
            {
                // build an agnostic state
                free();
                status    = waitForFirstByte;
                preceding = -1;
                repeating =  0;
                different =  0;
                
            }
            
            void Encoder:: flush()
            {
                emit();
            }

            void Encoder:: emitRepeating()
            {
                assert(waitForRepeating==status);
                assert(0==different);
                assert(repeating>0);
                assert(preceding>=0);
                assert(preceding<=255);

                Y_RLE(std::cerr << "[RLE] repeating '" << visible_char[preceding] << "' #" << repeating << std::endl);
                EmitRepeatingTo(*this,preceding,repeating);
                repeating =  0;
                preceding = -1;
                status    = waitForFirstByte;
            }

            void Encoder:: emitDifferent()
            {
                assert(waitForDifferent==status);
                assert(0==repeating);
                assert(different>0);
#ifndef NDEBUG
                for(size_t i=1;i<different;++i)
                {
                    assert( cache[i-1] != cache[i]);
                }
#endif
                EmitDifferentTo(*this,cache,different);
                different = 0;
                preceding = -1;
                status    = waitForFirstByte;
            }

            void Encoder:: emit()
            {

                switch(status)
                {
                    case waitForFirstByte:
                        assert( -1 == preceding );
                        assert(  0 == repeating );
                        assert(  0 == different );
                        break;

                    case waitForRepeating:
                        emitRepeating();
                        break;

                    case waitForDifferent:
                        emitDifferent();
                        break;
                }
                
            }
            
            void Encoder:: write(char C)
            {
                const uint8_t byte(C);
                const int     current = byte;

                switch(status)
                {
                    case waitForFirstByte:
                        //------------------------------------------------------
                        //
                        // initialize, assuming repeating
                        //
                        //------------------------------------------------------
                        Y_RLE(std::cerr << "[RLE] initialize with '" << visible_char[byte] << "'" << std::endl);
                        preceding = (cache[0] = current);
                        repeating = 1;
                        status    = waitForRepeating;
                        break;

                    case waitForRepeating:
                        assert(0==different);
                        assert(repeating>0);
                        assert(preceding>=0);
                        assert(preceding<256);
                        assert(repeating<MaxRepeating);
                        Y_RLE(std::cerr << "[RLE] repeating with '" << visible_char[byte] << "'/'" << visible_char[preceding] << "'" << std::endl);
                        if(current==preceding)
                        {
                            if(++repeating>=MaxRepeating)
                            {
                                emitRepeating();
                            }
                        }
                        else
                        {
                            Y_RLE(std::cerr << "[RLE] switch to different" << std::endl);
                            exit(1);
                        }
                        break;

                    case waitForDifferent:
                        exit(1);
                        break;

                }

            }
            
            
            
            
        }
        
    }
    
}


