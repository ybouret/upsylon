//! \file

#ifndef Y_SPADE_FIELD2D_INCLUDED
#define Y_SPADE_FIELD2D_INCLUDED 1

#include "y/spade/field/in1d.hpp"

namespace upsylon {
    
    namespace Spade {
        
        //! forward declaration
        template <typename T> class Field3D;

        //! common constructor part
#define Y_FIELD2D_CTOR()  rowLayout(lower.x,upper.x), row(0), built(0)

        //----------------------------------------------------------------------
        //
        //! 2D fields
        //
        //----------------------------------------------------------------------
        template <typename T>
        class Field2D : public FieldOf<T>, public Layout2D
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
            typedef            arc_ptr<Field2D>        Handle;      //!< alias
            
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            
            //! cleanup
            inline virtual ~Field2D() throw() { row += lower.y; clear(); }
            
            //! setup with internal memory
            template <typename LABEL> inline
            explicit Field2D(const LABEL      &id,
                             const LayoutType &L) :
            FieldOf<T>(id), LayoutType(L), Y_FIELD2D_CTOR()
            {
                create();
            }
            
            //! setup with internal memory, using coords
            template <typename LABEL> inline
            explicit Field2D(const LABEL      &id,
                             const_coord       lo,
                             const_coord       hi) :
            FieldOf<T>(id), LayoutType(lo,hi), Y_FIELD2D_CTOR()
            {
                create();
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
                return row[j];
            }
            
            //! row access, const
            inline const Row & operator[](const Coord1D j) const throw()
            {
                assert(j>=lower.y); assert(j<=upper.y);
                return row[j];
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
            Row          *row;
            size_t        built;
            

        private:
            inline void clear() throw()
            {
                assert(row);
                while(built>0)
                {
                    self_destruct(row[--built]);
                }
                row=0;
            }
            
            inline void create()
            {
                const size_t rowOffset = 0;
                const size_t rowLength = width.y * sizeof(Row);
                const size_t objOffset = memory::align(rowOffset+rowLength);
                const size_t objLength = items*sizeof(T);
                char        *p         = static_cast<char *>(this->allocate( memory::align(objLength+objOffset) ) );
                build(p+rowOffset,p+objOffset);
            }
            
            inline void build(void *rowAddr, void *objAddr)
            {
                assert(rowAddr);
                assert(objAddr);
                try {
                    const size_t  nrow = size_t(width.y);
                    const size_t  skip = rowLayout.items;
                    row                = static_cast<Row          *>(rowAddr);
                    this->addr         = static_cast<mutable_type *>(objAddr);
                    mutable_type *objs = this->addr;
                    Coord1D       indx = lower.y;
                    while(built<nrow)
                    {
                        const string id = this->name + Field::Suffix(indx);
                        new (row+built) Row(id,rowLayout,objs);
                        ++built;
                        ++indx;
                        objs+=skip;
                    }
                    row  -= lower.y;
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
            FieldOf<T>(id), LayoutType(L), Y_FIELD2D_CTOR()
            {
                build(rowAddr,objAddr);
            }
            
            inline virtual const void *getObjectByCoord(const Coord1D *c) const throw()
            {
                assert(c);
                const_coord C(c[0],c[1]);
                assert(this->has(C));
                return &row[C.y][C.x];
            }
            
        };
        
    }
    
}
#endif

