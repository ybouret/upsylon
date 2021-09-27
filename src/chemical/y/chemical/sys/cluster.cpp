
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
#include "y/sort/index.hpp"
#include "y/yap/gcd.hpp"

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

        typedef matrix<apz> zMatrix;

       
        
        void Cluster:: compile(const Lineage::Array &lineage) const
        {

            std::cerr << "Compiling Cluster..." << std::endl;
            assert(controls.size>0);
            assert(0==involved);

            const size_t N = controls.size;
            const size_t M = controls.head->nu.size();
            Flags        I(M,false);
            
            for(const Control *ctrl=controls.head;ctrl;ctrl=ctrl->next)
            {
                const iAccessible &nu = ctrl->nu;
                for(size_t j=M;j>0;--j)
                {
                    if(nu[j]!=0) I[j] = true;
                }
            }
            
            Lineage::Array source(M,as_capacity);
            Indices        sindex(M,as_capacity);
            
            for(size_t j=1;j<=M;++j)
            {
                if(I[j])
                {
                    aliasing::incr(involved);
                    source.push_back_(lineage[j]);
                    sindex.push_back_( (*source.back())->indx );
                }
            }
            std::cerr << "involved=" << involved << " / " << M << std::endl;
            std::cerr << "sindex=" << sindex << std::endl;
            const size_t m = involved;
            iMatrix      Nu(N,m);
            iMatrix      NuT(m,N);
            
            {
                size_t i=1;
                for(const Control *ctrl=controls.head;ctrl;ctrl=ctrl->next,++i)
                {
                    const iAccessible &nu = ctrl->nu;
                    for(size_t j=m;j>0;--j)
                    {
                        Nu[i][j] = nu[ sindex[j] ];
                    }
                }
            }
            NuT.assign_transpose(Nu);
            std::cerr << "Nu=" << Nu << std::endl;
            iMatrix Q(m,m);
            apk::compute_ortho_family(Q, Nu);
            std::cerr << "Q=" << Q << std::endl;

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
