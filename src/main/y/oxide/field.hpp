//! \file
#ifndef Y_OXIDE_FIELD_INCLUDED
#define Y_OXIDE_FIELD_INCLUDED 1

#include "y/oxide/field/type.hpp"

namespace upsylon
{
    namespace Oxide
    {
        //! common constructor part
#define Y_OXIDE_FIELD_CTOR() Field(id, L, sizeof(T), typeid(T) ), entry(NULL), _data(NULL)
        
        //! common abstract API for typed fields, no dimension yet
        template <typename T>
        class FieldOf : public Field
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definition
            //
            //------------------------------------------------------------------
            Y_DECL_ARGS(T,type); //!< aliases
            type        *entry;  //!< linear data entry


            //------------------------------------------------------------------
            //
            // virtual interface
            //
            //------------------------------------------------------------------
            //! cleanup
            inline virtual ~FieldOf() throw()
            {
                freeData();
                entry = NULL;
            }
            
            //! get address of an object by its index
            virtual const void *getObjectAddr( const Coord1D index ) const throw()
            {
                assert(entry);
                assert(index>=0);
                assert(size_t(index)<localObjects);
                return &entry[index];
            }

            //! copy C++ style object
            virtual void copyObject( const Coord1D target, const Coord1D source )
            {
                assert(entry);
                assert(target>=0);
                assert(size_t(target)<localObjects);
                assert(source>=0);
                assert(size_t(source)<localObjects);
                assert(source!=target);
                *(mutable_type *) &entry[target] = entry[source];
            }


            //------------------------------------------------------------------
            //
            // non virtual interface
            //
            //------------------------------------------------------------------
            //! set every object to the same value
            inline void ld( param_type arg )
            {
                for(size_t i=0;i<localObjects;++i)
                {
                    entry[i] = arg;
                }
            }

            
        protected:
            //! initialize
            explicit FieldOf(const string &id, const LayoutInfo &L) : Y_OXIDE_FIELD_CTOR() {}
            
            //! initialize
            explicit FieldOf(const char *id, const LayoutInfo  &L) : Y_OXIDE_FIELD_CTOR() {}

            //! free registered linear data
            inline void freeData() throw()
            {
                size_t      &n = (size_t&)ownedObjects;
                while(n>0)
                {
                    assert(_data);
                    self_destruct( _data[--n] );
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
            Y_DISABLE_COPY_AND_ASSIGN(FieldOf);
            mutable_type * _data; //!< where objects were built
        };
        
        
    }
    
}


#endif

