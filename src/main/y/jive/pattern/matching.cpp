
#include "y/jive/pattern/matching.hpp"

namespace upsylon
{
    namespace Jive
    {

        Matching:: ~Matching() throw()
        {
        }

        Matching::Matching(const Matching &m) throw() :
        Motif(m)
        {
        }

        bool Matching:: exactly_(Module *module) const
        {
            Source         source(module);
            Token          token;
            const Pattern &p = **this;
            if(p.accept(token,source))
            {
                return !source.alive();
            }
            else
            {
                return false;
            }
        }

        bool Matching:: somehow_(Token &token, Module *module) const
        {
            Source         source(module);
            const Pattern &p = **this;
            


        }


    }

}

