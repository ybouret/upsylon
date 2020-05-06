//! \file

#ifndef Y_SPADE_FIELD2D_INCLUDED
#define Y_SPADE_FIELD2D_INCLUDED 1

#include "y/spade/field1d.hpp"

namespace upsylon {
    
    namespace Spade {
        
        template <typename T>
        class Field2D : public Field<T>, public Layout2D
        {
        public:
            Y_DECL_ARGS(T,type);
            typedef Layout2D   LayoutType;
            typedef typename   LayoutType::coord       coord;
            typedef typename   LayoutType::const_coord const_coord;
            typedef Layout1D   RowLayout;
            typedef Field1D<T> RowType;
            
            inline virtual ~Field2D() throw() { rows += lower.y; clear(); }
            
            template <typename LABEL> inline
            explicit Field2D(const LABEL      &id,
                             const LayoutType &L) :
            Field<T>(id),
            LayoutType(L),
            rowLayout(lower.x,upper.x),
            rows(0),
            built(0)
            {
                const size_t rowOffset = 0;
                const size_t rowLength = width.y * sizeof(RowType);
                const size_t objOffset = memory::align(rowOffset+rowLength);
                const size_t objLength = items*sizeof(T);
                char        *p         = static_cast<char *>(this->allocate( memory::align(objLength+objOffset) ) );
                build(p+rowOffset,p+objOffset);
            }
            
            inline virtual const LayoutMetrics & metrics() const throw() { return *this; }
            
            inline RowType & operator[](const Coord1D j) throw()
            {
                assert(j>=lower.y); assert(j<=upper.y);
                return rows[j];
            }
            
            inline const RowType & operator[](const Coord1D j) const throw()
            {
                assert(j>=lower.y); assert(j<=upper.y);
                return rows[j];
            }
            
            inline type & operator[](const_coord c) throw()
            {
                assert(this->has(c));
                return (*this)[c.y][c.x];
            }
            
            inline const_type & operator[](const_coord c) const throw()
            {
                assert(this->has(c));
                return (*this)[c.y][c.x];
            }
            
            
            const RowLayout rowLayout;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Field2D);
            RowType *rows;
            size_t   built;
            
            inline void clear() throw()
            {
                assert(rows);
                while(built>0)
                {
                    self_destruct(rows[--built]);
                }
            }
            
            inline void build(void *rowAddr, void *objAddr)
            {
                assert(rowAddr);
                assert(objAddr);
                try {
                    const size_t  nrow = size_t(width.y);
                    const size_t  skip = rowLayout.items;
                    rows               = static_cast<RowType      *>(rowAddr);
                    mutable_type *objs = static_cast<mutable_type *>(objAddr);
                    while(built<nrow)
                    {
                        new (rows+built) RowType(this->name,rowLayout,objs);
                        ++built;
                        objs+=skip;
                    }
                    rows -= lower.y;
                }
                catch(...)
                {
                    clear();
                    throw;
                }
            }
            
            
        };
        
    }
    
}
#endif

