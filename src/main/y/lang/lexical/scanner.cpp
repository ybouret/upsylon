
#include "y/lang/lexical/scanner.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Lang {

        namespace Lexical {

#define Y_LANG_SCANNER_CTOR(ID) \
label(ID),                      \
rules(),                        \
probed(0),                      \
userDict(0),                    \
verbose(false),                 \
echo(false)

            Scanner:: Scanner( const string &id ) :
            Y_LANG_SCANNER_CTOR(new string(id))
            {
            }


            Scanner:: Scanner( const char *id ) :
            Y_LANG_SCANNER_CTOR(new string(id))
            {
            }

            Scanner:: Scanner( const Tag &id ) throw() :
            Y_LANG_SCANNER_CTOR(id)
            {
            }
            
            const string & Scanner:: key() const throw()
            {
                return *label;
            }

            Scanner:: ~Scanner() throw() {}

            std::ostream  & Scanner:: indent( std::ostream &os) const
            {
                os << ' ';
                return os;
            }
            
            void Scanner:: emitLabel(const Tag &ruleLabel) const
            {
                indent(std::cerr) << "@scan[" << label << "].add( '" << ruleLabel << "' )" << std::endl;
            }


            void Scanner:: checkLabel(const Tag &ruleLabel) const
            {
                for(const Rule *r = rules.head; r; r=r->next )
                {
                    if( *ruleLabel == *(r->label) )
                    {
                        throw exception("[%s] multiple rule '%s'",**label,**ruleLabel);
                    }
                }
            }

            void Scanner:: checkMotif(const Tag &ruleLabel ,const Motif &ruleMotif) const
            {
                if( ruleMotif->weak() )
                {
                    throw exception("[%s] weak expression for rule '%s'", **label, **ruleLabel);
                }
            }

            const Rule * Scanner:: getRuleByLabel(const string &id) const throw()
            {
                for(const Rule *r = rules.head; r; r=r->next )
                {
                    if(id==*(r->label))
                    {
                        return r;
                    }
                }
                return 0;
            }

            const Rule & Scanner:: last() const throw()
            {
                assert(rules.size>0);
                assert(rules.tail);
                return *(rules.tail);
            }


            string Scanner::JumpLabel( const string &target )
            {
                return "->" + target;
            }

            string Scanner::CallLabel( const string &target )
            {
                return "=>" + target;
            }

            string Scanner:: BackLabel( const string &l, const string &rx )
            {
                return "<=" + l + '.' + rx;
            }

            void Scanner:: nothing(const Token &t)
            {
                if(echo) {
                    const string s = t.toVisible();
                    std::cerr << '<' << s << '>';
                }
            }
            
            void Scanner:: newline(const Token &t)  
            {
                if(probed) probed->newLine();
                nothing(t);
            }

            void Scanner:: emit(const string &id,const string &rx) { forward(id,rx,this,&Scanner::nothing); }

            void Scanner:: emit(const char   *id,const char   *rx) { const string _=id; const string __=rx; emit(_,__); }

            void Scanner:: drop(const string &id,const string &rx) { discard(id,rx,this,&Scanner::nothing); }

            void Scanner:: drop(const char   *id,const char   *rx) { const string _=id; const string __=rx; drop(_,__); }

            void Scanner:: drop(const char *rx) { const string _(rx); drop(_,_); }

            void Scanner:: endl(const string &id,const string &rx) { discard(id,rx,this,&Scanner::newline); }

            void Scanner:: endl(const char   *id,const char   *rx) { const string _=id; const string __=rx; endl(_,__); }

            void Scanner:: endl(const char *rx) { const string _(rx); endl(_,_); }

        }
    }
}
