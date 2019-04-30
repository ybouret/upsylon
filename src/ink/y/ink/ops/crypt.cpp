
#include "y/ink/ops/crypt.hpp"
#include "y/memory/pooled.hpp"

namespace upsylon
{
    namespace Ink
    {

        void Crypt::Run(Bitmap &tgt, const Bitmap &src, crypto::kstream &ks) throw()
        {
            assert(tgt.h==src.h);
            assert(tgt.w==src.w);
            assert(tgt.depth==src.depth);

            const size_t h = tgt.h;
            const size_t n = tgt.scanline;
            for(size_t j=0;j<h;++j)
            {
                const uint8_t *s = static_cast<const uint8_t *>( src.get_line(j) );
                uint8_t       *t = static_cast<uint8_t       *>( tgt.get_line(j) );
                for(size_t i=n;i>0;--i)
                {
                    *(t++) = ks( *(s++) );
                }
            }
        }

        typedef point2d<size_t>              mark;
        typedef vector<mark,memory::pooled>  marks;

        static inline
        void transfer_to(Bitmap            &tgt,
                         const digest      &oblock,
                         const array<mark> &output)
        {
            assert( output.size() <= oblock.size );
            for(size_t k=output.size();k>0;)
            {
                const mark    p = output[k--];
                const uint8_t b = oblock[k];
                uint8_t   *t = static_cast<uint8_t *>(tgt.get_line(p.y));
                t[p.x]       = b;
            }
        }

        void Crypt:: Run( Bitmap &tgt, const Bitmap &src, crypto::operating &op, crypto::ciphers &c )
        {
            assert(tgt.h    ==src.h    );
            assert(tgt.w    ==src.w    );
            assert(tgt.depth==src.depth);

            const size_t block_size = c.block_size;
            const size_t h          = tgt.h;
            const size_t n          = tgt.scanline;
            digest            iblock(block_size);
            digest            oblock(block_size);
            marks             output(block_size,as_capacity);
            size_t            count      = 0;

            assert(0==output.size());
            for(size_t j=0;j<h;++j)
            {
                const uint8_t *s = static_cast<const uint8_t *>( src.get_line(j) );
                for(size_t i=0;i<n;++i)
                {
                    assert(count<block_size);
                    assert(count==output.size());

                    iblock[count++] = s[i];
                    const mark m(i,j);
                    output.push_back(m);
                    if(count>=block_size)
                    {
                        assert(block_size==count);
                        op.write_block(c,oblock.rw(),iblock.ro());
                        transfer_to(tgt, oblock, output);
                        count=0;
                        output.free();
                    }
                }
            }

            {
                assert(count==output.size());
                op.flush_block(c,oblock.rw(),iblock.ro(),count);
                transfer_to(tgt, oblock, output);
            }

        }


    }
}

