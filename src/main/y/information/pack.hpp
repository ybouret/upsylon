//!\file

#ifndef Y_INFORMATION_PACK_INCLUDED
#define Y_INFORMATION_PACK_INCLUDED 1

#include "y/information/mtf.hpp"
#include "y/information/filter/rle/encoder.hpp"

namespace upsylon {

    namespace information {


        namespace Pack  {


            class Encoder : public filterQ
            {
            public:
                explicit Encoder(const size_t bs);
                virtual ~Encoder() throw();

                const size_t blockSize;
                const size_t wordBytes;
                
                virtual void reset() throw();
                virtual void write(char C);
                virtual void flush();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Encoder);
                size_t       count;   //!< current loaded bytes
                char        *input;   //!< blockSize
                char        *output;  //!< blockSize
                size_t      *indices; //!< blockSize
                mtf_encoder *mtf;
                void        *wksp;
                size_t       wlen;
                RLE::Encoder rle;
                
                void emit();
                void emitSize(size_t sz);
            };
        };



    }

}

#endif

