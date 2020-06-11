
#include "y/spade/vtk.hpp"

namespace upsylon {

    namespace Spade
    {

        vtk:: vtk() :
        natives(),
        writers()
        {
            registerNatives();
            registerWriters();

#if 0
            natives.get_root().graphViz("vtk-natives.dot");
            writers.get_root().graphViz("vtk-writers.dot");
#endif
        }

        vtk:: ~vtk() throw()
        {
        }
        
    }

}
