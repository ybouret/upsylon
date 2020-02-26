

//! \file

#ifndef Y_TRANSLATOR_SHANNON_FANO_ENCODER_INCLUDED
#define Y_TRANSLATOR_SHANNON_FANO_ENCODER_INCLUDED 1

#include "y/information/translator/shannon-fano.hpp"
#include "y/information/translator/queue.hpp"
#include "y/information/translator/encoding-tree.hpp"

namespace upsylon {

    namespace information {

        namespace ShannonFano {

            typedef EncodingWith<Tree> EncodingTree;

            //! ShannonFano Encoder
            class Encoder : public EncodingTree, public TranslatorQueue
            {
            public:
                virtual const char *family() const throw();         //!< FMID
                virtual const char *name() const throw();           //!< ENCODER
                explicit            Encoder();                      //!< initialize all
                virtual            ~Encoder() throw();              //!< cleanup
                virtual void        reset() throw();                //!< restart and clean all
                virtual void        write(char);                    //!< emit and update
                virtual void        flush();                        //!< EOS
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Encoder);
                qbits Q;
            };

        }

    }
}

#endif

