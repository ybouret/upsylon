
#include "y/spade/vtk.hpp"

namespace upsylon {

    namespace Spade
    {

        const char   vtk::DATASET[]      = "DATASET";
        const char   vtk::DIMENSIONS[]   = "DIMENSIONS";
        const char   vtk::ORIGIN[]       = "ORIGIN";
        const char   vtk::SPACING[]      = "SPACING";
        const char   vtk::_COORDINATES[] = "_COORDINATES";
        const char   vtk::POINTS[]       = "POINTS";
        const size_t vtk::Repeat[4]      = { 0, 4, 2, 1 };

        vtk:: vtk() :
        natives(),
        writers()
        {
            registerNatives();
            registerWriters();
        }

        vtk:: ~vtk() throw()
        {
        }
        
    }

}
