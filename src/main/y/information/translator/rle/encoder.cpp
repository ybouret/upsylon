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

                Y_RLE(std::cerr << "[RLE] emit repeating '" << visible_char[preceding] << "' #" << repeating << std::endl);
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
                Y_RLE(std::cerr << "[RLE] emit #different=" << different << ": '";
                      for(size_t i=0;i<different;++i) std::cerr << visible_char[cache[i]];
                      std::cerr << "'" << std::endl);
#ifndef NDEBUG
                for(size_t i=1;i<different;++i)
                {
                    assert( cache[i-1] != cache[i]);
                }
#endif

                EmitDifferentTo(*this,cache,different);
                different =  0;
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
                        // sanity check
                        //------------------------------------------------------
                        assert(0==different);
                        assert(0==repeating);
                        assert(-1==preceding);
                        Y_RLE(std::cerr << "[RLE] initialize with '" << visible_char[byte] << "'" << std::endl);

                        //------------------------------------------------------
                        // initialize, assuming repeating
                        //------------------------------------------------------
                        preceding = (cache[0] = current);
                        repeating = 1;
                        status    = waitForRepeating;

                        break;

                    case waitForRepeating:
                        //------------------------------------------------------
                        // sanity check
                        //------------------------------------------------------
                        assert(0==different);
                        assert(repeating>0);
                        assert(preceding>=0);
                        assert(preceding<256);
                        assert(repeating<MaxRepeating);
                        Y_RLE(std::cerr << "[RLE] update repeating with '" << visible_char[byte] << "'/'" << visible_char[preceding] << "'" << std::endl);

                        //------------------------------------------------------
                        // case study
                        //------------------------------------------------------
                        if(current==preceding)
                        {
                            //--------------------------------------------------
                            // increase count an flush if necessary
                            //--------------------------------------------------
                            if(++repeating>=MaxRepeating)
                            {
                                emitRepeating();
                            }
                        }
                        else
                        {
                            //--------------------------------------------------
                            // change strategy
                            //--------------------------------------------------
                            Y_RLE(std::cerr << "[RLE] switch to different from #repeating=" << repeating << std::endl);
                            fromRepeatingToDifferentWith(byte);
                        }

                        break;

                    case waitForDifferent:
                        //------------------------------------------------------
                        // sanity check
                        //------------------------------------------------------
                        assert(0==repeating);
                        assert(different>0);
                        assert(different<MaxDifferent);
                        assert(preceding>=0);
                        assert(preceding<256);
                        Y_RLE(std::cerr << "[RLE] different with '" << visible_char[byte] << "'/'" << visible_char[preceding] << "'" << std::endl);

                        //------------------------------------------------------
                        // case study
                        //------------------------------------------------------
                        if(current!=preceding)
                        {
                            //--------------------------------------------------
                            // increase count an flush if necessary
                            //--------------------------------------------------
                            cache[different++] = byte;
                            if(different>=MaxDifferent)
                            {
                                emitDifferent();
                            }
                            else
                            {
                                preceding = current;
                            }
                        }
                        else
                        {
                            //--------------------------------------------------
                            // change strategy
                            //--------------------------------------------------
                            Y_RLE(std::cerr << "[RLE] switch to repeating with #different=" << different << std::endl);
                            fromDifferentToRepeatingWith(byte);
                            assert(int(byte)==preceding);
                        }

                        break;

                }

            }
            
            void Encoder:: fromRepeatingToDifferentWith(const uint8_t byte)
            {
                switch( repeating )
                {
                        // just a former first repeating byte
                    case 1:
                        assert( int(cache[0]) == preceding );
                        cache[1]  = byte;
                        different = 2;
                        repeating = 0;
                        break;

                        // a few repeating bytes
                    default:
                        emitRepeating();
                        cache[0]  = byte;
                        different = 1;
                        break;
                }
                assert(0==repeating);
                assert(different>0);
                status    = waitForDifferent;
                preceding = int(byte);
            }

            void Encoder:: fromDifferentToRepeatingWith(const uint8_t byte)
            {
                switch( different )
                {
                        // just a first  different byte
                    case 1:
                        assert( int(cache[0]) == preceding );
                        repeating = 2;
                        different = 0;
                        break;

                        // a few different byte
                    default:
                        emitDifferent();
                        cache[0]  = byte;
                        repeating = 1;
                        break;
                }
                assert(0==different);
                assert(repeating>0);
                status = waitForRepeating;
                preceding = int(byte);
            }
            
        }
        
    }
    
}


