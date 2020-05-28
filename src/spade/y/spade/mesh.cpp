
#include "y/spade/mesh.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Spade {

        namespace Kernel {

            Mesh:: ~Mesh() throw()
            {
            }

            static const char fn[] = "Spade::Mesh: ";

            void Mesh:: checkDims() const
            {
                if(space<=0)            throw exception("%snegative space for %s",    fn, *label);
                if(topology<=0)         throw exception("%snegative topology for %s", fn, *label);
                if(topology>space)      throw exception("%stopology>space for %s",    fn, *label);
            }

#define Y_SPADE_MESH_CTOR()   label(id),  topology(t), space(d)

            Mesh:: Mesh(const string &id, const unsigned t, const unsigned d) :
            Y_SPADE_MESH_CTOR()
            {
                checkDims();
            }

            Mesh:: Mesh(const char *id, const unsigned t, const unsigned d) :
            Y_SPADE_MESH_CTOR()
            {
                checkDims();
            }
            
            string Mesh:: AxisTag(const unsigned d)
            {
                return vformat("#%u",d);
            }


        }
        
        
    }
    
}
