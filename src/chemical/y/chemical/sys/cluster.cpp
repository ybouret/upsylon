
#include "y/chemical/sys/cluster.hpp"
#include "y/mkl/tao.hpp"

namespace upsylon
{
    using namespace mkl;
    namespace Chemical
    {
        Cluster:: Control:: ~Control() throw()
        {
        }

        Cluster:: Control:: Control(const Primary &P, const iAccessible &NU) throw() :
        Object(), authority<const Primary>(P),
        nu(NU), next(0), prev(0)
        {
        }


        bool Cluster:: overlaps(const iAccessible &nu) const throw()
        {
            for(const Control *comp=controls.head;comp;comp=comp->next)
            {
                if( tao::dot<unit_t>::of(nu,comp->nu) != 0 ) return true;
            }
            return false;
        }



    }

}

#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/apk.hpp"

namespace upsylon
{
    namespace Chemical
    {
        Cluster:: ~Cluster() throw()
        {
        }

        Cluster:: Cluster() throw() :
        controls(),
        involved(0)
        {}
        
        void Cluster:: grow(const Primary &p, const iAccessible &nu)
        {
            aliasing::_(controls).push_back( new Control(p,nu) );
        }


        void Cluster:: compile() const
        {
            assert(controls.size>0);
            assert(0==involved);

            const size_t N = controls.size;
            const size_t M = controls.head->nu.size();

            iMatrix Nu(N,M);
            {
                size_t i=1;
                for(const Control *ctrl=controls.head;ctrl;ctrl=ctrl->next,++i)
                {
                    tao::set(Nu[i],ctrl->nu);
                }
            }
            for(size_t j=M;j>0;--j)
            {
                for(size_t i=N;i>0;--i)
                {
                    if(Nu[i][j]) aliasing::incr(involved);
                }
            }
            std::cerr << "localNu  = " << Nu << std::endl;
            std::cerr << "involved = " << involved << std::endl;
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


        Cluster * Clusters:: start(const Primary &p, const iAccessible &nu)
        {
            Cluster *cls = push_back( new Cluster() );
            cls->grow(p,nu);
            return cls;
        }

    }

}
