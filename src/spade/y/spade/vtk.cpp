
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

        void vtk:: writeHeader(ios::ostream & fp,
                               const unsigned major,
                               const unsigned minor) const
        {
            fp << "# vtk DataFile Version ";
            fp("%u.%u\n",major,minor);
        }

        void vtk:: writeTitle(ios::ostream &fp,
                              const string &title) const
        {
            for(size_t i=0;i<title.size();++i)
            {
                char C = title[i];
                switch(C)
                {
                    case '\n':
                    case '\r':
                        C = '_';
                        break;

                    default:
                        break;
                }
                fp << C;
            }
            fp << '\n';
            fp << "ASCII\n";
        }


    }

}
