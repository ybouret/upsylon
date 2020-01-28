#include "y/oxide/field/type.hpp"

namespace upsylon
    {
        namespace Oxide
            {
                Field:: ~Field() throw()
                {
                    assert(0==ownedObjects);
                    _bzset(linearExtent);
                    _bzset(sizeOfObject);
                    releasePrivate();
                }


                void *   Field:: acquirePrivate()
                {
                    static memory::allocator &mgr = memory::global::instance();
                    return (privateData = mgr.acquire(privateSize));
                }

                void Field:: releasePrivate() throw()
                {
                    assert(ownedObjects==0);
                    if(privateSize)
                    {
                        static memory::allocator &mgr = memory::global::location();
                        mgr.release(privateData,privateSize);
                    }
                }

                string Field:: subName( const Coord1D n ) const
                {
                    return name + vformat("[%ld]", static_cast<long>(n) );
                }

                size_t Field:: save(ios::ostream &fp, const Coord1D index  ) const
                {
                    ios::plugin &plg = (ios::plugin&)(*transfer);
                    return plg.save(fp, getObjectAddress(index));
                }

                size_t Field:: load( ios::istream &fp, const Coord1D index )
                {
                    ios::plugin &plg = (ios::plugin&)(*transfer);
                    return plg.load(fp, (void*)getObjectAddress(index));
                }


                bool operator<(const Field &lhs, const Field &rhs) throw()
                {
                    return lhs.name < rhs.name;
                }


                void Field:: copyInternalObject(const Coord1D target, const Coord1D source)
                {
                    assert(target!=source);
                    copyLocalObjects( (void*)getObjectAddress(target), getObjectAddress(source) );
                }

                void Field:: copyExternalObject(const Coord1D   target,
                                                const Field    &other,
                                                const Coord1D   source)
                {
                    assert(typeOfObject==other.typeOfObject);
                    copyLocalObjects( (void*)getObjectAddress(target), other.getObjectAddress(source) );
                }

            }
    }


#include "y/oxide/field/io.hpp"

namespace upsylon
    {
        namespace Oxide
            {

                static inline ios::plugin * __plg( const std::type_info &tid )
                {
                    static const IO &io = IO::instance();
                    return io.create_for( type_spec::declare(tid) );
                }

#define Y_OXIDE_FIELD_INFO_CTOR() \
name(id),                         \
localObjects(L.items),            \
ownedObjects(0),                  \
linearExtent(szObj*localObjects), \
sizeOfObject(szObj),              \
typeOfObject(tid),                \
transfer(__plg(typeOfObject)),    \
privateData(0),                   \
privateSize(0)

                Field:: Field(const string &id, const LayoutInfo &L, const size_t szObj, const std::type_info &tid) :
                Y_OXIDE_FIELD_INFO_CTOR()
                {
                }

                Field:: Field(const char *id, const LayoutInfo &L, const size_t szObj, const std::type_info &tid) :
                Y_OXIDE_FIELD_INFO_CTOR()
                {
                }
            }

    }

