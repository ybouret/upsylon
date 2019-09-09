#include "y/oxide/field/type.hpp"

namespace upsylon
{
    namespace Oxide
    {
        FieldType:: ~FieldType() throw()
        {
            assert(0==ownedObjects);
            (size_t &)linearExtent = 0;
            (size_t &)sizeOfObject = 0;
            releasePrivate();
        }
        
#define Y_OXIDE_FIELD_INFO_CTOR() \
name(id),                         \
localObjects(L.items),            \
ownedObjects(0),                  \
linearExtent(szObj*localObjects), \
sizeOfObject(szObj),              \
privateData(0),                   \
privateSize(0)
        
        FieldType:: FieldType(const string &id, const LayoutInfo &L, const size_t szObj) :
        Y_OXIDE_FIELD_INFO_CTOR()
        {
        }
        
        FieldType:: FieldType(const char *id, const LayoutInfo &L, const size_t szObj) :
        Y_OXIDE_FIELD_INFO_CTOR()
        {
        }
        
        void *   FieldType:: acquirePrivate()
        {
            static memory::allocator &mgr = memory::global::instance();
            return (privateData = mgr.acquire(privateSize));
        }
        
        void FieldType:: releasePrivate() throw()
        {
            assert(ownedObjects==0);
            if(privateSize)
            {
                static memory::allocator &mgr = memory::global::location();
                mgr.release(privateData,privateSize);
            }
        }
        
        string FieldType:: subName( const Coord1D n ) const
        {
            return name + vformat("[%ld]", static_cast<long>(n) );
        }

        size_t FieldType:: save(ios::ostream &fp, const Coord1D index, SaveProc proc) const
        {
            assert(proc);
            size_t ns = 0;
            proc(fp,getObjectAddr(index),&ns);
            return ns;
        }

        size_t FieldType:: load( ios::istream &fp, const Coord1D index, LoadProc proc)
        {
            assert(proc);
            size_t nr = 0;
            proc(fp,(void*)getObjectAddr(index),&nr);
            return nr;
        }


    }
}

