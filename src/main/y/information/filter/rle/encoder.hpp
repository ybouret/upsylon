
//! \file
#ifndef Y_INFORMATION_RLE_ENCODER_INCLUDED
#define Y_INFORMATION_RLE_ENCODER_INCLUDED 1

#include "y/information/filter/rle.hpp"

namespace upsylon {

    namespace information {

        namespace RLE {

            //! Run-Length encoder
            class Encoder : public Context
            {
            public:
                explicit Encoder() throw(); //!< setup
                virtual ~Encoder() throw(); //!< cleanup

                virtual void reset() throw(); //!< restart
                virtual void write(char);     //!< encode new char
                virtual void flush();         //!< finish all

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
