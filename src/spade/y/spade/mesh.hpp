//! \file

#ifndef Y_SPADE_MESH_INCLUDED
#define Y_SPADE_MESH_INCLUDED 1

#include "y/spade/layout.hpp"
#include "y/string.hpp"

namespace upsylon {
    
    namespace Spade {

        namespace Kernel
        {
            //! base class for meshses
            class Mesh
            {
            public:
                virtual            ~Mesh()           throw();     //!< cleanup
                virtual const char *category() const throw() = 0; //!< info

                const string     name;            //!< identifier
                const unsigned   topology;        //!< logical dimensions
                const unsigned   space;           //!< physical dimensions
                
                static string    AxisTag(const unsigned); //!< build axis tag

            protected:
                //! setup
                explicit Mesh(const string &, const unsigned, const unsigned);
                //! setup
                explicit Mesh(const char   *, const unsigned, const unsigned);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Mesh);
                void checkDims() const;
            };
        }

        //----------------------------------------------------------------------
        //
        //! base class for Mesh
        //
        //----------------------------------------------------------------------
        template <typename COORD>
        class Mesh : public Kernel::Mesh
        {
        public:
            //! the topology is  the dimension of the required access coordinate
            static const unsigned Topology = Coord::Get<COORD>::Dimensions;
            
            //! cleanup
            inline virtual ~Mesh() throw(){}
            
        protected:
            //! setup
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

