#include "y/spade/mesh/curvilinear.hpp"
#include "y/exceptions.hpp"

#include <cerrno>

namespace upsylon {

    namespace Spade {

        namespace Kernel {

            Curvilinear:: Curvilinear() throw()
            {
            }

            Curvilinear:: ~Curvilinear() throw()
            {
            }

            const char Curvilinear::Category[] = "curvilinear";

            void Curvilinear::CheckTorus(const double rOut, const double rIn)
            {
                static const char fn[] = "CurvlinearMesh::Torus: ";
                if(rOut<=0)  throw libc::exception(EDOM,"%srOut=%g",fn,rOut);
                if(rIn<=0)   throw libc::exception(EDOM,"%srIn=%g",fn,rIn);
                if(rIn>rOut) throw libc::exception(EDOM,"%srIn=%g>rOut=%g",fn,rIn,rOut);
            }

        }

    }
}
