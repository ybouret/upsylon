//! \file

#ifndef Y_SPADE_FIELD_INCLUDED
#define Y_SPADE_FIELD_INCLUDED 1

#include "y/spade/layout/metrics.hpp"
#include "y/container/const-field.hpp"
#include "y/string.hpp"
#include <typeinfo>

namespace upsylon {
    
    namespace ios
    {
        class conveyor;
    }
    
    namespace Spade {
        
        namespace Kernel {
            
            //------------------------------------------------------------------
            //
            //! Field(s) base type
            //
            //------------------------------------------------------------------
            class Field : public Object
            {
            public:
                virtual ~Field() throw(); //!< cleanup
                 
                //--------------------------------------------------------------
                //
                // virtual interface
                //
                //--------------------------------------------------------------
                
                //! get common metrics
                virtual const LayoutMetrics &metrics() const throw() = 0;
                
                //--------------------------------------------------------------
                //
                // non virtual interface
                //
                //--------------------------------------------------------------
                
                size_t        objectBytes() const throw();    //!< bytes for objects
                size_t        localMemory() const throw();    //!< allocated
                double        virtualBits() const throw();    //!< allocated per used
                size_t        objects() const throw();        //!< metrics().items
                static string Suffix(const Coord1D c);        //!< "[c]"
                
                //--------------------------------------------------------------
                //
                // members
                //
                //--------------------------------------------------------------
                const string          name;        //!< identifier
                const std::type_info &objectType;  //!< type_spec
                const size_t          objectSize;  //!< bytes per object
                const ios::conveyor  *io;          //!< optional I/O

                const void *objectAt(const size_t indx) const throw();
                void       *objectAt(const size_t indx)       throw();

            protected:
                //! setup
                explicit Field(const string         &id,
                               const std::type_info &ts,
                               const size_t          bs);
                
                //! setup
                explicit Field(const char           *id,
                               const std::type_info &ts,
                               const size_t          bs);
                 
                void  *workspace; //!< private bytes
                size_t allocated; //!< private bytes
                
                void*  allocate(const size_t); //!< acquire private bytes
                
                virtual const void *getObjectByCoord(const Coord1D *) const throw() = 0;
                virtual const void *getObjectAt(const size_t   ) const throw() = 0;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Field);
            };
        }
        
        
        //----------------------------------------------------------------------
        //
        //! typed Filed base class
        //
        //----------------------------------------------------------------------
        template <typename T>
        class Field : public Kernel::Field
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            Y_DECL_ARGS(T,type); //!< aliases
            
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            //! cleanup
            inline virtual ~Field() throw() { addr=0; }

            //------------------------------------------------------------------
            //
            // non virtual interface
            //
            //------------------------------------------------------------------

            //! direct access
            inline type & operator()(const size_t indx) throw()
            {
                assert(addr);
                assert(indx<metrics().items);
                return addr[indx];
            }

            //! direct access, const
            inline const_type & operator()(const size_t indx) const throw()
            {
                assert(addr);
                assert(indx<metrics().items);
                return addr[indx];
            }
            
           
            
        protected:
            //! setup
            template <typename LABEL>
            inline explicit Field(const LABEL &id) :
            Kernel::Field(id,typeid(T),sizeof(T)),
            addr(0)
            {
            }

            mutable_type *addr; //!< address of first object
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Field);
            virtual const void *getObjectAt(const size_t indx) const throw()
            {
                assert(addr);
                assert(indx<metrics().items);
                return addr+indx;
            }

        };
        
    }
    
}

#endif


