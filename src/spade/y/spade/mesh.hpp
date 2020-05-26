//! \file

#ifndef Y_SPADE_MESH_INCLUDED
#define Y_SPADE_MESH_INCLUDED 1

#include "y/spade/layout.hpp"
#include "y/string.hpp"

namespace upsylon {
    
    namespace Spade {
        
        class Mesh
        {
        public:
            const unsigned   dimensions;
            const string     name;
            virtual ~Mesh() throw();
            
        protected:
            explicit Mesh(const unsigned,const string &);
            explicit Mesh(const unsigned,const char   *);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Mesh);
        };
        
        
        template <typename COORD>
        class MeshOf : public Mesh
        {
        public:
            static const unsigned Dimensions = Coord::Get<COORD>::Dimensions;
            
            inline virtual ~MeshOf() throw()
            {}
            
            
        protected:
            
            
        private:
            Y_DISABLE_ASSIGN(MeshOf);
        };
        
    }
    
}

#endif

