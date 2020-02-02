#include "y/codec/byte-editor.hpp"


namespace upsylon {

    byte_editor::  byte_editor() throw() {}

    byte_editor:: ~byte_editor() throw() {}

    void byte_editor:: encode(void *output,const void *input, size_t length) throw()
    {
        assert(!(0==output&&length>0));
        assert(!(0==input &&length>0));
        uint8_t       *tgt = (uint8_t *)output;
        const uint8_t *src = (const uint8_t *)input;
        for(size_t i=0;i<length;++i)
        {
            *(tgt++) = encode( *(src++) );
        }
    }

    void byte_editor:: decode(void *output,const void *input, size_t length) throw()
    {
        assert(!(0==output&&length>0));
        assert(!(0==input &&length>0));
        uint8_t       *tgt = (uint8_t *)output;
        const uint8_t *src = (const uint8_t *)input;
        for(size_t i=0;i<length;++i)
        {
            *(tgt++) = decode( *(src++) );
        }
    }



}
