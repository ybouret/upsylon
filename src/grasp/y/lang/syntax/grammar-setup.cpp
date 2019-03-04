#include "y/lang/syntax/grammar.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            Terminal  & Grammar:: terminal( const string &id ) {
                Y_LANG_SYNTAX_VERBOSE("|_Terminal '" << id << "'" << std::endl);
                return decl( new Terminal(id) );
            }

            Terminal  & Grammar:: terminal( const char   *id ) { const string _(id); return terminal(_); }


            Aggregate & Grammar:: aggregate( const string &id ) {
                Y_LANG_SYNTAX_VERBOSE("|_Aggregate '" << id << "'" << std::endl);
                return decl( new Aggregate(id) );
            }

            Aggregate  & Grammar:: aggregate( const char   *id ) { const string _(id); return aggregate(_); }


            string Grammar:: MakeCompoundName( const ArrayOfStrings &strings, const char sep) const
            {
                const size_t ns = strings.size();
                if(ns<=0) throw exception("{%s} no strings to MakeCompoundName",**name);
                string ans;
                ans << '(';
                ans << strings[1];
                for(size_t i=2;i<=ns;++i)
                {
                    ans << sep << strings[i];
                }
                ans << ')';
                return ans;
            }

            string Grammar:: MakeAlternateName(const ArrayOfStrings &strings) const
            {
                return MakeCompoundName(strings,'|');
            }

            string Grammar:: MakeAggregateName(const ArrayOfStrings &strings) const
            {
                return MakeCompoundName(strings,' ');
            }



            Alternate & Grammar:: alternate( const string &id ) {
                Y_LANG_SYNTAX_VERBOSE("|_Alternate '" << id << "'" << std::endl);
                return decl( new Alternate(id) ); }

            Alternate & Grammar:: alternate( const char   *id ) { const string _(id); return alternate(_); }

            Alternate & Grammar:: alternate()
            {
                const string id  = vformat("alt#%u",iAlt++);
                return alternate(id);
            }

            const Rule & Grammar:: choice(const Rule &a, const Rule &b)
            {
                if(!ownsRule(a)) throw exception("{%s}.binary choice(loose Rule '%s')", **name, *a.name);
                if(!ownsRule(b)) throw exception("{%s}.binary choice(loose Rule '%s')", **name, *b.name);

                Strings strings(2,as_capacity);
                strings.push_back_(a.name);
                strings.push_back_(b.name);
                const string id = MakeAlternateName(strings);
                strings.release();
                Alternate & alt = alternate(id);
                alt << a << b;
                return alt;
            }

            const Rule & Grammar:: choice(const Rule &a, const Rule &b, const Rule &c)
            {
                if(!ownsRule(a)) throw exception("{%s}.ternary choice(loose Rule '%s')", **name, *a.name);
                if(!ownsRule(b)) throw exception("{%s}.ternary choice(loose Rule '%s')", **name, *b.name);
                if(!ownsRule(c)) throw exception("{%s}.ternary choice(loose Rule '%s')", **name, *c.name);

                Strings strings(3,as_capacity);
                strings.push_back_(a.name);
                strings.push_back_(b.name);
                strings.push_back_(c.name);
                const string id = MakeAlternateName(strings);
                strings.release();
                Alternate & alt = alternate(id);
                alt << a << b << c;
                return alt;
            }

            const Rule  & Grammar:: optional( const Rule &r )
            {
                if(!ownsRule(r)) throw exception("{%s}.optional(loose Rule '%s')",**name, *r.name);
                const string id = r.name + '?';
                Y_LANG_SYNTAX_VERBOSE("|_Optional '" << id << "'" << std::endl);
                return decl( new Optional(id,r) );
            }

            const Rule & Grammar:: zeroOrMore(const Rule &r)
            {
                if(!ownsRule(r)) throw exception("{%s}.zeroOrMore(loose Rule '%s')",**name, *r.name);
                const string id = r.name + '*';
                Y_LANG_SYNTAX_VERBOSE("|_ZeroOrMore '" << id << "'" << std::endl);
                return decl( new ZeroOrMore(id,r) );
            }

            const Rule & Grammar:: oneOrMore(const Rule &r)
            {
                if(!ownsRule(r)) throw exception("{%s}.oneOrMore(loose Rule '%s')",**name, *r.name);
                const string id = r.name + '+';
                Y_LANG_SYNTAX_VERBOSE("|_OneOrMore '" << id << "'" << std::endl);
                return decl( new OneOrMore(id,r) );
            }


#if 0
            Aggregate & Grammar:: aggregate()
            {
                const string id  = vformat("agg#%u",iAgg);
                Aggregate   &ans = aggregate(id);
                ++iAgg;
                return ans;
            }
#endif
            
            const Rule & Grammar:: join( const Rule &a, const Rule &b)
            {
                if(!ownsRule(a)) throw exception("{%s}.binary join(loose Rule '%s')", **name, *a.name);
                if(!ownsRule(b)) throw exception("{%s}.binary join(loose Rule '%s')", **name, *b.name);

                Strings strings(2,as_capacity);
                strings.push_back_(a.name);
                strings.push_back_(b.name);
                const string id = MakeAggregateName(strings);
                strings.release();
                Aggregate & agg = aggregate(id);
                agg << a << b;
                return agg.will(Merge);
            }

            const Rule & Grammar:: join( const Rule &a, const Rule &b, const Rule &c)
            {
                if(!ownsRule(a)) throw exception("{%s}.ternary join(loose Rule '%s')", **name, *a.name);
                if(!ownsRule(b)) throw exception("{%s}.ternary join(loose Rule '%s')", **name, *b.name);
                if(!ownsRule(c)) throw exception("{%s}.ternary join(loose Rule '%s')", **name, *c.name);

                Strings strings(3,as_capacity);
                strings.push_back_(a.name);
                strings.push_back_(b.name);
                strings.push_back_(c.name);
                const string id = MakeAggregateName(strings);
                strings.release();
                Aggregate & agg = aggregate(id);
                agg << a << b;
                return agg.will(Merge);
            }
        }
    }
}

