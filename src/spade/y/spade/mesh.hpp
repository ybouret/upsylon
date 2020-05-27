//! \file

#ifndef Y_SPADE_MESH_INCLUDED
#define Y_SPADE_MESH_INCLUDED 1

#include "y/spade/layout.hpp"
#include "y/string.hpp"

namespace upsylon {
    
    namespace Spade {

        namespace Kernel
        {
            class Mesh
            {
            public:
                virtual ~Mesh() throw();
                virtual const char *category() const throw() = 0;

                const string     name;       //!< identifier
                const unsigned   topology;   //!< logical dimensions
                const unsigned   dimensions; //!< physical dimensions
                
                static string    AxisTag(const unsigned );

            protected:
                explicit Mesh(const string &, const unsigned, const unsigned);
                explicit Mesh(const char   *, const unsigned, const unsigned);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Mesh);
                void checkDims() const;
            };
        }

        
        template <typename COORD>
        class Mesh : public Kernel::Mesh
        {
        public:
            static const unsigned Topology = Coord::Get<COORD>::Dimensions;
            
            inline virtual ~Mesh() throw()
            {}
            
            
        protected:
            template <typename LABEL> inline
            explicit Mesh(const LABEL   &id,
                          const unsigned dim) :
            Kernel::Mesh(id,Topology,dim)
            {
            }
            
            
        private:
            Y_DISABLE_ASSIGN(Mesh);
        };

      

    }
    
}

#endif

