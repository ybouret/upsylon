#include "y/jive/language/axiom.hpp"


namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {


            size_t Node:: serialize(ios::ostream &fp) const
            {
                size_t written = axiom.name->serialize(fp);
                switch(state)
                {
                    case IsTerminal:
                        written += fp.write_upack(lexeme()->size);
                        for(const Char *ch=lexeme()->head;ch;ch=ch->next)
                        {
                            fp.write(ch->code);
                            ++written;
                        }
                        break;

                    case IsInternal:
                        written += fp.write_upack(leaves().size);
                        for(const Node *node=leaves().head;node;node=node->next)
                        {
                            written += node->serialize(fp);
                        }
                        break;
                }
                return written;
            }
        }
    }
}
