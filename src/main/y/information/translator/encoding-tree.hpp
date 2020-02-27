
//!\file

#ifndef Y_INFORMATION_TRANSLATOR_ENCODING_TREE_INCLUDED
#define Y_INFORMATION_TRANSLATOR_ENCODING_TREE_INCLUDED 1

#include "y/information/qencoder.hpp"

namespace upsylon {

    namespace Information {

        template <typename TREE>
        class EncodingWith : public TREE, public qencoder
        {
        public:
            inline explicit EncodingWith() : TREE(), qencoder() {}
            inline virtual ~EncodingWith() throw() {}

            inline virtual void startBits() throw()
            {
                this->setupTree();
            }

            inline virtual void writeBits(IOBits &Q, const char C)
            {
                this->inputByte(C,&Q);
            }

            inline virtual void flushBits(IOBits &Q)
            {
                this->eos->emit(Q);
                Q.zpad();
            }
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(EncodingWith);
        };


    }
}

#endif
