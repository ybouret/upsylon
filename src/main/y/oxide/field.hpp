
//! \file

#ifndef Y_OXIDE_FIELD_INCLUDED
#define Y_OXIDE_FIELD_INCLUDED 1

#include "y/oxide/field/info.hpp"

namespace upsylon
{
    namespace Oxide
    {
        template <typename T>
        class Field : public FieldInfo
        {
        public:
            Y_DECL_ARGS(T,type);
            type        *entry; //!< linear data entry
            
            inline virtual ~Field() throw()
            {
                freeData();
            }
            
        protected:
            explicit Field<T>(const string &id, const LayoutInfo &L ) :
            FieldInfo(id, L.items * sizeof(T) ),
            entry(NULL),
            _data(NULL)
            {
            }
            
            //! free registers data
            void freeData() throw()
            {
                size_t      &n = (size_t&)ownedTypes;
                while(n>0)
                {
                    assert(_data);
                    destruct(& _data[--n] );
                }
                _data = 0;
            }
            
            //! create is necessary setup entry
            void makeData(void *addr, const LayoutInfo &L)
            {
                assert(addr);
                assert(0==ownedTypes);
                _data = static_cast<mutable_type *>(addr);
                size_t      &n = (size_t&)ownedTypes;
                const size_t m = L.items;
                try
                {
                    while(n<m)
                    {
                        new (_data+n) mutable_type();
                        ++n;
                    }
                }
                catch(...)
                {
                    throw;
                }
            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Field);
            mutable_type * _data; //!< where objects were built
        };
        
        
    }
    
}


#endif

