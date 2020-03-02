

//! \file

#ifndef Y_TRANSLATOR_SHANNON_FANO_ENCODER_INCLUDED
#define Y_TRANSLATOR_SHANNON_FANO_ENCODER_INCLUDED 1

#include "y/information/translator/shannon-fano.hpp"
#include "y/information/translator/queue.hpp"
#include "y/information/translator/encoding-tree.hpp"

namespace upsylon {

    namespace Information {

        namespace ShannonFano {

            //! use Tree to build Encoder base class
            class  EncodingTree : public EncodingWith<Tree>
            {
            public:
                static const char CLID[];                   //!< ShannonFano-Encoder
                explicit EncodingTree();                    //!< setup
                virtual ~EncodingTree() throw();            //!< cleanup
                virtual const char *label() const throw();  //!< CLID

            private:
                Y_DISABLE_COPY_AND_ASSIGN(EncodingTree);
            };

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
                IOBits Q;
            };

        }

    }
}

#endif

