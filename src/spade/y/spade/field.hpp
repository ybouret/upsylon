//! \file

#ifndef Y_SPADE_FIELD_INCLUDED
#define Y_SPADE_FIELD_INCLUDED 1

#include "y/spade/layout-metrics.hpp"
#include "y/string.hpp"
#include "y/type/spec.hpp"
#include "y/container/const-field.hpp"

namespace upsylon {
    
    namespace Spade {
        
        namespace Kernel {
            
            class Field : public Object
            {
            public:
                virtual ~Field() throw();
                
                const string     name;
                const type_spec &objectType;
                const size_t     objectSize;
                
                virtual const LayoutMetrics &metrics() const throw() = 0;
                
            protected:
                explicit Field(const string    &id,
                               const type_spec &ts,
                               const size_t     bs);
                explicit Field(const char      *id,
                               const type_spec &ts,
                               const size_t     bs);
                 
                void  *workspace;
                size_t allocated;
                
                void*  allocate(const size_t);
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Field);
            };
        }
        
        template <typename T>
        class Field : public Kernel::Field
        {
        public:
            Y_DECL_ARGS(T,type);
            
            inline virtual ~Field() throw() {}
            
            
            
        protected:
            template <typename LABEL>
            inline explicit Field(const LABEL &id) :
            Kernel::Field(id,type_spec_of<T>(),sizeof(T))
            {
            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Field);
        };
        
    }
    
}

#endif


