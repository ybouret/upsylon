//!\file

#ifndef Y_INFORMATION_PACK_INCLUDED
#define Y_INFORMATION_PACK_INCLUDED 1

#include "y/information/shift/mtf.hpp"
#include "y/information/filter/rle/encoder.hpp"

namespace upsylon {

    namespace information {


        namespace Pack  {

            //! encoder
            class Encoder : public filterQ
            {
            public:
                explicit Encoder(const size_t bs); //!< setup block size
                virtual ~Encoder() throw();        //!< cleanup

                const size_t blockSize; //!< blockSize
                const size_t wordBytes; //!< bytes to write count/primary index
                
                virtual void reset() throw(); //!< reset all
                virtual void write(char C);   //!< write to fill/emit
                virtual void flush();         //!< emit

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

