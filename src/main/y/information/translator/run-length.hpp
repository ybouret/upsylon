//! \file

#ifndef Y_INFORMATION_RUN_LENGTH_INCLUDED
#define Y_INFORMATION_RUN_LENGTH_INCLUDED 1

#include "y/container/sequence.hpp"
#include "y/information/qencoder.hpp"

namespace upsylon {
    
    namespace Information {
        
        namespace RunLength {
            
            //! Framework to read/write
            /**
             According to BlockLength:
             - if(count<=BlockSwitch) then repeating (count+1) bytes [1..BlockLength]
             - if(count>BlockSwitch)  then different (count-BlockSwitch) bytes [1..255-BlockSwitch]
             */
            class Framework
            {
            public:
                static const char    FMID[];                          //!< RunLength
                static const uint8_t MaxRepeating = 128;              //!< max repeating block size
                static const uint8_t BehaviorCode = MaxRepeating-1;   //!< switch to be compared to
                static const uint8_t MaxDifferent = 255-BehaviorCode; //!< max different block size
                static const uint8_t MaxBlockSize = (MaxRepeating>=MaxDifferent) ? MaxRepeating : MaxDifferent; //!< for memory caches
                
                virtual ~Framework() throw(); //!< cleanup
                
            protected:
                explicit Framework() throw(); //!< setup

                //! helper to encode a repeating sequence
                static void EmitRepeatingTo(sequence<char> &S,
                                            IOBits         &Q,
                                            const uint8_t   byte,
                                            const size_t    size,
                                            QEncoder       &characterEncoder,
                                            QEncoder       &repeatingEncoder);

                //! helper to encode a different sequence, switch to repeating is count==1
                static void EmitDifferentTo(sequence<char> &S,
                                            IOBits         &Q,
                                            const uint8_t  *cache,
                                            const uint8_t   count,
                                            QEncoder       &characterEncoder,
                                            QEncoder       &repeatingEncoder,
                                            QEncoder       &differentEncoder);
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Framework);
            };

            template <
            typename BROKER,
            typename CHARACTER_BROKER
            >
            class Single
            {
            public:
                typedef typename BROKER::Pointer PointerType;

                explicit Single() : characterBroker( new CHARACTER_BROKER() ) {}
                virtual ~Single() throw() {}

            protected:
                BROKER & _character() throw() { return *characterBroker; }
                BROKER & _repeating() throw() { return *characterBroker; }
                BROKER & _different() throw() { return *characterBroker; }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Single);
                PointerType characterBroker;
            };

            template <
            typename BROKER,
            typename CHARACTER_BROKER,
            typename REPEATING_BROKER
            >
            class Double
            {
            public:
                typedef typename BROKER::Pointer PointerType;

                explicit Double() :
                characterBroker( new CHARACTER_BROKER() ),
                repeatingBroker( new REPEATING_BROKER() )
                {}

                virtual ~Double() throw() {}

            protected:
                BROKER & _character() throw() { return *characterBroker; }
                BROKER & _repeating() throw() { return *repeatingBroker; }
                BROKER & _different() throw() { return *repeatingBroker; }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Double);
                PointerType characterBroker;
                PointerType repeatingBroker;
            };


            template <
            typename BROKER,
            typename CHARACTER_BROKER,
            typename REPEATING_BROKER,
            typename DIFFERENT_BROKER
            >
            class Triple
            {
            public:
                typedef typename BROKER::Pointer PointerType;

                explicit Triple() :
                characterBroker( new CHARACTER_BROKER() ),
                repeatingBroker( new REPEATING_BROKER() ),
                differentBroker( new DIFFERENT_BROKER() )
                {}

                virtual ~Triple() throw() {}

            protected:
                BROKER & _character() throw() { return *characterBroker; }
                BROKER & _repeating() throw() { return *repeatingBroker; }
                BROKER & _different() throw() { return *differentBroker; }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Triple);
                PointerType characterBroker;
                PointerType repeatingBroker;
                PointerType differentBroker;

            };

        }
        
    }
}

#endif
