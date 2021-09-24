
#include "y/chemical/sys/cluster.hpp"
#include "y/mkl/tao.hpp"

namespace upsylon
{
    using namespace mkl;
    namespace Chemical
    {
        Cluster:: Component:: ~Component() throw()
        {
        }

        Cluster:: Component:: Component(const Primary &P, const iAccessible &NU) throw() :
        Object(), authority<const Primary>(P),
        nu(NU), next(0), prev(0)
        {
        }


        bool Cluster:: overlaps(const iAccessible &nu) const throw()
        {
            for(const Component *comp=components.head;comp;comp=comp->next)
            {
                if( tao::dot<unit_t>::of(nu,comp->nu) != 0 ) return true;
            }
            return false;
        }



    }

}

namespace upsylon
{
    namespace Chemical
    {
        Cluster:: ~Cluster() throw()
        {
        }

        Cluster:: Cluster() throw() : components() {}
        
        void Cluster:: grow(const Primary &p, const iAccessible &nu)
        {
            aliasing::_(components).push_back( new Component(p,nu) );
        }

    }
}


namespace upsylon
{
    namespace Chemical
    {

        Clusters:: ~Clusters() throw() {}

        Clusters:: Clusters() throw() : Clusters_()
        {

        }


        Cluster * Clusters:: init(const Primary &p, const iAccessible &nu)
        {
            Cluster *cls = push_back( new Cluster() );
            cls->grow(p,nu);
            return cls;
        }

    }

}
