
//!\file

#ifndef Y_INFORMATION_TRANSLATOR_ENCODING_TREE_INCLUDED
#define Y_INFORMATION_TRANSLATOR_ENCODING_TREE_INCLUDED 1

#include "y/information/qencoder.hpp"

namespace upsylon {

    namespace Information {

        //! wrap operations
        template <typename TREE>
        class EncodingWith : public TREE, public QEncoder
        {
        public:
            inline explicit EncodingWith() : TREE(), QEncoder() {}
            inline virtual ~EncodingWith() throw() {}

            //! will setup internal tree
            inline virtual void startBits() throw()
            {
                this->setupTree();
            }

            //! will emit byte and update tree
            inline virtual void writeBits(IOBits &Q, const char C)
            {
                this->inputByte(C,&Q);
            }

            //! emit EOS
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
