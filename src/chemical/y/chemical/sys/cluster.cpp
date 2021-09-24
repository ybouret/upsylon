
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

            iMatrix        Nu(N,M);
            iMatrix        NuT(M,N);
            iMatrix        Nu2(N,N);
            zMatrix        aNu2(N,N);
            zMatrix        aNu3(N,M);
            zMatrix        P(M,M);
            zMatrix        zQT(M,M);
            iMatrix        QT(M,M);
            Lineage::Array source(M,as_capacity);
            Indices        rating(M,as_capacity);
            Indices        spRank(M,as_capacity);

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
                    if(Nu[i][j])
                    {
                        aliasing::incr(involved);
                        const Lineage::Pointer &S = lineage[j];
                        const Species          &s = **S;
                        source.push_back(S);
                        rating.push_back(s.rating);
                        break;
                    }
                }
            }

            NuT.assign_transpose(Nu);
            spRank.adjust(involved,0);
            indexing::make(spRank,comparison::increasing<size_t>,rating);

            std::cerr << "localNu  = " << Nu << std::endl;
            std::cerr << "involved = " << involved << std::endl;
            assert(involved==source.size());
            std::cerr << "  {";
            for(size_t i=1;i<=involved;++i)
            {
                std::cerr << ' ' << (*source[i])->name;
            }
            std::cerr << " }" << std::endl;



            apz dNu2 = apk::adjoint_gram(aNu2,Nu);
            if(dNu2<=0)
            {
                std::cerr << "No ortho space" << std::endl;
                return;
            }
            std::cerr << "aNu2=" << aNu2 << "/" << dNu2 << std::endl;
            tao::mmul(aNu3,aNu2,Nu);
            tao::mmul(P,NuT,aNu3);
            std::cerr << "P=" << P << "/" << dNu2 << std::endl;

            for(size_t i=1;i<=M;++i)
            {
                for(size_t j=1;j<=M;++j)
                {
                    if(i==j) zQT[i][i] = dNu2 - P[i][i];
                }
            }
            std::cerr << "zQT=" << zQT << std::endl;



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
