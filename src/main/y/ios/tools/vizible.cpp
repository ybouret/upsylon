#include "y/ios/tools/vizible.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/tools/graphviz.hpp"

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


        bool vizible:: graphViz(const string &fileName, const bool keepFile) const
        {
            {
                ios::ocstream fp(fileName);
                fp << "digraph G {\n";
                vizSave(fp);
                fp << "}\n";
            }
            return GraphViz::Render(fileName,keepFile);
        }



    }
}

#include "y/string.hpp"
namespace upsylon {

    namespace ios {

        bool vizible:: graphViz(const char *fileName, const bool keepFile) const
        {
            const  string   _(fileName);
            return graphViz(_,keepFile);
        }
    }

}

