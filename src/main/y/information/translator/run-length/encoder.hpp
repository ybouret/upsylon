
//! \file
#ifndef Y_INFORMATION_RUN_LENGTH_ENCODER_INCLUDED
#define Y_INFORMATION_RUN_LENGTH_ENCODER_INCLUDED 1

#include "y/information/translator/run-length.hpp"
#include "y/information/translator/queue.hpp"

namespace upsylon {
    
    namespace Information {
        
        namespace RunLength {
            
            //! Run Length Encoder
            class Encoder : public Framework, public TranslatorQueue
            {
            public:
                virtual const char *name()   const throw(); //!< EncoderID
                virtual const char *family() const throw(); //!< FMID
                
                explicit Encoder(QEncoder &characterEnc,
                                 QEncoder &repeatingEnc,
                                 QEncoder &differentEnc);   //!< setup
                virtual ~Encoder() throw();                 //!< cleanup
                
                virtual void write(char C);   //!< encode next char
                virtual void flush();         //!< flush current status
                virtual void reset() throw(); //!< fresh start
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Encoder);
                enum Status
                {
                    waitForFirstByte,
                    waitForRepeating,
                    waitForDifferent,
                };
                QEncoder &characterEncoder;
                QEncoder &repeatingEncoder;
                QEncoder &differentEncoder;
                Status    status;      //!< current status
                int       preceding;   //!< preceding char
                size_t    repeating;   //!< number of repeating same char
                size_t    different;   //!< number of different chars in cache
                IOBits    Q;
                uint8_t   cache[MaxDifferent];
                
                void    emit();
                void    emitRepeating();
                void    emitDifferent();
                
                void    fromRepeatingToDifferentWith(const uint8_t byte);
                void    fromDifferentToRepeatingWith(const uint8_t byte);

                virtual void resetBits() throw() = 0;
            };
            
            
            

            
            
            template <typename ENCODER = Single<QEncoder,QWriter> >
            class EncodeWith : public ENCODER, public Encoder
            {
            public:
                inline virtual ~EncodeWith() throw() {}
                inline explicit EncodeWith() :
                ENCODER(),
                Encoder( this->_character(), this->_repeating(), this->_different() )
                {
                }


                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(EncodeWith);
                inline virtual void resetBits() throw()
                {
                    this->resetBrokers();
                }
            };
            
            
        }
    }
}


#endif
