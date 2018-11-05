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

            //! query data from field
            template <typename T>
            inline size_t query( const field<T> &F ) throw()
            {
                assert(allocated);
                assert(data);
                const size_t iobytes = sizeof(T)*indices.size();
                assert( data + iobytes  <= static_cast<uint8_t *>(workspace)+allocated );
                field_io::query(data,F,indices);
                return iobytes;
            }

            //! store loaded data info field
            template <typename T>
            inline void store( field<T> &F ) throw()
            {
                assert(allocated);
                assert(data);
                assert(data+sizeof(T)*indices.size() <= static_cast<uint8_t *>(workspace)+allocated );
                field_io::store(F,data,indices);
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(ghost);
            uint8_t       *data;

            indices_type   indices;
            size_t         allocated;
            void          *workspace;

            void __release() throw();

        };

#if 0
        template <typename T>
        void field<T>:: save_into( ghost &gh ) const throw()
        {
            gh.query(*this);
        }

        template <typename T>
        void field<T>:: load_from( ghost &gh ) throw()
        {
            gh.store(*this);
        }
#endif
        

    }
}


#endif

