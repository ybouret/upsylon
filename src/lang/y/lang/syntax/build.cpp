#include "y/lang/syntax/grammar.hpp"
#include "y/lang/syntax/joker.hpp"

#include "y/exception.hpp"


namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            bool  Grammar:: owns( const Rule &r ) const throw()
            {
                return rules.owns( &r );
            }

            Rule & Grammar:: optional( const Rule &r )
            {
                // check
                if(!owns(r)) throw exception("{%s}.optional: doesn't own '%s'", **name, *(r.name));

                // create the name
                const string id = r.name + '?';

                // check multiple
                const Rule  *s  = getRuleByName(id);
                if(s)
                {
                    // already exists
                    return *(Rule *)s;
                }
                else
                {
                    // new rule
                    return __add( new Optional(id,r) );
                }
            }

            Rule & Grammar:: repeating(const Rule &r,const size_t nmin)
            {
                if(!owns(r)) throw exception("{%s}.repeating: doesn't own '%s'", **name, *(r.name));

                //create the name
                string id = r.name;
                switch(nmin)
                {
                    case 0:  id << '*'; break;
                    case 1:  id << '+'; break;
                    default: id << vformat(">=%u",unsigned(nmin)); break;
                }

                // check multiple
                const Rule *s = getRuleByName(id);
                if(s)
                {
                    // already exists
                    return *(Rule *)s;
                }
                else
                {
                    // new rule
                    return __add( new Repeating(id,r,nmin) );
                }
            }

        }
    }
}
