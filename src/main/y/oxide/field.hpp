
//! \file

#ifndef Y_OXIDE_FIELD_INCLUDED
#define Y_OXIDE_FIELD_INCLUDED 1

#include "y/oxide/field/info.hpp"

namespace upsylon
{
    namespace Oxide
    {
        //! common constructor part
#define Y_OXIDE_FIELD_CTOR() FieldInfo(id, L, sizeof(T) ), entry(NULL), _data(NULL)
        
        //! common abstract API for fields
        template <typename T>
        class Field : public FieldInfo
        {
        public:
            Y_DECL_ARGS(T,type); //!< aliases
            type        *entry;  //!< linear data entry

            //! cleanup
            inline virtual ~Field() throw()
            {
                freeData();
                entry = NULL;
            }

            //------------------------------------------------------------------
            // virtual interface
            //------------------------------------------------------------------
            virtual const void *getObjectAddr( const Coord1D index ) const throw()
            {
                assert(entry);
                assert(index>=0);
                assert(size_t(index)<localObjects);
                return &entry[index];
            }

            //------------------------------------------------------------------
            // non virtual interface
            //------------------------------------------------------------------
            inline void ld( param_type arg )
            {
                for(size_t i=0;i<localObjects;++i)
                {
                    entry[i] = arg;
                }
            }

            
        protected:
            //! initialize
            explicit Field<T>(const string &id, const LayoutInfo &L) : Y_OXIDE_FIELD_CTOR() {}
            
            //! initialize
            explicit Field<T>(const char *id, const LayoutInfo  &L) : Y_OXIDE_FIELD_CTOR() {}

            //! free registered data
            inline void freeData() throw()
            {
                size_t      &n = (size_t&)ownedObjects;
                while(n>0)
                {
                    assert(_data);
                    destruct(& _data[--n] );
                }
                _data = 0;
            }
            
            //! create L.items objects, becoming ownedTypes
            inline void makeData(void *addr)
            {
                assert(addr);
                assert(0==ownedObjects);
                assert(0==_data);
                _data          = static_cast<mutable_type *>(addr);
                size_t      &n = (size_t&)ownedObjects;
                const size_t m = localObjects;
                try
                {
                    while(n<m)
                    {
                        new (_data+n) mutable_type();
                        ++n;
                    }
                    assert(localObjects==ownedObjects);
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

