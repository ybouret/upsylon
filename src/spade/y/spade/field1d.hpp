//! \file
#ifndef Y_SPADE_FIELD1D_INCLUDED
#define Y_SPADE_FIELD1D_INCLUDED 1

#include "y/spade/field.hpp"
#include "y/spade/layout.hpp"

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
        class Field1D : public Field<T>, public Layout1D
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
            Field<T>(id),
            LayoutType(L),
            addr(static_cast<mutable_type *>(this->allocate(sizeof(T)*items) )),
            item(addr-lower),
            built(0)
            {
                build();
            }
            
            //! setup with internal memory, using coordinates
            template <typename LABEL> inline
            explicit Field1D(const LABEL &id,
                             const_coord  lo,
                             const_coord  up) :
            Field<T>(id),
            LayoutType(lo,up),
            addr(static_cast<mutable_type *>(this->allocate(sizeof(T)*items) )),
            item(addr-lower),
            built(0)
            {
                build();
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
            mutable_type *addr;  //!< entry
            mutable_type *item;  //!< entry-lower
            size_t        built; //!< items
            
            friend class Field2D<T>;
            
            //! data bytes >= items * sizeof(T)
            template <typename LABEL> inline
            explicit Field1D(const LABEL      &id,
                             const LayoutType &L,
                             void             *data) :
            Field<T>(id),
            LayoutType(L),
            addr(static_cast<mutable_type *>(data) ),
            item(addr-lower),
            built(0)
            {
                build();
            }
            
            inline void clear() throw()
            {
                while(built>0)
                {
                    self_destruct(addr[--built]);
                }
            }
            
            inline void build()
            {
                try {
                    while(built<items)
                    {
                        new (addr+built) mutable_type();
                        ++built;
                    }
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

