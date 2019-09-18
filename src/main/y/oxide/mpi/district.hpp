//! \file

#ifndef Y_OXIDE_MPI_DISTRICT_INCLUDED
#define Y_OXIDE_MPI_DISTRICT_INCLUDED 1

#include "y/oxide/field/split.hpp"
#include "y/oxide/mpi/domain.hpp"
#include "y/oxide/mpi/realm.hpp"

namespace upsylon
{

    namespace Oxide
    {
        template <typename COORD>
        class District :
        public Split<COORD>, public Domain<COORD>
        {
        public:
            typedef Split<COORD>  SplitType;
            typedef Realm<COORD>  RealmType;
            typedef Layout<COORD> LayoutType;
            typedef Domain<COORD> DomainType;

            typedef typename LayoutType::coord       coord;
            typedef typename LayoutType::const_coord const_coord;

            RealmType *realm;

            //! setup domain and create realm at head node
            explicit District(mpi              &_MPI,
                              const LayoutType &fullLayout,
                              const_coord       boundaries,
                              const size_t      ghostsZone,
                              coord             preferred) :
            SplitType(_MPI.size,fullLayout,boundaries,preferred),
            DomainType(_MPI,fullLayout,this->favored,boundaries,ghostsZone),
            realm(0)
            {
                if(this->MPI.isHead)
                {
                    realm = new RealmType(_MPI,fullLayout,this->favored,boundaries,ghostsZone);
                }
            }

            //! cleanup
            virtual ~District() throw()
            {
                if(realm)
                {
                    delete realm;
                    realm = 0;
                }
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(District);
        };
    }


}

#endif

