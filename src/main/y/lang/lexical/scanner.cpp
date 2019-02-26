
#include "y/lang/lexical/scanner.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {
#define Y_LANG_SCANNER_CTOR(ID) \
label(ID),                      \
rules(),                        \
probed(0),                      \
userDict(0),                    \
verbose(false)

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

            void Scanner:: emitLabel(const Tag &ruleLabel) const
            {
                std::cerr << "@scan[" << label << "].add( '" << ruleLabel << "' )" << std::endl;
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

            void Scanner:: nothing(const Token &) throw() {}

            void Scanner:: newline(const Token &) throw() { if(probed) probed->newLine(); }

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
