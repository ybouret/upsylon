#include "y/jive/language/axiom.hpp"
#include "y/string/convert.hpp"


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


namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {
            string   Axiom:: gvName() const
            {
                return string_convert::to_printable(*name);
            }

            Axiom:: ~Axiom() throw()
            {
            }

        }
    }

}


