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


    string miniLZO::Compress(const string &source) const
    {
        HEAP_ALLOC(wrkmem, LZO1X_1_MEM_COMPRESS);
        const lzo_uint IN_LEN  = source.length();
        lzo_uint       OUT_LEN = miniLZO_out_len(IN_LEN);
        miniLZO_Buffer target(OUT_LEN);

        const int r = lzo1x_1_compress((const unsigned char*)source.ro(),IN_LEN,
                                       (unsigned char      *)target.rw(),&OUT_LEN,wrkmem);
        if(r != LZO_E_OK )
        {
            throw exception("LZO::Compress internal failure");
        }
        std::cerr << "Compressed " << IN_LEN << " into " << OUT_LEN << std::endl;
        
        return string(*target,OUT_LEN);
    }

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
        size_t shift = 0;
        fp.emit_upack(out_len,&shift);
        fp.output(*target,out_len);
        std::cerr << "Compressed " << in_len << " into " << out_len << "+" << shift << std::endl;
    }


    string miniLZO:: Decompress(const size_t size, const string &source) const
    {
        const lzo_uint       src_len = source.size();
        const unsigned char *src     = (const unsigned char *) *source;
        lzo_uint             tgt_len = size;
        miniLZO_Buffer       target(tgt_len);
        unsigned char       *tgt     = (unsigned char *) target.rw();
        lzo_uint             new_len = tgt_len;
        if( LZO_E_OK != lzo1x_decompress(src, src_len, tgt, &new_len, NULL) || new_len != tgt_len )
        {
            throw exception("LZO::Decompress corrupted data");
        }
        std::cerr << "Decompressed " << src_len << " into " << tgt_len << std::endl;
        return string(*target,tgt_len);
    }


    void Decompress(void       *output, const size_t outlen,
                    const void *input,  const size_t inlen)
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
    

}

