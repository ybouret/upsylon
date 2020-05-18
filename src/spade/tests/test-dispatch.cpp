
#include "y/spade/dispatcher.hpp"
#include "y/spade/partition.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/string/convert.hpp"
#include "y/spade/ops.hpp"

using namespace upsylon;
using namespace Spade;

namespace {
 
    template <typename COORD> static inline
    void doTest( const string &layout, const size_t ghosts, const size_t cores )
    {
        
        typedef typename FieldFor<COORD>::template Of<int>::Type iField;
        typedef typename iField::Handle iFieldHandle;
        
        
        const COORD   lower = Coord::Ones<COORD>();
        const COORD   upper = Coord::Parse<COORD>(layout);
        const Coord1D ng    = Coord1D(ghosts);
        const Layout<COORD> fullLayout(lower,upper);
        vector<COORD>       mappings;
        fullLayout.findMappings(mappings,cores);
        Dispatcher          dispatch(comms::homogeneous);
        
        std::cerr << "mappings=" << mappings << std::endl;
        for(size_t i=1;i<=mappings.size();++i)
        {
            const COORD          &mapping = mappings[i];
            
            std::cerr << "using " << mapping << std::endl;
            typename Layout<COORD>::Loop loop( Coord::Zero<COORD>(), Coord::Ones<COORD>() );
            for(loop.boot();loop.good();loop.next())
            {
                std::cerr << "\tpbcs=" << loop.value << std::endl;
                const Partition<COORD> partition(fullLayout,
                                                 mapping,
                                                 loop.value,
                                                 ng);
                const size_t        size = partition.size;
                slots<iFieldHandle> iFields(size);
                
                // filling
                for(size_t rank=0;rank<size;++rank)
                {
                    const string name = vformat("iField#%u",unsigned(rank));
                    iFieldHandle F    = new iField(name,partition.parts[rank].outer);
                    iFields.push(F);
                    Ops::Ld(*F,*F,int(rank));
                }
                
                // transfer
                for(size_t rank=0;rank<size;++rank)
                {
                    iField               &iF = *iFields[rank];
                    const Layouts<COORD> &L  = partition.parts[rank];
                    std::cerr << "\t\tautoExchange" << std::endl;
                    dispatch.autoExchange(iF,L);
                    
                }
                
                
                
            }
            
        }
        
    }
    
}


Y_UTEST(dispatch)
{
    string layout     = "10:10:10";
    size_t ghosts     = 1;
    size_t cores      = 1;
    
    if(argc>1) layout = argv[1];
    if(argc>2) ghosts = string_convert::to<size_t>(argv[2],"ghosts");
    if(argc>3) cores  = string_convert::to<size_t>(argv[3],"cores");

    doTest<Coord1D>(layout,ghosts,cores);
    doTest<Coord2D>(layout,ghosts,cores);
    doTest<Coord3D>(layout,ghosts,cores);
}
Y_UTEST_DONE()

