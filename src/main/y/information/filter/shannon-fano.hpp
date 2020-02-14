
//! \file
#ifndef Y_INFORMATION_SHANNON_FANO_INCLUDED
#define Y_INFORMATION_SHANNON_FANO_INCLUDED 1

#include "y/information/qbits.hpp"
#include "y/information/filter/alphabet.hpp"

namespace upsylon {

    namespace information {

        namespace ShannonFano {
            
            //! context based on alphabet, used to build tree
            class Context : public Alphabet
            {
            public:
                virtual ~Context() throw();   //!< cleanup
                explicit Context(const Mode); //!< setup

            protected:
                Node   *root;
                Node   *nstk;
                
                void  update( Node *node ) throw();

                void split(Node *hi, Node *lo);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Context);

            };


        }



    }


}

#endif
