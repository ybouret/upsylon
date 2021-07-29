
#include "y/alchemy/library.hpp"
#include "y/exception.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace alchemy
    {

        library:: ~library() throw()
        {
        }

        library:: library() :
        db(),
        compiled(false),
        max_name(0)
        {
        }

        const library::db_type & library:: operator*() const throw()
        {
            return db;
        }

        const char library::clid[] = "alchemy::library";

        const species & library:: use(species *s)
        {
            assert(NULL!=s);
            const species::pointer sp(s);
            if(!db.insert(sp))
            {
                throw exception("%s: multiple '%s'",clid,*(s->name) );
            }
            aliasing::_(compiled) = false;
            aliasing::_(max_name) = max_of(max_name,s->name.size());
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

        size_t library:: compile()
        {
            aliasing::_(compiled) = false;
            const size_t       n  = db.size();
            db_type::iterator  it = db.begin();
            for(size_t i=1;i<=n;++i,++it)
            {
                aliasing::_((**it).indx) = i;
            }
            aliasing::_(compiled) = true;
            return n;
        }

    }
}


