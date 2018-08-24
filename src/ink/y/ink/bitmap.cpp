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
        unit_t __check(const size_t v, const char *id)
        {
            assert(id);
            if(v<=0) throw exception("ink.bitmap(%s<=0)", id);
            return v;
        }
        
        bitmap:: bitmap(const size_t D, const size_t W, const size_t H) :
        area( coord(0,0),__check(W,"width"),__check(H,"height") ),
        entry(0),
        rows(0),
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
        depth(    bmp->depth    ),
        scanline( bmp->scanline ),
        stride(   bmp->stride   ),
        bytes(    bmp->bytes    ),
        shared(   bmp           ),
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
        
        bitmap:: bitmap(const void  *data,
                        const size_t D,
                        const size_t W,
                        const size_t H,
                        const size_t S) :
        area( coord(0,0),__check(W,"width"),__check(H,"height") ),
        entry((void*)data),
        rows(0),
        depth( __check(D,"depth")  ),
        scanline( w * depth ),
        stride( S ),
        bytes(pixels*depth),
        shared(0),
        private_memory(0),
        private_length(0),
        model( memory_from_user )
        {
            if( stride < scanline ) throw exception("bitmap from user data: invalid stride!");
            allocate_rows_only();
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
            assert(stride>=scanline);
            uint8_t    *p = (uint8_t    *)entry;
            row_layout *r = (row_layout *)rows;
            for(size_t j=0;j<h;++j)
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
                    allocate();
                    memcpy(entry,bmp.entry,bytes);
                    break;
                    
                case memory_is_shared:
                    assert(shared);
                    shared->withhold();
                    allocate_rows_only();
                    break;
                    
                case memory_from_user:
                    allocate_rows_only();
                    break;
            }
        }
        
        void bitmap:: copy(const bitmap &other) throw()
        {
            if(this!=&other)
            {
                assert(h==other.h);
                assert(scanline==other.scanline);
                const size_t n = scanline;
                for(size_t j=0;j<h;++j)
                {
                    memcpy(get_line(j),other.get_line(j),n);
                }
            }
        }

        
    }
}

