
//! \file
#ifndef Y_INFORMATION_RLE_ENCODER_INCLUDED
#define Y_INFORMATION_RLE_ENCODER_INCLUDED 1

#include "y/information/filter/rle.hpp"

namespace upsylon {

    namespace information {

        namespace RLE {

            class Encoder : public Context
            {
            public:
                explicit Encoder() throw();
                virtual ~Encoder() throw();

                virtual void reset() throw();
                virtual void write(char);
                virtual void flush();

            private:
                enum Flag
                {
                    waiting,
                    counting,
                };
                Flag   flag;
                size_t count;

                Y_DISABLE_COPY_AND_ASSIGN(Encoder);
            };


        }

    }

}

#endif
