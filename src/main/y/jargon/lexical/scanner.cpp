#include "y/jargon/lexical/scanner.hpp"
#include "y/associative/suffix-tree.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        namespace Lexical {
            
            const char Scanner:: callPrefix[] = "->";
            const char Scanner:: jumpPrefix[] = "=>";
            const char Scanner:: backPrefix[] = "<-";
            
            
            typedef suffix_tree<Rule *> rDict;
            
            Scanner:: ~Scanner() throw()
            {
                finish();
            }
            
#define Y_JSCANNER(TAG) \
Object(),\
inode<Scanner>(),\
label(TAG),\
rules(),\
chars(NULL),\
rdict( new rDict() ),\
dict(NULL)
            
            Scanner:: Scanner( const string &id ) :
            Y_JSCANNER( new string(id) )
            {
            }
            
            Scanner:: Scanner( const Tag &tag ) :
            Y_JSCANNER( tag )
            {
            }
            

            
            void Scanner:: doNothing(const Token &) const throw()
            {
            }
            
            void Scanner:: finish() throw()
            {
                if(rdict)
                {
                    delete static_cast<rDict *>(rdict);
                    rdict = 0;
                }
            }
            
            void Scanner:: resume()
            {
                if(!rdict)
                {
                    rdict = new rDict();
                    Rules  temp;
                    temp.swap_with(rules);
                    while( temp.size )
                    {
                        add( temp.pop_front() );
                    }
                }
            }
            
            bool Scanner:: building() const throw()
            {
                return NULL!=rdict;
            }
            
            void Scanner:: add(Rule *rule)
            {
                assert(rule!=NULL);
                assert(building());
                
                rDict        *dict = static_cast<rDict *>(rdict);
                const string &id   = *(rule->label);
                try
                {
                    if( !dict->insert_by(id,rule))
                    {
                        throw exception("Jargon::Scanner[%s].add(multiple rule <%s>)",**label,*id);
                    }
                }
                catch(...)
                {
                    delete rule;
                    throw;
                }
                rules.push_back(rule);
            }
            
            void Scanner:: doNewLine(const Token &) throw()
            {
                assert(chars);
                chars->newLine();
            }
            
            void Scanner:: compileRulesWith( Analyzer &lexer )
            {
                for(Rule *rule=rules.head;rule;rule=rule->next)
                {
                    const Event &event = *(rule->event);
                    if(Event::Control==event.kind)
                    {
                        static_cast<ControlEvent *>((void*)event.self)->compileWith(lexer);
                    }
                }
                
            }

            
        }
    }
}
