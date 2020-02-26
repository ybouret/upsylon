
//! \file
#ifndef Y_INFORMATION_RUN_LENGTH_ENCODER_INCLUDED
#define Y_INFORMATION_RUN_LENGTH_ENCODER_INCLUDED 1

#include "y/information/translator/run-length.hpp"
#include "y/information/translator/queue.hpp"

namespace upsylon {
    
    namespace information {
        
        namespace RunLength {
            
            //! Run Length Encoder
            class Encoder : public Framework, public TranslatorQueue
            {
            public:
                virtual const char *name()   const throw(); //!< EncoderID
                virtual const char *family() const throw(); //!< FMID
                
                explicit Encoder(qencoder &characterEnc,
                                 qencoder &repeatingEnc,
                                 qencoder &differentEnc);   //!< setup
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
                qencoder &characterEncoder;
                qencoder &repeatingEncoder;
                qencoder &differentEncoder;
                Status    status;      //!< current status
                int       preceding;   //!< preceding char
                size_t    repeating;   //!< number of repeating same char
                size_t    different;   //!< number of different chars in cache
                qbits     Q;
                uint8_t   cache[MaxDifferent];
                
                void    emit();
                void    emitRepeating();
                void    emitDifferent();
                
                void    fromRepeatingToDifferentWith(const uint8_t byte);
                void    fromDifferentToRepeatingWith(const uint8_t byte);
                
            };
            
            
            
            template <typename QENCODER>
            class SingleEncoder
            {
            public:
                inline virtual ~SingleEncoder() throw() {}
                
            protected:
                inline explicit SingleEncoder() : enc( new QENCODER() )
                {}
                
                inline qencoder & _character() throw() { return *enc; }
                inline qencoder & _repeating() throw() { return *enc; }
                inline qencoder & _different() throw() { return *enc; }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(SingleEncoder);
                qencoder::pointer enc;
            };
            
            
            template <
            typename CHARACTER_QENCODER,
            typename REPEATING_QENCODER
            >
            class DoubleEncoder
            {
            public:
                inline virtual ~DoubleEncoder() throw() {}
                
            protected:
                inline explicit DoubleEncoder() :
                characterEnc( new CHARACTER_QENCODER() ),
                repeatingEnc( new REPEATING_QENCODER() )
                {}
                
                inline qencoder & _character() throw() { return *characterEnc; }
                inline qencoder & _repeating() throw() { return *repeatingEnc; }
                inline qencoder & _different() throw() { return *repeatingEnc; }
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(DoubleEncoder);
                qencoder::pointer characterEnc;
                qencoder::pointer repeatingEnc;

            };
            
            template <
            typename CHARACTER_QENCODER,
            typename REPEATING_QENCODER,
            typename DIFFERENT_QENCODER
            >
            class TripleEncoder
            {
            public:
                inline virtual ~TripleEncoder() throw() {}
                
            protected:
                inline explicit TripleEncoder() :
                characterEnc( new CHARACTER_QENCODER() ),
                repeatingEnc( new REPEATING_QENCODER() ),
                differentEnc( new DIFFERENT_QENCODER() )
                {}
                
                inline qencoder & _character() throw() { return *characterEnc; }
                inline qencoder & _repeating() throw() { return *repeatingEnc; }
                inline qencoder & _different() throw() { return *differentEnc; }
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(TripleEncoder);
                qencoder::pointer characterEnc;
                qencoder::pointer repeatingEnc;
                qencoder::pointer differentEnc;

            };
            
            
            
            
            template <typename ENCODER>
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
            };
            
            
        }
    }
}


#endif
