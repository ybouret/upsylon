
#include "y/jive/language/parser.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {
            Terminal::Type Parser:: Check(const Lexical::Rule &rule,
                                          const TerminalUsage  usage) const
            {
                const Motif &motif = rule.motif;
                if(motif->feeble())
                {
                    const string &id  = *rule.label;
                    const string  rx  = motif->toRegExp();
                    throw exception("%s: weak terminal <%s> : \"%s\"",**name,*id,*rx);
                }
                switch(usage)
                {
                    case SemanticTerminal:
                        return motif->univocal() ? Terminal::Univocal : Terminal::Standard;

                    case DivisionTerminal:
                        return Terminal::Division;
                }
                throw exception("%s: corrupted code in Check(rule,usage)",**name);
            }

            Parser:: ~Parser() throw()
            {
            }

            void Parser:: validate() const
            {
                validateWith(this);
            }

            Node *Parser:: parse(Source &source)
            {
                return run(source,*this);
            }

        }
    }
}
