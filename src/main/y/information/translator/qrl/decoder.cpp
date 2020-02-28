
#include "y/information/translator/qrl/decoder.hpp"
#include "y/exceptions.hpp"

//#define Y_RLE_DEBUG 1

#if defined(Y_RLE_DEBUG)
#include "y/code/utils.hpp"
#define Y_RLE(CODE) do { CODE; } while(false)
#else
#define Y_RLE(CODE)
#endif


namespace upsylon {

    namespace Information {

        namespace QRL {

            const char * Decoder:: name()   const throw() { return DecoderID; }
            const char * Decoder:: family() const throw() { return FMID;      }

            Decoder:: Decoder() : Parameters(), TranslatorQueue( MaxBlockSize ),
            status( waitForDirective ),
            count(0)
            {
                
            }

            Decoder:: ~Decoder() throw()
            {
            }

            void Decoder:: reset() throw()
            {
                free();
                status = waitForDirective;
                count  = 0;
            }

            void Decoder:: flush()
            {
                static const char fn[] = "QRL::Decoder::flush";
                switch( status )
                {
                    case waitForDirective:
                        break;

                    case waitForRepeating:
                        assert(count>0);
                        throw exception("%s(waiting for #repeating=%u)", fn, unsigned(count));
                        break;

                    case waitForDifferent:
                        assert(count>0);
                        throw exception("%s(waiting for #different=%u)", fn, unsigned(count));
                        break;
                }
            }

            void Decoder:: write(char C)
            {
                static const unsigned mark = BehaviorCode;
                const uint8_t         byte = uint8_t(C);
                const unsigned        code = byte;

                switch( status )
                {
                    case waitForDirective:
                        assert(count==0);
                        Y_RLE(std::cerr << "[UNRLE] waitForDirective" << std::endl);
                        if(code<=mark)
                        {
                            count = code+1; assert(count<=MaxRepeating);
                            status = waitForRepeating;
                            Y_RLE(std::cerr << "[UNRLE] waitForRepeating " << count << std::endl);

                        }
                        else
                        {
                            assert(code>mark);
                            count = code - mark; assert(count<=MaxDifferent);
                            status = waitForDifferent;
                            Y_RLE(std::cerr << "[UNRLE] waitForDifferent " << count << std::endl);

                        }
                        break;

                    case waitForRepeating:
                        assert(count>0);
                        Y_RLE(std::cerr << "[UNRLE] '" << visible_char[byte] << "' * " << count << std::endl);
                        for(;count>0;--count)
                        {
                            push_back(byte);
                        }
                        assert(0==count);
                        status = waitForDirective;
                        break;

                    case waitForDifferent:
                        assert(count>0);
                        Y_RLE(std::cerr << "[UNRLE] + '" << visible_char[byte] << "'" << std::endl);
                        push_back(byte);
                        if(--count<=0)
                        {
                            status = waitForDirective;
                            assert(0==count);
                        }
                        break;
                }
            }
        }

    }

}


