#include "y/ink/bitmap.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Ink
    {
        Bitmap:: ~Bitmap() throw()
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
        
        Bitmap:: Bitmap(const size_t D, const size_t W, const size_t H) :
        Area( coord(0,0),__check(W,"width"),__check(H,"height") ),
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
        
        static inline Bitmap *__check(Bitmap *bmp)
        {
            return bmp;
        }
        
        Bitmap:: Bitmap( Bitmap *bmp ) :
        Area( *__check(bmp) ),
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
        
        Bitmap:: Bitmap(const void  *data,
                        const size_t D,
                        const size_t W,
                        const size_t H,
                        const size_t S) :
        Area( coord(0,0),__check(W,"width"),__check(H,"height") ),
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


        void Bitmap:: allocate()
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
        
        void Bitmap:: allocate_rows_only()
        {
            assert(entry);
            const size_t rows_offset = 0;
            const size_t rows_length = h * sizeof(row_layout);
            
            private_length = memory::align(rows_offset+rows_length);
            private_memory = memory::global::instance().acquire(private_length);
            rows  = private_memory;
            
            link_rows();
        }
        
        void Bitmap:: link_rows() throw()
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
        
        Bitmap:: Bitmap( const Bitmap &bmp ) :
        Area( bmp ),
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
        
        void Bitmap:: copy(const Bitmap &other) throw()
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

        void Bitmap:: __signature( hashing::function &H ) const throw()
        {
            H.set();
            for(size_t j=0;j<h;++j)
            {
                H.run(get_line(j),scanline);
            }
        }


        namespace
        {
            struct copyOps
            {
                Bitmap       *target;
                const Bitmap *source;
                inline void operator()( const Area &zone, lockable & ) throw()
                {
                    assert(target);
                    assert(source);
                    assert(target->contains(zone));
                    assert(source->contains(zone));
                    if(zone.pixels)
                    {
                        const size_t bytes_per_line = target->depth * zone.w;
                        const unit_t jlo = zone.lower.y;
                        const unit_t ilo = zone.lower.x;
                        for(unit_t j=zone.upper.y;j>=jlo;--j)
                        {
                            memcpy(target->get(ilo,j),source->get(ilo,j),bytes_per_line);
                        }
                    }

                }
            };
        }

        void Bitmap:: copy(const Bitmap &other, Engine &E)
        {
            if(this!=&other)
            {
                assert(h==other.h);
                assert(scanline==other.scanline);
                copyOps ops = { this, &other };
                E.run(ops);
            }
        }

        
    }
}

