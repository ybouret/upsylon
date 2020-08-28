
#include "y/jive/pattern/joker/joker.hpp"

namespace upsylon
{

    namespace Jive
    {
        Joker:: ~Joker() throw()
        {

        }

        Joker:: Joker(const uint32_t t, const Motif &m) throw() :
        Pattern(t),
        motif(m)
        {
        }

        Joker:: Joker(const Joker &j) :
        Pattern(j),
        motif(j.motif->clone())
        {

        }

        void Joker:: vizLink(ios::ostream &fp) const
        {
            motif->vizSave(fp);
            endl( motif->vizName(vizName(fp) << " -> ") );
        }


    }

}

