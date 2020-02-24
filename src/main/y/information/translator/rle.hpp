//! \file

#ifndef Y_INFORMATION_RLE_INCLUDED
#define Y_INFORMATION_RLE_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    namespace information {

        namespace RLE {

            //! parameters to read/write rle
            /**
             According to BlockLength:
             - if(count<=BlockSwitch) then repeating (count+1) bytes [1..BlockLength]
             - if(count>BlockSwitch)  then different (count-BlockSwitch) bytes [1..255-BlockSwitch]
             */
            class Parameters
            {
            public:
                static const char    FMID[];                          //!< RLE
                static const uint8_t MaxRepeating = 128;              //!< max repeating block size
                static const uint8_t BehaviorCode = MaxRepeating-1;   //!< switch to be compared to
                static const uint8_t MaxDifferent = 255-BehaviorCode; //!< max different block size
                
                virtual ~Parameters() throw();

            protected:
                explicit Parameters() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Parameters);
            };

        }

    }
}

#endif

