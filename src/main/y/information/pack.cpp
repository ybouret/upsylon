#include "y/information/pack.hpp"
#include "y/memory/embed.hpp"
#include "y/information/bwt.hpp"

namespace upsylon {

    namespace information {

        namespace Pack {

            Encoder:: Encoder( const size_t bs ) :
            filterQ(bs+16),
            blockSize(bs),
            wordBytes( bytes_for(blockSize) ),
            count(0),
            input(0),
            output(0),
            indices(0),
            mtf(0),
            wksp(0),
            wlen(0),
            rle()
            {
                memory::embed emb[] =
                {
                    memory::embed::as(input,  blockSize),
                    memory::embed::as(output, blockSize),
                    memory::embed::as(indices,blockSize),
                    memory::embed( (void **)&mtf, sizeof(mtf_encoder))
                };
                wksp = memory::embed::create_global(emb, sizeof(emb)/sizeof(emb[0]), wlen);
                new (mtf) mtf_encoder();
            }

            Encoder:: ~Encoder() throw()
            {
                memset(wksp,0,wlen);
                count = 0;
                memory::global::location().release(wksp,wlen);
            }

            void Encoder:: reset() throw()
            {
                this->free();
                count = 0;
                mtf->reset();
            }

            void Encoder:: write(char C)
            {
                assert(count<blockSize);
                input[count++] = C;
                if(count>=blockSize)
                {
                    emit();
                }
            }

            void Encoder::emitSize(   size_t sz )
            {
                assert(sz<=blockSize);
                for(size_t i=0;i<wordBytes;++i)
                {
                    push_back( uint8_t(sz) );
                    sz >>= 8;
                }
                assert(0==sz);


            }

            void Encoder:: flush()
            {
                if(count>0)
                {
                    emit();
                }
            }

            void Encoder:: emit()
            {
                assert(count>0);

                const size_t pidx = bwt::encode(output, input, count, indices, *mtf);
                emitSize(count);
                emitSize(pidx);

                rle.reset(); assert(0==rle.size());
                for(size_t i=0;i<count;++i)
                {
                    rle.write(output[i]);
                }
                rle.flush();
                this->merge_back( rle );
                //put_all(output,count);
                count = 0;
            }


        }



    }

}
