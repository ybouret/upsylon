
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


#if 0


        
        
        
        const char library::clid[]       = "alchemy::library";
        
        const species & library:: use(species *s)
        {
            assert(NULL!=s);
            const species::pointer sp(s);
            if(!db.insert(sp))
            {
                throw exception("%s: multiple '%s'",clid,*(s->name) );
            }
            aliasing::_(compiled) = false;
            return *s;
        }
        
       
        
        bool library:: owns(const species &s) const throw()
        {
            const species::pointer *pps = db.search(s.key());
            if(!pps)
            {
                return false;
            }
            else
            {
                return &s == &(**pps);
            }
        }
        
        void library:: on_compile()
        {
            const size_t       n  = db.size();
            size_t             w  = 0;
            
            {
                db_type::iterator  it = db.begin();
                for(size_t i=1;i<=n;++i,++it)
                {
                    const species &sp = **it;
                    aliasing::_(sp.indx) = i;
                    w = max_of(w,sp.name.size());
                }
            }
            
            
            {
                db_type::iterator  it = db.begin();
                for(size_t i=1;i<=n;++i,++it)
                {
                    aliasing::_((**it).width) = w;
                }
            }
            
        }
#endif

    }
}


