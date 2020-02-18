//! \file

#ifndef Y_INFORMATION_RLE_DECODER_INCLUDED
#define Y_INFORMATION_RLE_DECODER_INCLUDED 1

#include "y/information/filter/rle.hpp"

namespace upsylon {

    namespace information {

        namespace RLE {


            //! RLE decoder
            class Decoder : public Context
            {
            public:
                explicit Decoder() throw(); //!< setup
                virtual ~Decoder() throw(); //!< cleanup

                virtual void reset() throw(); //!< restart, flag=wait_for_input
                virtual void write(char);     //!< decode with new char
                virtual void flush();         //!< check state

            private:
                enum Flag
                {
                    wait_for_input,
                    wait_for_count
                };
                Flag flag;

                Y_DISABLE_COPY_AND_ASSIGN(Decoder);
            };

        }

    }

}

#endif
