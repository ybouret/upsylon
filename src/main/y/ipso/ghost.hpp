//! \file
#ifndef Y_IPSO_GHOST_INCLUDED
#define Y_IPSO_GHOST_INCLUDED 1

#include "y/memory/cblock.hpp"
#include "y/ipso/field-io.hpp"
#include "y/sequence/vector.hpp"

namespace upsylon
{
    namespace ipso
    {

        //! a ghost for I/O
        class ghost
        {
        public:
            typedef vector<coord1D>           indices_type; //!< alias

            //! setup1D
            explicit ghost(const layout1D &full, const layout1D &sub, const size_t block_size=0);
            //! setup2D
            explicit ghost(const layout2D &full, const layout2D &sub, const size_t block_size=0);
            //! setup3D
            explicit ghost(const layout3D &full, const layout3D &sub, const size_t block_size=0);
            //!destructors
            virtual ~ghost() throw();
            //! memory change
            void ensure( const size_t block_size );

            //! initialize curr
            void init() throw();

            //! query data from a field
            void query( const field_info &F ) throw()
            {
                assert(allocated);
                assert(data);
                assert( data + F.item_size * indices.size() <= static_cast<uint8_t *>(workspace)+allocated );
                field_io::query(data,F.address(),F.item_size,indices);
            }

            // store loaded data info field
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ghost);
            uint8_t       *data;

            indices_type   indices;
            size_t         allocated;
            void          *workspace;

            void __release() throw();

        };


    }
}


#endif

