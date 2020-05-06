//! \file

#ifndef Y_SPADE_FIELD2D_INCLUDED
#define Y_SPADE_FIELD2D_INCLUDED 1

#include "y/spade/field1d.hpp"

namespace upsylon {
    
    namespace Spade {
        
        //! forward declaration
        template <typename T> class Field3D;
        
        //----------------------------------------------------------------------
        //
        //! 2D fields
        //
        //----------------------------------------------------------------------
        template <typename T>
        class Field2D : public Field<T>, public Layout2D
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            Y_DECL_ARGS(T,type);                                    //!< aliaes
            typedef            Layout2D                LayoutType;  //!< alias
            typedef typename   LayoutType::coord       coord;       //!< alias
            typedef typename   LayoutType::const_coord const_coord; //!< alias
            typedef            Layout1D                RowLayout;   //!< alias
            typedef            Field1D<T>              Row;         //!< alias
            
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            
            //! cleanup
            inline virtual ~Field2D() throw() { rows += lower.y; clear(); }
            
            //! setup with internal memory
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
                const size_t rowLength = width.y * sizeof(Row);
                const size_t objOffset = memory::align(rowOffset+rowLength);
                const size_t objLength = items*sizeof(T);
                char        *p         = static_cast<char *>(this->allocate( memory::align(objLength+objOffset) ) );
                build(p+rowOffset,p+objOffset);
            }
            
            //------------------------------------------------------------------
            //
            // virtual interface
            //
            //------------------------------------------------------------------
          
            //! return metrics
            inline virtual const LayoutMetrics & metrics() const throw() { return *this; }
            
            //------------------------------------------------------------------
            //
            // non virtual interface
            //
            //------------------------------------------------------------------
            
            //! row access
            inline Row & operator[](const Coord1D j) throw()
            {
                assert(j>=lower.y); assert(j<=upper.y);
                return rows[j];
            }
            
            //! row access, const
            inline const Row & operator[](const Coord1D j) const throw()
            {
                assert(j>=lower.y); assert(j<=upper.y);
                return rows[j];
            }
            
            //! direct access
            inline type & operator[](const_coord c) throw()
            {
                assert(this->has(c));
                return (*this)[c.y][c.x];
            }
            
            //! direct access, const
            inline const_type & operator[](const_coord c) const throw()
            {
                assert(this->has(c));
                return (*this)[c.y][c.x];
            }
            
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            
            const RowLayout rowLayout; //!< shared layout for all rows
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Field2D);
            friend class Field3D<T>;
            
            Row     *rows;
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
                    rows               = static_cast<Row          *>(rowAddr);
                    mutable_type *objs = static_cast<mutable_type *>(objAddr);
                    Coord1D       indx = lower.y;
                    while(built<nrow)
                    {
                        const string id = this->name + Kernel::Field::Suffix(indx);
                        new (rows+built) Row(id,rowLayout,objs);
                        ++built;
                        ++indx;
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
            
            
            //! setup with internal memory
            template <typename LABEL> inline
            explicit Field2D(const LABEL      &id,
                             const LayoutType &L,
                             void             *rowAddr,
                             void             *objAddr) :
            Field<T>(id),
            LayoutType(L),
            rowLayout(lower.x,upper.x),
            rows(0),
            built(0)
            {
                build(rowAddr,objAddr);
            }
            
            
        };
        
    }
    
}
#endif

