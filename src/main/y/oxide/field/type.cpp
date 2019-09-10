#include "y/oxide/field/type.hpp"

namespace upsylon
{
    namespace Oxide
    {
        FieldType:: ~FieldType() throw()
        {
            assert(0==ownedObjects);
            bzset_(linearExtent);
            bzset_(sizeOfObject);
            releasePrivate();
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

        size_t FieldType:: save(ios::ostream &fp, const Coord1D index  ) const
        {
            return transfer->save(fp, getObjectAddr(index));
        }

        size_t FieldType:: load( ios::istream &fp, const Coord1D index )
        {
            return transfer->load(fp, (void*)getObjectAddr(index));
        }

    }
}


#include "y/oxide/field/io.hpp"

namespace upsylon
{
    namespace Oxide
    {

        static inline ios::plugin * __plugin_for( const std::type_info &tid )
        {
            static const IO &io = IO::instance();
            return io.create_for(tid);
        }

#define Y_OXIDE_FIELD_INFO_CTOR() \
name(id),                         \
localObjects(L.items),            \
ownedObjects(0),                  \
linearExtent(szObj*localObjects), \
sizeOfObject(szObj),              \
transfer(__plugin_for(tid)),      \
privateData(0),                   \
privateSize(0)

        FieldType:: FieldType(const string &id, const LayoutInfo &L, const size_t szObj, const std::type_info &tid) :
        Y_OXIDE_FIELD_INFO_CTOR()
        {
        }

        FieldType:: FieldType(const char *id, const LayoutInfo &L, const size_t szObj, const std::type_info &tid) :
        Y_OXIDE_FIELD_INFO_CTOR()
        {
        }
    }

}

