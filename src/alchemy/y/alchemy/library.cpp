
#include "y/alchemy/library.hpp"
#include "y/exception.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace Alchemy
    {

        Library:: ~Library() throw()
        {
        }

        Library:: Library() :
        gateway<const Species::Set>(),
        sdb(),
        snw(0)
        {
        }


        Library::const_type &Library:: bulk() const throw()
        {
            return sdb;
        }

        const char Library::CLID[] = "Library";
        
        const Species & Library:: use(Species *s)
        {
            assert(s);
            const Species::Pointer sp(s);
            if(freezed)         throw exception("%s is freezed whilst adding '%s'",CLID,*(s->name));
            if(!sdb.insert(sp)) throw exception("%s: multiple '%s'", CLID, *(s->name));
            aliasing::_(snw) = max_of(snw,s->name.size());
            return *s;
        }

        bool Library:: owns(const Species &s) const throw()
        {
            const Species::Pointer *pps = sdb.search(s.key());
            if(!pps)
            {
                return false;
            }
            else
            {
                return &s == &(**pps);
            }
        }

        const Species & Library::operator[](const string &id) const
        {
            const Species::Pointer *pps = sdb.search(id);
            if(!pps) throw exception("no %s['%s']",CLID,*id);
            return **pps;
        }

        const Species & Library::operator[](const char *id) const
        {
            const string _(id); return (*this)[_];
        }

    }
}


