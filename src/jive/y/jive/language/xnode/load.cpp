
#include "y/jive/language/grammar.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {

            Node * Node:: Reload(Source        &source,
                                 const Grammar &G,
                                 size_t        &stamp)
            {
                static const char  fn[] = "LNode::Load::";
                const        char *gn   = **G.name;
                size_t             sz   = 0;
                const string       id   = string::read(source,sz,"Axiom");
                const Axiom       *ax   = G.query(id); if(!ax)              throw exception("%s%s(no <%s>)",fn,gn,*id);
                size_t             nn   = 0; if(!source.query_upack(nn,sz)) throw exception("%s%s(missing count after <%s>)",fn,gn,*id);

                if(Terminal::UUID==ax->uuid)
                {
                    Lexeme::Pointer lx( Lexeme::Create(source.context(),ax->name,stamp) );
                    while(nn>0)
                    {
                        Char *ch = source.get();
                        if(!ch) throw exception("%s%s(missing char for <%s> lexeme)",fn,gn,*id);
                        lx->push(ch);
                        --nn;
                    }
                    return Node::Acquire(*ax,lx.yield());
                }
                else
                {
                    Pointer node( Node::Acquire(*ax) );
                    while(nn>0)
                    {
                        node->leaves().push_back( Reload(source,G,stamp) );
                        --nn;
                    }
                    return node.yield();
                }

            }

            Node * Node:: Load(Module *module,const Grammar &grammar)
            {
                size_t stamp = 0;
                Source source(module);
                return Reload(source,grammar,stamp);
            }

            
        }

    }
}
