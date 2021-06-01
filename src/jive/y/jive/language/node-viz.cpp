
#include "y/jive/language/axiom.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {

         

            void Node:: vizCore(ios::ostream &fp) const
            {

                switch(state)
                {
                    case IsTerminal: {
                        const Lexeme *l = lexeme();
                        string        s = "";
                        if(l) s = l->toEncoded();
                        fp << "[label=\"" << axiom.gvName();
                        if(s.size())
                        {
                            fp << "='" << s << "'";
                        }
                        fp << "\",shape=box]";
                        endl(fp);
                    } break;

                    case IsInternal:
                        endl(fp << "[label=\"" << *axiom.name << "\",shape=oval]");
                        for(const Node *node=leaves().head;node;node=node->next) {
                            node->vizSave(fp);
                            endl(vizJoin(fp,node));
                        }  break;
                }

            }
        }
    }

}

