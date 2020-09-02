
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/jive/scatter.hpp"
#include "y/sequence/list.hpp"
#include "y/jive/regexp.hpp"

using namespace upsylon;
using namespace Jive;

namespace
{

    static inline void dispMotif(std::ostream &os, const void *addr)
    {
        os << static_cast<const Pattern *>(addr)->toRegExp();
    }
}
Y_UTEST(scatter)
{

    {
        Scatter     table;
        list<Motif> motifs;
        for(unsigned i=0;i<256;++i)
        {
            {
                string rx;
                rx += vformat("\\x%02x",i);
                const Motif m = RegExp(rx,0);
                motifs.push_back(m);
            }
            table.record(motifs.back().content(),i);
        }
        table.display(std::cerr,dispMotif);
    }
    
    {
        Scatter     table;
        const Motif m1 = RegExp("[:digit:]+",NULL);
        {
            Leading fc;
            m1->start(fc);
            table.record(m1.content(),fc);
        }
        const Motif m2 = RegExp("[a-z]?",NULL);
        {
            Leading fc;
            m2->start(fc);
            table.record(m2.content(),fc);
        }

        table.display(std::cerr,dispMotif);
    }

    Y_UTEST_SIZEOF(Scatter::Node);

}
Y_UTEST_DONE()
