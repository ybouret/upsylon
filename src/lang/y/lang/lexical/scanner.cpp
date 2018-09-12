
#include "y/lang/lexical/scanner.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {
#define Y_LANG_SCANNER_CTOR() \
label(new string(id)),        \
rules(),                      \
probed(0),                    \
userDict(0)

            Scanner:: Scanner( const string &id ) throw():
            Y_LANG_SCANNER_CTOR()
            {
            }


            Scanner:: Scanner( const char *id ) throw():
            Y_LANG_SCANNER_CTOR()
            {
            }

            Scanner:: ~Scanner() throw() {}
            

            void Scanner:: add(const string          &rule_label,
                               const Pattern::Handle &rule_motif,
                               const Action          &the_action)
            {
                std::cerr << "[" << label << "].." << rule_label << std::endl;
                for(const Rule *rule = rules.head; rule; rule=rule->next)
                {
                    if(*(rule->label)==rule_label)
                    {
                        throw exception("[%s]: multiple rule <%s>", **label, *rule_label);
                    }
                }
                const Origin rule_origin = new string(rule_label);
                rules.push_back( new Rule(rule_origin,rule_motif,the_action) );
            }

        }

    }
}

#include "y/code/utils.hpp"
namespace upsylon
{
    namespace Lang
    {
        namespace Lexical
        {
            Lexeme *Scanner:: probe( Source &source, Result &result )
            {

                probed =  *source;
                while(true)
                {
                    //__________________________________________________________
                    //
                    // check source status
                    //__________________________________________________________
                    result = Forward;
                    const Char *ch = source.peek();
                    if(!ch)
                    {
                        return NULL; //!< EOF
                    }

                    //__________________________________________________________
                    //
                    // find a first matching rule
                    //__________________________________________________________
                    Token bestData;
                    Rule *bestRule = rules.head;
                    for(;bestRule;bestRule=bestRule->next)
                    {
                        if(bestRule->motif->match(bestData,source))
                        {
                            break;
                        }
                    }

                    //__________________________________________________________
                    //
                    // lexical error
                    //__________________________________________________________
                    if(!bestRule)
                    {
                        throw exception("%s:%d:%d unexpected char '%s' for lexical scanner [%s]", **(ch->origin), ch->line,ch->column, visible_char[ch->code], **label);
                    }

                    //__________________________________________________________
                    //
                    // find a better rule
                    //__________________________________________________________
                    source.ungetCopy(bestData);
                    for(Rule *tempRule=bestRule->next;tempRule;tempRule=tempRule->next)
                    {
                        Token tempData;
                        if(tempRule->motif->match(tempData,source))
                        {
                            if(tempData.size>bestData.size)
                            {
                                //----------------------------------------------
                                // new winner
                                //----------------------------------------------
                                bestData.swap_with(tempData);
                                bestRule = tempRule;
                                source.ungetCopy(bestData);
                            }
                            else
                            {
                                //----------------------------------------------
                                // too late!
                                //----------------------------------------------
                                source.unget(tempData);
                            }
                        }
                    }
                    assert(source.loaded()>=bestData.size);
                    source.forward(bestData.size);

                    //__________________________________________________________
                    //
                    // execute better rule
                    //__________________________________________________________
                    result = bestRule->action(bestData);
                    switch(result)
                    {
                        case Forward: {
                            Lexeme *u = new Lexeme(bestRule->label);
                            u->swap_with(bestData);
                            return u;
                        }

                        case Discard:
                            break; // and continue

                        case Control:
                            return NULL;

                    }
                }


            }

        }
    }
}
