

//! \file

#ifndef Y_TRANSLATOR_SHANNON_FANO_ENCODER_INCLUDED
#define Y_TRANSLATOR_SHANNON_FANO_ENCODER_INCLUDED 1

#include "y/information/translator/shannon-fano.hpp"
#include "y/information/translator/queue.hpp"

namespace upsylon {

    namespace information {

        namespace ShannonFano {

            //! ShannonFano Encoder
            class Encoder : public Tree, public TranslatorQueue
            {
            public:
                static const char   CLID[];               //!< ShannonFanoEncoder
                virtual const char *name() const throw(); //!< CLID
                explicit            Encoder();            //!< initialize all
                virtual            ~Encoder() throw();    //!< cleanup
                virtual void        write(char C);        //!< emit according to tree and update
                virtual void        flush();              //!< emit EOS
                virtual void        reset() throw();      //!< restart and clean all

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Encoder);
                qbits Q;
            };

        }

    }
}

#endif

