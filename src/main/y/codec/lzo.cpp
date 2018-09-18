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



    string miniLZO::Compress(const string &source) const
    {
        HEAP_ALLOC(wrkmem, LZO1X_1_MEM_COMPRESS);
        const lzo_uint IN_LEN  = source.length();
        lzo_uint       OUT_LEN = (IN_LEN + IN_LEN / 16 + 64 + 3);
        memory::global_buffer_of<char> target(OUT_LEN);

        const int r = lzo1x_1_compress((const unsigned char*)source.ro(),IN_LEN,
                                       (unsigned char      *)target.rw(),&OUT_LEN,wrkmem);
        if(r != LZO_E_OK )
        {
            throw exception("LZO::Compress internal failure");
        }
        std::cerr << "Compressed " << IN_LEN << " into " << OUT_LEN << std::endl;

        return string(*target,OUT_LEN);
    }

    string miniLZO:: Decompress(const string &source) const
    {

        return string();
    }

}

