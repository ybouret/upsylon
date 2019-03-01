#include "y/lang/syntax/grammar.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            Alternate & Grammar:: alternate( const string &id ) { return decl( new Alternate(id) ); }
            Alternate & Grammar:: alternate( const char   *id ) { const string _(id); return alternate(_); }

            Alternate & Grammar:: alternate()
            {
                const string id  = vformat("alt#%u",iAlt++);
                return alternate(id);
            }

            const Rule & Grammar:: choice(const Rule &a, const Rule &b)
            {
                Alternate & alt = alternate();
                alt << a << b;
                return alt;
            }

            const Rule & Grammar:: choice(const Rule &a, const Rule &b, const Rule &c)
            {
                Alternate & alt = alternate();
                alt << a << b << c;
                return alt;
            }

            const Rule  & Grammar:: optional( const Rule &r )
            {
                const string id = vformat("opt#%u",iOpt++);
                return decl( new Optional(id,r) );
            }

            const Rule & Grammar:: zeroOrMore(const Rule &r)
            {
                const string id = vformat("zom#%u",iZoM++);
                return decl( new ZeroOrMore(id,r) );
            }

            const Rule & Grammar:: oneOrMore(const Rule &r)
            {
                const string id = vformat("oom#%u",iOoM++);
                return decl( new OneOrMore(id,r) );
            }
        }
    }
}

