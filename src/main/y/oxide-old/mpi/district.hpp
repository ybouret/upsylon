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
        //! a domain built from preferences and a realm at master node
        template <typename COORD>
        class District :
        public Split<COORD>, public Domain<COORD>
        {
        public:
            static  const size_t  Dimensions = Coord::Get<COORD>::Dimensions; //!< alias
            typedef Split<COORD>  SplitType;                                  //!< alias
            typedef Realm<COORD>  RealmType;                                  //!< alias
            typedef Layout<COORD> LayoutType;                                 //!< alias
            typedef Domain<COORD> DomainType;                                 //!< alias
            typedef typename LayoutType::coord       coord;                   //!< alias
            typedef typename LayoutType::const_coord const_coord;             //!< alias

            RealmType *realm; //!< realm is allocated

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

            //! create a field in the domain and in the realm with same type/id
            /**
             return the local field
             */
            template <typename FIELD>
            inline FIELD &createExported( const string &id )
            {
                if(realm)
                {
                    realm->template create<FIELD>(id);
                }
                return this->template create<FIELD>(id);
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

            //! scatter a field with parentName into each child with childName
            inline void Scatter(const string &parentName,
                                const string &childName)
            {
                RealmType::Scatter(this->MPI,realm, parentName,*this,childName);
            }

            //! scatter a field with the same name in the realm and domains
            inline void Scatter(const string &name)
            {
                Scatter(name,name);
            }

            //! gather a field with parentName from each child with childName
            inline void Gather(const string &parentName,
                               const string &childName)
            {
                RealmType::Gather(this->MPI,realm, parentName,*this,childName);
            }

            //! gather a field with the same name in the realm and domains
            inline void Gather(const string &name)
            {
                Gather(name,name);
            }




        private:
            Y_DISABLE_COPY_AND_ASSIGN(District);
        };
    }


}

#endif

