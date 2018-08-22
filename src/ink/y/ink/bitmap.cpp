#include "y/ink/bitmap.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace ink
    {
        bitmap:: ~bitmap() throw()
        {
            switch(model)
            {
                case memory_is_global: break;
                case memory_from_user: break;
                case memory_is_shared:
                    if(shared->liberate())
                    {
                        delete shared;
                    }
                    break;
            }
            memory::global::location().release(private_memory,private_length);
        }
        
        static inline
        unit_t __check(const unit_t v, const char *id)
        {
            assert(id);
            if(v<=0) throw exception("ink.bitmap(%s<=0)", id);
            return v;
        }
        
        bitmap:: bitmap(const unit_t D, const unit_t W, const unit_t H) :
        area( coord(0,0), coord(W,H), area_sizes ),
        entry(0),
        rows(0),
        w(     __check(W,"width")  ),
        h(     __check(H,"height") ),
        depth( __check(D,"depth")  ),
        scanline( w * depth ),
        stride( scanline ),
        bytes(pixels*depth),
        shared(0),
        private_memory(0),
        private_length(0),
        model( memory_is_global )
        {
            allocate();
        }
        
        static inline bitmap *__check(bitmap *bmp)
        {
            return bmp;
        }
        
        bitmap:: bitmap( bitmap *bmp ) :
        area( *__check(bmp) ),
        entry( bmp->entry   ),
        rows(0),
        w( bmp->w ),
        h( bmp->h ),
        depth( bmp->depth ),
        scanline( bmp->scanline ),
        stride(   bmp->stride   ),
        bytes(    bmp->bytes    ),
        shared( bmp ),
        private_memory(0),
        private_length(0),
        model( memory_is_shared )
        {
            shared->withhold();
            try
            {
                allocate_rows_only();
            }
            catch(...)
            {
                if(shared->liberate())
                {
                    delete shared;
                }
                throw;
            }
        }
        
        
        
        void bitmap:: allocate()
        {
            const size_t data_offset = 0;
            const size_t data_length = bytes;
            const size_t rows_offset = memory::align(data_length+data_offset);
            const size_t rows_length = h * sizeof(row_layout);
            
            private_length = memory::align(rows_offset+rows_length);
            private_memory = memory::global::instance().acquire(private_length);
            
            entry = private_memory;
            rows  = memory::io::cast<void>(entry,rows_offset);
            
            link_rows();
        }
        
        void bitmap:: allocate_rows_only()
        {
            assert(entry);
            const size_t rows_offset = 0;
            const size_t rows_length = h * sizeof(row_layout);
            
            private_length = memory::align(rows_offset+rows_length);
            private_memory = memory::global::instance().acquire(private_length);
            rows  = private_memory;
            
            link_rows();
        }
        
        void bitmap:: link_rows() throw()
        {
            assert(rows);
            assert(entry);
            uint8_t    *p = (uint8_t    *)entry;
            row_layout *r = (row_layout *)rows;
            for(unit_t j=0;j<h;++j)
            {
                r->p = p;
                r->w = w;
                ++r;
                p += stride;
            }
        }
        
        bitmap:: bitmap( const bitmap &bmp ) :
        area( bmp ),
        counted(),
        entry(0),
        rows(0),
        w( bmp.w ),
        h( bmp.h ),
        depth(    bmp.depth    ),
        scanline( bmp.scanline ),
        stride(   bmp.depth    ),
        bytes(    bmp.bytes    ),
        shared(   bmp.shared   ),
        private_memory(0),
        private_length(0),
        model(    bmp.model    )
        {
            switch(model)
            {
                case memory_is_global:
                    break;
                    
                case memory_is_shared:
                    break;
                    
                case memory_from_user:
                    break;
            }
        }
        
        
        
    }
}

