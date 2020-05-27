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
            const string     name;
            const unsigned   dimensions; //!< [1|2|3]
            const unsigned   topology;   //!< topology<=dimension
            virtual ~Mesh() throw();
            
        protected:
            explicit Mesh(const string &, const unsigned, const unsigned);
            explicit Mesh(const char   *, const unsigned, const unsigned);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Mesh);
            void checkDims() const;
        };
        
        
        template <typename COORD>
        class MeshOf : public Mesh
        {
        public:
            static const unsigned Dimensions = Coord::Get<COORD>::Dimensions;
            
            inline virtual ~MeshOf() throw()
            {}
            
            
        protected:
            template <typename LABEL> inline
            explicit MeshOf(const LABEL &id, const unsigned topo) :
            Mesh(id,Dimensions,topo)
            {
            }
            
            
        private:
            Y_DISABLE_ASSIGN(MeshOf);
        };
        
    }
    
}

#endif

