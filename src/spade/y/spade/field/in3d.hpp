//! \file

#ifndef Y_SPADE_FIELD3D_INCLUDED
#define Y_SPADE_FIELD3D_INCLUDED 1

#include "y/spade/field/in2d.hpp"

namespace upsylon {
    
    namespace Spade {
        
        //----------------------------------------------------------------------
        //
        //! 3D fields
        //
        //----------------------------------------------------------------------
        template <typename T>
        class Field3D : public FieldOf<T>, public Layout3D
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
            typedef typename   LayoutType::Loop        Loop;        //!< alias

            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            
            //! cleanup
            inline virtual ~Field3D() throw() { clear(); }
            
            
            //! setup with internal memory
            template <typename LABEL> inline
            explicit Field3D(const LABEL      &id,
                             const LayoutType &L) :
            FieldOf<T>(id),
            LayoutType(L),
            sliceLayout(lower.xy(),upper.xy()),
            rowLayout(lower.x,lower.y),
            slice(0),
            built(0)
            {
                //______________________________________________________________
                //
                // acquire
                //______________________________________________________________
                Row          *rowAddr = 0;
                mutable_type *objAddr = 0;
                const size_t  nslice  = size_t(width.z);
                memory::embed emb[] =
                {
                    memory::embed::as(slice,nslice),
                    memory::embed::as(rowAddr,size_t(width.y)*nslice),
                    memory::embed::as(objAddr,items)
                };
                this->allocate(emb,sizeof(emb)/sizeof(emb[0]));

                //______________________________________________________________
                //
                // link
                //______________________________________________________________
                Slice *s    = slice;
                slice      -= lower.z;
                this->addr  = objAddr;

                //______________________________________________________________
                //
                // build
                //______________________________________________________________
                try
                {
                    Coord1D indx = lower.z;
                    while(built<nslice)
                    {
                        const string sid = this->name + Field::Suffix(indx);
                        new (s) Slice(sid,sliceLayout,rowAddr,objAddr);
                        ++s;
                        ++built;
                        ++indx;
                    }
                }
                catch(...)
                {
                    clear();
                    throw;
                }
                
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
                return slice[k];
            }
            
            //! slice access, const
            const Slice & operator[](const Coord1D k) const throw()
            {
                assert(k>=lower.z); assert(k<=upper.z);
                return slice[k];
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
            Slice        *slice;
            size_t        built;
            
            inline void clear() throw()
            {
                assert(slice);
                slice += lower.z;
                while(built>0)
                {
                    self_destruct(slice[--built]);
                }
            }
            
            Y_DISABLE_COPY_AND_ASSIGN(Field3D);
            inline virtual const void *getObjectByCoord(const Coord1D *c) const throw()
            {
                assert(c);
                const_coord C(c[0],c[1],c[2]);
                assert(this->has(C));
                return &slice[C.z][C.y][C.x];
            }
        };
     
      
        
        
    }
    
}
#endif

