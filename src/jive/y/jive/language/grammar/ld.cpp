
#include "y/jive/language/grammar.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {

            Node * Node:: Load(Source &source, const Grammar &G)
            {
                static const char  fn[] = "LNode::Load::";
                const        char *gn   = **G.name;
                Char *ch = source.get();
                if(!ch)
                {
                    throw exception("%s%s(EOS)",fn,gn);
                }

                switch(ch->code)
                {
                    case TerminalMarker: {
                    } break;

                    case InternalMarker: {
                    } break;

                    default:
                        throw exception("%s.%s(unexpected Marker=0x%02x)",fn,gn,ch->code);
                }
                return NULL;
            }

            
        }

    }
}
