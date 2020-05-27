
#include "y/spade/mesh.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Spade {
        
        Mesh:: ~Mesh() throw()
        {
        }

        static const char fn[] = "Spade::Mesh: ";

        void Mesh:: checkDims() const
        {
            if(dimensions<=0)       throw exception("%snegative dimensions for %s",fn, *name);
            if(topology>dimensions) throw exception("%stopology>dimension for %s",fn, *name);
        }

#define Y_SPADE_MESH_CTOR()   name(id), dimensions(d), topology(t)

        Mesh:: Mesh(const string &id, const unsigned d, const unsigned t) :
        Y_SPADE_MESH_CTOR()
        {
            checkDims();
        }

        Mesh:: Mesh(const char *id, const unsigned d, const unsigned t) :
        Y_SPADE_MESH_CTOR() 
        {
            checkDims();
        }
       
        
        
        
    }
    
}
