#include "y/ios/tools/vizible.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {

    namespace ios {

        vizible:: ~vizible() throw() {}

        vizible:: vizible() throw() {}

        ostream & vizible:: vizName(ostream &fp) const
        {
            return fp.viz(this);
        }

        ostream & vizible:: vizJoin(ostream &fp, const vizible *p) const
        {
            static const char arrow[] = " -> ";
            assert(p);
            vizName(fp);
            fp.output(arrow,sizeof(arrow)-1);
            return p->vizName(fp);
        }

        void vizible:: endl(ostream &fp)
        {
            fp << ';' << '\n';
        }

        void vizible:: vizSave(ostream &fp) const
        {
            vizCore( vizName(fp) );
        }

    }
}
