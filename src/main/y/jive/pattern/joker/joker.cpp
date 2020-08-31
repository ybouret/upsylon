
#include "y/jive/pattern/joker/joker.hpp"

namespace upsylon
{

    namespace Jive
    {
        Joker:: ~Joker() throw()
        {
            assert(motif);
            delete (Pattern *)motif;
            aliasing::_(motif) = 0;
        }

        Joker:: Joker(const uint32_t t, const Pattern *m) throw() :
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

        void Joker:: update(Entropy &E) const throw()
        {
            motif->update(E);
        }

        void Joker:: start(FirstChars &fc) const
        {
            assert(0==fc.size());
            motif->start(fc);
        }

        void Joker:: optimize() throw()
        {
            aliasing::_(motif) = Optimize( (Pattern *)motif );
        }

    }

}

