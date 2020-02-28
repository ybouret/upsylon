
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
                
            };
            
            
            
            template <typename QENCODER = QWriter>
            class SingleEncoder
            {
            public:
                inline virtual ~SingleEncoder() throw() {}
                
            protected:
                inline explicit SingleEncoder() : enc( new QENCODER() )
                {}
                
                inline QEncoder & _character() throw() { return *enc; }
                inline QEncoder & _repeating() throw() { return *enc; }
                inline QEncoder & _different() throw() { return *enc; }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(SingleEncoder);
                QEncoder::Pointer enc;
            };
            
            
            template <
            typename CHARACTER_QENCODER=QWriter,
            typename REPEATING_QENCODER=QWriter
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
                
                inline QEncoder & _character() throw() { return *characterEnc; }
                inline QEncoder & _repeating() throw() { return *repeatingEnc; }
                inline QEncoder & _different() throw() { return *repeatingEnc; }
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(DoubleEncoder);
                QEncoder::Pointer characterEnc;
                QEncoder::Pointer repeatingEnc;

            };
            
            template <
            typename CHARACTER_QENCODER=QWriter,
            typename REPEATING_QENCODER=QWriter,
            typename DIFFERENT_QENCODER=QWriter
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
                
                inline QEncoder & _character() throw() { return *characterEnc; }
                inline QEncoder & _repeating() throw() { return *repeatingEnc; }
                inline QEncoder & _different() throw() { return *differentEnc; }
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(TripleEncoder);
                QEncoder::Pointer characterEnc;
                QEncoder::Pointer repeatingEnc;
                QEncoder::Pointer differentEnc;

            };
            
            
            
            
            template <typename ENCODER = SingleEncoder<> >
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
