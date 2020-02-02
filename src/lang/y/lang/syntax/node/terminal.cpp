#include "y/lang/syntax/node/terminal.hpp"
#include "y/lang/syntax/rule.hpp"

namespace upsylon {

    namespace Lang {

        namespace Syntax {

            Node * Node::Create(const Rule &r, Lexeme *l)
            {
                assert(l);
                try
                {
                    return new TerminalNode(r,l);
                }
                catch(...)
                {
                    delete l;
                    throw;
                }
            }



            void TerminalNode:: returnTo(Lexer &lexer) throw()
            {
                assert(lx);
                lexer.unget(lx);
                lx=0;
            }


            TerminalNode:: ~TerminalNode() throw()
            {
                if(lx)
                {
                    delete lx;
                    lx = 0;
                }
            }

            TerminalNode:: TerminalNode(const Rule &r, Lexeme *l) throw() :
            Node(r,true),
            lx(l)
            {
                assert(NULL!=lx);
            }

            Node * TerminalNode:: clone() const
            {
                return Node::Create(rule, new Lexeme(*lx) );
            }

            const void  * TerminalNode:: inner() const throw()
            {
                assert(lx);
                return lx;
            }


            size_t TerminalNode:: serialize(ios::ostream &fp) const
            {
                size_t ans = rule.name.serialize(fp);
                Y_OSTREAM_ADD_TO(ans,fp.emit_net,UUID);
                Y_OSTREAM_ADD_TO(ans,fp.emit_upack,lx->size);
                for(const Char *ch = lx->head;ch;ch=ch->next)
                {
                    fp.emit_net(ch->code);
                }
                return ans+lx->size;
            }


            const char *TerminalNode:: className() const throw()
            {
                return "TerminalNode";
            }

            const string * TerminalNode:: data() const throw() { return 0; }

        }

    }

}

#include "y/lang/syntax/rule/terminal.hpp"
#include "y/string/convert.hpp"

namespace upsylon {

    namespace Lang {

        namespace Syntax {

            void     TerminalNode::   vizCore( ios::ostream &fp ) const
            {
                assert(lx);
                string l = string_convert::to_printable( rule.name );
                if(!rule.as<Terminal>().univocal)
                {
                    if(lx->size)
                    {
                        const string content = lx->toPrintable();
                        l <<'=' << '\'' << content << '\'';
                    }
                }
                const char *sh =  "box";
                const char *st =  "solid";
                switch( rule.as<Terminal>().attr )
                {
                    case Standard: break;
                    case Operator: sh="triangle"; break;
                    case Semantic: st="dashed";   break;
                }
                fp("[shape=\"%s\",style=\"%s\",label=\"",sh,st); fp << l << "\"]";
                endl(fp);
            }

        }

    }

}


