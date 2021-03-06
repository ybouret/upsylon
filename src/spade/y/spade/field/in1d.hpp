//! \file
#ifndef Y_SPADE_FIELD1D_INCLUDED
#define Y_SPADE_FIELD1D_INCLUDED 1

#include "y/spade/field.hpp"
#include "y/spade/layout.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {
    
    namespace Spade {
        
        //! forward declaration
        template <typename T> class Field2D;
        
        //----------------------------------------------------------------------
        //
        //! 1D fields
        //
        //----------------------------------------------------------------------
        template <typename T>
        class Field1D : public FieldOf<T>, public Layout1D
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            Y_DECL_ARGS(T,type);                                  //!< aliases
            typedef          Layout1D                LayoutType;  //!< alias
            typedef typename LayoutType::coord       coord;       //!< alias
            typedef typename LayoutType::const_coord const_coord; //!< alias
            typedef          arc_ptr<Field1D>        Handle;      //!< alias
            typedef typename LayoutType::Loop        Loop;        //!< alias

            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            
            //! cleanup
            inline virtual ~Field1D() throw() { clear(); }
            
            //! setup with internal memory
            template <typename LABEL> inline
            explicit Field1D(const LABEL      &id,
                             const LayoutType &L) :
            FieldOf<T>(id), LayoutType(L), item(0), built(0)
            {
                standalone();
            }
            
            //! setup with internal memory, using coordinates
            template <typename LABEL> inline
            explicit Field1D(const LABEL &id,
                             const_coord  lo,
                             const_coord  up) :
            FieldOf<T>(id), LayoutType(lo,up), item(0), built(0)
            {
                standalone();
            }
                        
            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            
            //! return metrics
            inline virtual const LayoutMetrics & metrics() const throw() { return *this; }
            
            //! access
            inline type & operator[](const_coord i) throw()
            {
                assert(this->has(i));
                return item[i];
            }
            
            //! access, const
            inline const_type & operator[](const_coord i) const throw()
            {
                assert(this->has(i));
                return item[i];
            }
            
            
            

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Field1D);
            mutable_type *item;  //!< entry-lower
            size_t        built; //!< items
            
            friend class Field2D<T>;

            //! build as standalone
            inline void standalone()
            {
                mutable_type  *data  = 0;
                memory::embed  emb[] = { memory::embed::as(data,items) };
                this->allocate(emb,sizeof(emb)/sizeof(emb[0]));
                build(data);
            }


            //! data bytes >= items * sizeof(T)
            template <typename LABEL> inline
            explicit Field1D(const LABEL      &id,
                             const LayoutType &L,
                             mutable_type     * &data) :
            FieldOf<T>(id), LayoutType(L), item(0), built(0)
            {
                build(data);
            }
            
            inline void clear() throw()
            {
                assert(this->addr);
                assert(item);
                while(built>0)
                {
                    collapse(this->addr[--built]);
                }
                item = 0;
            }
            
            inline void build(mutable_type * &data)
            {
                assert(data);
                //______________________________________________________________
                //
                // link
                //______________________________________________________________
                this->addr = data;
                item       = this->addr-lower;

                //______________________________________________________________
                //
                // local build
                //______________________________________________________________
                try {
                    while(built<items)
                    {
                        new (data) mutable_type();
                        ++built;
                        ++data;
                    }
                }
                catch(...)
                {
                    clear();
                    throw;
                }
            }
            
            inline virtual const void *getObjectByCoord(const Coord1D *c) const throw()
            {
                assert(c);
                const_coord C = c[0];
                assert(this->has(C));
                return item+C;
            }
            
        };
        
    }
    
}
#endif

