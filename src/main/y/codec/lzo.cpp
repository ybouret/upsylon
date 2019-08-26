#include "y/codec/lzo.hpp"
#include "y/codec/lzo/minilzo.h"
#include "y/memory/buffers.hpp"
#include "y/exception.hpp"

namespace upsylon
{


    miniLZO:: ~miniLZO() throw()
    {
    }

    miniLZO:: miniLZO()
    {
         if (lzo_init() != LZO_E_OK)
         {
             throw exception("lzo_init failure");
         }
    }

    const char * miniLZO:: VersionString() const throw()
    {
        return lzo_version_string();
    }

    const char * miniLZO:: VersionDate() const throw()
    {
        return lzo_version_date();
    }


#define HEAP_ALLOC(var,size) \
lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t) ]



    static inline lzo_uint miniLZO_out_len(const lzo_uint in_len)
    {
        return in_len + Y_ROUND16(in_len)/16 + 64 + 3;
    }

    typedef memory::global_buffer_of<char> miniLZO_Buffer;

    
    void miniLZO:: Compress( ios::ostream &fp, const void *data, const size_t size)
    {
        assert(!(NULL==data&&size>0));
        HEAP_ALLOC(wrkmem, LZO1X_1_MEM_COMPRESS);
        const lzo_uint in_len  = size;
        lzo_uint       out_len = miniLZO_out_len(in_len);
        miniLZO_Buffer target(out_len);

        const int r = lzo1x_1_compress((const unsigned char*)data,in_len,
                                       (unsigned char      *)target.rw(),&out_len,wrkmem);
        if(r != LZO_E_OK )
        {
            throw exception("LZO::Compress internal failure");
        }
        size_t extra = 0;
        {
            size_t shift = 0;
            fp.emit_upack<size_t>(size,&shift);
            extra += shift;
        }
        {
            size_t shift = 0;
            fp.emit_upack<size_t>(out_len,&shift);
            fp.output(*target,out_len);
            extra += shift;
        }
        std::cerr << "Compressed " << in_len << " into " << out_len << "+" << extra << std::endl;
    }


    void miniLZO:: Decompress(void       *output, const size_t outlen,
                              const void *input,  const size_t inlen) const
    {
        const lzo_uint       src_len = inlen;
        const unsigned char *src     = (const unsigned char*)input;
        lzo_uint             tgt_len = outlen;
        unsigned char       *tgt     = (unsigned char *)output;
        lzo_uint             new_len = tgt_len;
        if( LZO_E_OK != lzo1x_decompress(src, src_len, tgt, &new_len, NULL) || new_len != tgt_len )
        {
            throw exception("LZO::Decompress corrupted data");
        }
    }


    string miniLZO:: Decompress( ios::istream &fp ) const
    {
        const size_t     outlen = fp.read_upack<size_t>();
        const size_t     inlen = fp.read_upack<size_t>();
        miniLZO_Buffer   input(inlen);
        fp.input(*input,inlen);

        string ans(outlen,as_capacity,false);
        Decompress(*ans,outlen,*input,inlen);
        ans.force(outlen);
        return ans;
    }


}

