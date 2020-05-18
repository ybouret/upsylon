//! \file

#ifndef Y_SPADE_FIELD3D_INCLUDED
#define Y_SPADE_FIELD3D_INCLUDED 1

#include "y/spade/field2d.hpp"
#include "y/spade/layout.hpp"

namespace upsylon {
    
    namespace Spade {
        
        //----------------------------------------------------------------------
        //
        //! 3D fields
        //
        //----------------------------------------------------------------------
        template <typename T>
        class Field3D : public Field<T>, public Layout3D
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            Y_DECL_ARGS(T,type);                                    //!< aliaes
            typedef            Layout3D                LayoutType;  //!< alias
            typedef typename   LayoutType::coord       coord;       //!< alias
            typedef typename   LayoutType::const_coord const_coord; //!< alias
            typedef            Layout2D                SliceLayout; //!< alias
            typedef            Field2D<T>              Slice;       //!< alias
            typedef            Layout1D                RowLayout;   //!< alias
            typedef            Field1D<T>              Row;         //!< alias
            typedef            arc_ptr<Field3D>        Handle;      //!< alias

            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            
            //! cleanup
            inline virtual ~Field3D() throw() { slices += lower.z; clear(); }
            
            
            //! setup with internal memory
            template <typename LABEL> inline
            explicit Field3D(const LABEL      &id,
                             const LayoutType &L) :
            Field<T>(id),
            LayoutType(L),
            sliceLayout(lower.xy(),upper.xy()),
            rowLayout(lower.x,lower.y),
            slices(0),
            built(0)
            {
                const size_t nslice       = size_t(width.z);   // number of slices
                const size_t rps          = size_t(width.y);   // rows per slice
                const size_t ops          = sliceLayout.items; // objects per slice
                const size_t sliceOffset  = 0;
                const size_t sliceLength  = nslice * sizeof(Slice);
                const size_t rowOffset    = memory::align(sliceOffset+sliceLength);
                const size_t rowLength    = nslice * rps * sizeof(Row);
                const size_t objOffset    = memory::align(rowOffset+rowLength);
                const size_t objLength    = items * sizeof(T);
                char         *p           = static_cast<char *>( this->allocate( memory::align(objOffset+objLength) ) );
                slices                    = (Slice        *) &p[sliceOffset];
                Row          *rows        = (Row          *) &p[rowOffset];
                mutable_type *objs        = (mutable_type *) &p[objOffset];
                this->addr = objs;
                try {
                    Coord1D indx = lower.z;
                    while(built<nslice)
                    {
                        const string sid = this->name + Kernel::Field::Suffix(indx);
                        new (slices+built) Slice(sid,sliceLayout,rows,objs);
                        rows += rps;
                        objs += ops;
                        ++built;
                    }
                }
                catch(...)
                {
                    clear();
                    throw;
                }
                slices -= lower.z;
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
            
            //! slice access
            Slice & operator[](const Coord1D k) throw()
            {
                assert(k>=lower.z); assert(k<=upper.z);
                return slices[k];
            }
            
            //! slice access, const
            const Slice & operator[](const Coord1D k) const throw()
            {
                assert(k>=lower.z); assert(k<=upper.z);
                return slices[k];
            }
            
            //! type access
            type & operator[](const_coord c) throw()
            {
                assert(this->has(c));
                return (*this)[c.z][c.y][c.x];
            }
            
            //! type access, const
            const_type & operator[](const_coord c) const throw()
            {
                assert(this->has(c));
                return (*this)[c.z][c.y][c.x];
            }


            
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const SliceLayout sliceLayout; //!< layout of a slice
            const RowLayout   rowLayout;   //!< layout of a row
            
        private:
            Slice        *slices;
            size_t        built;
            
            inline void clear() throw()
            {
                while(built>0)
                {
                    self_destruct(slices[--built]);
                }
            }
            
            Y_DISABLE_COPY_AND_ASSIGN(Field3D);
            inline virtual const void *getObjectAt(const Coord1D *c) const throw()
            {
                assert(c);
                const_coord C(c[0],c[1],c[2]);
                assert(this->has(C));
                return &slices[C.z][C.y][C.x];
            }
        };
     
        
        template <typename COORD> struct FieldFor;
        
        template <> struct FieldFor<Coord1D>
        {
            template <typename T> struct Of
            {
                typedef Field1D<T> Type;
            };
        };
        
        template <> struct FieldFor<Coord2D>
        {
            template <typename T> struct Of
            {
                typedef Field2D<T> Type;
            };
        };
        
        template <> struct FieldFor<Coord3D>
        {
            template <typename T> struct Of
            {
                typedef Field3D<T> Type;
            };
        };
        
        
        
    }
    
}
#endif

