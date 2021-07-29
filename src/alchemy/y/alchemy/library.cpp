
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

    }
}


