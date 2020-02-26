//! \file

#ifndef Y_INFORMATION_RUN_LENGTH_INCLUDED
#define Y_INFORMATION_RUN_LENGTH_INCLUDED 1

#include "y/container/sequence.hpp"
#include "y/information/qencoder.hpp"

namespace upsylon {
    
    namespace information {
        
        namespace RunLength {
            
            //! parameters to read/write rle
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
                
                virtual ~Framework() throw();
                
            protected:
                explicit Framework() throw();
                static void EmitRepeatingTo(qbits        &Q,
                                            const uint8_t byte,
                                            const size_t  size,
                                            qencoder     &characterEncoder,
                                            qencoder     &repeatingEncoder);
                
                static void EmitDifferentTo(qbits         &Q,
                                            const uint8_t *cache,
                                            const uint8_t  count,
                                            qencoder      &characterEncoder,
                                            qencoder      &repeatingEncoder,
                                            qencoder      &differentEncoder);
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Framework);
            };
            
        }
        
    }
}

#endif
