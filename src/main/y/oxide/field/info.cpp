#include "y/oxide/field/info.hpp"

namespace upsylon
{
    namespace Oxide
    {
        FieldInfo:: ~FieldInfo() throw()
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
        
        FieldInfo:: FieldInfo(const string &id, const LayoutInfo &L, const size_t szObj) :
        Y_OXIDE_FIELD_INFO_CTOR()
        {
        }
        
        FieldInfo:: FieldInfo(const char *id, const LayoutInfo &L, const size_t szObj) :
        Y_OXIDE_FIELD_INFO_CTOR()
        {
        }
        
        void *   FieldInfo:: acquirePrivate()
        {
            static memory::allocator &mgr = memory::global::instance();
            return (privateData = mgr.acquire(privateSize));
        }
        
        void FieldInfo:: releasePrivate() throw()
        {
            assert(ownedObjects==0);
            if(privateSize)
            {
                static memory::allocator &mgr = memory::global::location();
                mgr.release(privateData,privateSize);
            }
        }
        
        string FieldInfo:: subName( const Coord1D n ) const
        {
            return name + vformat("[%ld]", static_cast<long>(n) );
        }

        void FieldInfo:: save(ios::ostream &fp, const Coord1D index, SaveProc proc) const
        {
            assert(proc);
            proc(fp,getObjectAddr(index));
        }

        void FieldInfo:: save(ios::ostream &fp, SaveProc proc) const
        {
            assert(proc);
            for(size_t index=0;index<localObjects;++index)
            {
                save(fp,index,proc);
            }
        }

        void FieldInfo:: load( ios::istream &fp, const Coord1D index, LoadProc proc)
        {
            assert(proc);
            proc(fp,(void*)getObjectAddr(index));
        }

       
        
        
        void FieldInfo:: load(ios::istream &fp, LoadProc proc)
        {
            assert(proc);
            for(size_t index=0;index<localObjects;++index)
            {
                load(fp,index,proc);
            }
        }

        void FieldInfo:: load(const memory::ro_buffer &buff, const Coord1D index, LoadProc proc)
        {
            ios::imstream fp(buff);
            load(fp,index,proc);
        }
        
        void FieldInfo:: load(const memory::ro_buffer &buff, LoadProc proc)
        {
            ios::imstream fp(buff);
            load(fp,proc);
        }


    }
}

