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
            assert(p);
            vizName(fp);
            arrow(fp);
            return p->vizName(fp);
        }

        ostream & vizible:: endl(ostream &fp)
        {
            return (fp << ';' << '\n');
        }

        ostream & vizible:: arrow(ostream &fp)
        {
            static const char arrow[] = " -> ";
            fp.output(arrow,sizeof(arrow)-1);
            return fp;
        }

        void vizible:: vizSave(ostream &fp) const
        {
            vizCore( vizName(fp) );
        }


        bool vizible:: graphViz(const string &fileName, const bool keepFile) const
        {
            {
                ios::ocstream fp(fileName);
                enterDigraph(fp,"G");
                vizSave(fp);
                leaveDigraph(fp);
            }
            return GraphViz::Render(fileName,keepFile);
        }




    }
}

#include "y/string/convert.hpp"

namespace upsylon {

    namespace ios {

        bool vizible:: graphViz(const char *fileName, const bool keepFile) const
        {
            const  string   _(fileName);
            return graphViz(_,keepFile);
        }

        void  vizible:: enterDigraph(ostream &fp, const string &id)
        {
            static const char gvDigraph[] = "digraph ";
            static const char gvDGEnter[] = " {\n";
            fp.output(gvDigraph, sizeof(gvDigraph)-1);
            fp << id;
            fp.output(gvDGEnter,sizeof(gvDGEnter)-1);
        }

        void  vizible:: enterDigraph(ostream &fp, const char *id)
        {
            const string _(id); enterDigraph(fp,_);
        }

        void  vizible:: leaveDigraph(ostream &fp)
        {
            static const char gvDGLeave[] = "}\n";
            fp.output(gvDGLeave,sizeof(gvDGLeave)-1);
        }

        ostream & vizible:: outputLabel(ostream &fp, const string &s)
        {
            const string _ = string_convert::to_printable(s);
            fp << "label=\"" << _ << "\"";
            return fp;
        }


        ostream & vizible:: outputLabel(ostream &fp, const char *s)
        {
            const string _ = s;
            return outputLabel(fp,_);
        }

        ostream & vizible:: appendShape(ostream &fp, const string &s)
        {
            fp << ",shape=\"" << s << "\"";
            return fp;
        }

        ostream & vizible:: appendStyle(ostream &fp, const string &s)
        {
            fp << ",style=\"" << s << "\"";
            return fp;
        }

    }

}

