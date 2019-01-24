//! \file
#ifndef Y_INK_BITMAP_INCLUDED
#define Y_INK_BITMAP_INCLUDED 1

#include "y/ink/engine.hpp"
#include "y/ptr/counted.hpp"
#include "y/hashing/function.hpp"

namespace upsylon
{
    namespace Ink
    {
     
        //! type agnostic bitmap
        class Bitmap : public Area, public counted
        {
        public:
            //! different kind of bitmap
            enum memory_model
            {
                memory_is_global, //!< fully allocated
                memory_is_shared, //!< shared bitmap
                memory_from_user  //!< memory from user
            };
            
            void        *entry;    //!< address of first pixel
        protected:
            void        *rows;     //!< address of first row
        public:
            const size_t depth;    //!< bytes per pixel > 0
            const size_t scanline; //!< w*depth
            const size_t stride;   //!< stride>=scanline
            const size_t bytes;    //!< pixels*depth
            
            //! memory_is_global bitmap
            explicit Bitmap(const size_t D,const size_t W,const size_t H);
            
            //! memory_is_shared bitmap, same layouts
            explicit Bitmap( Bitmap *bmp );

            //! memory from user
            explicit Bitmap(const void  *data,
                            const size_t D,
                            const size_t W,
                            const size_t H,
                            const size_t S);

            //! copy with same memory
            Bitmap( const Bitmap &bmp );
            
            //! release private memory
            virtual ~Bitmap() throw();

            //! get line address
            inline void *get_line(const size_t j) throw()
            {
                assert(j<h);
                return (static_cast<row_layout *>(rows)+j)->p;
            }

            //! get line address, const
            inline const void *get_line(const size_t j) const throw()
            {
                assert(j<h);
                return (static_cast<row_layout *>(rows)+j)->p;
            }

            //! get pixel address
            inline void *get(const size_t i, const size_t j) throw()
            {
                assert(i<w);
                assert(j<h);
                uint8_t *p = (static_cast<row_layout *>(rows)+j)->p;
                return &p[i*depth];
            }

            //! get pixel address, const
            inline const void *get(const size_t i, const size_t j) const throw()
            {
                assert(i<w);
                assert(j<h);
                const uint8_t *p = (static_cast<row_layout *>(rows)+j)->p;
                return &p[i*depth];
            }

            //! hard copy
            void copy(const Bitmap &other) throw();
            //! hard copy with tiles
            void copy(const Bitmap &other, Engine &E);

            //! H.set(), H.run(on all data)
            void __signature( hashing::function &H ) const throw();
            
            //! get a hash
            template <typename T>
            T __hash( hashing::function &H ) const throw()
            {
                __signature(H);
                return H.key<T>();
            }

            inline void zset() throw()
            {
                const size_t n = scanline;
                for(size_t j=0;j<h;++j)
                {
                    memset((static_cast<row_layout *>(rows)+j)->p,0,n);
                }
            }

        private:
            Y_DISABLE_ASSIGN(Bitmap);
            //! layout of a pixmap row
            struct row_layout { uint8_t *p; size_t w; };
            
            Bitmap *shared;
            void   *private_memory;
            size_t  private_length;
            
            void allocate();
            void allocate_rows_only();
            void link_rows() throw();
            
        public:
            const memory_model model; //!< memory model for this bitmap
        };
    }
}

#endif

