//! \file
#ifndef Y_INFORMATION_HUFFMAN_ENCODER_INCLUDED
#define Y_INFORMATION_HUFFMAN_ENCODER_INCLUDED 1

#include "y/information/filter/huffman.hpp"
#include "y/information/filter/xqueue.hpp"

namespace upsylon {

    namespace information {

        namespace Huffman {
            
            //! encoder based on a context and an eXtended Queue Filter
            class Encoder : public Context, public filterXQ
            {
            public:
                explicit Encoder(const Mode m); //!< setup
                virtual ~Encoder() throw();     //!< cleanup

                virtual void reset() throw();   //!< restart context and free all data
                virtual void write(char C);     //!< encode char
                virtual void flush();           //!< flush with/out EOS

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Encoder);
            };

        }

    }

}

#endif

