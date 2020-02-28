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
            
        }
        
    }
}

#endif
