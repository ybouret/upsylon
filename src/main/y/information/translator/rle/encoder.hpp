//! \file
#ifndef Y_INFORMATION_RLE_ENCODER_INCLUDED
#define Y_INFORMATION_RLE_ENCODER_INCLUDED 1

#include "y/information/translator/rle.hpp"
#include "y/information/translator/queue.hpp"

namespace upsylon {

    namespace information {

        namespace RLE {

            class Encoder : public Parameters, public TranslatorQueue
            {
            public:
                virtual const char *name()   const throw();
                virtual const char *family() const throw();
                
                explicit Encoder();
                virtual ~Encoder() throw();

                virtual void write(char C);
                virtual void flush();
                virtual void reset() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Encoder);
                enum Status
                {
                    waitForFirstByte,
                    waitForRepeating,
                    waitForDifferent,
                };
                Status  status;      //!< current status
                int     preceding;   //!< preceding char
                size_t  repeating;   //!< number of repeating same char
                size_t  different;   //!< number of different chars in cache
                uint8_t cache[MaxDifferent];

                void    emit();
                void    emitRepeating();
                void    emitDifferent();

                void    fromRepeatingToDifferentWith(const uint8_t byte);  


            };

        }
    }
}


#endif
