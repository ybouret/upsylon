//! \file

#ifndef Y_INFORMATION_RLE_DECODER_INCLUDED
#define Y_INFORMATION_RLE_DECODER_INCLUDED 1

#include "y/information/filter/rle.hpp"

namespace upsylon {

    namespace information {

        namespace RLE {


            class Decoder : public Context
            {
            public:
                explicit Decoder() throw();
                virtual ~Decoder() throw();

                virtual void reset() throw();
                virtual void write(char);
                virtual void flush();

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
