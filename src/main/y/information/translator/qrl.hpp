//! \file

#ifndef Y_INFORMATION_QRL_INCLUDED
#define Y_INFORMATION_QRL_INCLUDED 1

#include "y/container/sequence.hpp"

namespace upsylon {

    namespace Information {

        namespace QRL {

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
                static const uint8_t MaxBlockSize = (MaxRepeating>=MaxDifferent) ? MaxRepeating : MaxDifferent; //!< for memory caches

                virtual ~Parameters() throw();

            protected:
                explicit Parameters() throw();

                //! helper
                static void EmitRepeatingTo(sequence<char> &Q,
                                            const uint8_t   b,
                                            const size_t    n);

                //! helper, call emit repeating if count==1
                static void EmitDifferentTo(sequence<char> &Q,
                                            const uint8_t  *cache,
                                            const size_t    count);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Parameters);
            };

        }

    }
}

#endif

