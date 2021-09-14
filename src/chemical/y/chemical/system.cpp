
#include "y/chemical/system.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/sort/unique.hpp"
#include "y/core/dnode.hpp"

#include "y/counting/comb.hpp"

#include <iomanip>

namespace upsylon
{

    namespace Chemical
    {

        Lineage:: ~Lineage() throw()
        {
        }

        Lineage:: Lineage( const Species &sp ) :
        authority<const Species>(sp),
        bounded(true),
        primary(sp.rating,as_capacity)
        {

        }


        void Lineage:: link(const Primary::Pointer &p) throw()
        {
            aliasing::_(primary).push_back_(p);
            if(!p->bounded)
            {
                aliasing::_(bounded) = false;
            }
        }

    }

}


namespace upsylon
{
    using namespace mkl;

    namespace Chemical
    {

        const char System:: CLID[] = "Chemical::System";
        const char System:: PrimaryEnter[] = "<Primary>";
        const char System:: PrimaryLeave[] = "<Primary/>";
        const char System:: ReplicaEnter[] = "<Replica>";
        const char System:: ReplicaLeave[] = "<Replica/>";

        const char System:: Success[] = "Success";
        const char System:: Failure[] = "Failure";
        
        const char * System:: Outcome(const bool flag) throw()
        {
            return flag ? Success : Failure;
        }

        System:: ~System() throw()
        {
        }


        static inline
        size_t checkValidity(const size_t MW, const size_t N)
        {
            if(N>MW) throw exception("%s has too many equilibria/working species",System::CLID);
            return MW-N;
        }

        System:: System(const Library    &usrLib,
                        const Equilibria &usrEqs,
                        const unsigned    flags ) :
        lib(usrLib),
        eqs(usrEqs),
        N(  eqs->size() ),
        M(  lib->size() ),
        MW( lib.countWorking()  ),
        Nc( checkValidity(MW,N) ),
        MP( lib.countPrimary()  ),
        MR( lib.countReplica()  ),
        MS( lib.spectators()    ),
        Nu(N,N>0?M:0),
        NuT(Nu.cols,Nu.rows),
        primary(N, as_capacity),
        lineage(M,as_capacity),
        xi(N,0),
        ok(N,false),
        who(N,as_capacity),
        libLatch( aliasing::_(lib) ),
        eqsLatch( aliasing::_(eqs) )
        {

            Y_CHEMICAL_PRINTLN("<System>");
            Y_CHEMICAL_PRINTLN("  N   = " << std::setw(3) << N  << " # equilibria");
            Y_CHEMICAL_PRINTLN("  M   = " << std::setw(3) << M  << " # species");
            Y_CHEMICAL_PRINTLN("  MW  = " << std::setw(3) << MW << " # working species");
            Y_CHEMICAL_PRINTLN("  Nc  = " << std::setw(3) << Nc << " # complementaries");
            Y_CHEMICAL_PRINTLN("  MP  = " << std::setw(3) << MP << " # primary species");
            Y_CHEMICAL_PRINTLN("  MR  = " << std::setw(3) << MR << " # replica species");
            Y_CHEMICAL_PRINTLN("  MS  = " << std::setw(3) << MS << " # spectators");

            assert(N+Nc==MW);
            assert(MP+MR==MW);
            assert(MW+MS==M);

            if(N>0)
            {
                //--------------------------------------------------------------
                //
                // checking equilibria
                //
                //--------------------------------------------------------------
                eqs.verify(flags);
                eqs.fill( aliasing::_(Nu) );
                aliasing::_(NuT).assign_transpose(Nu);
                Y_CHEMICAL_PRINTLN("  Nu  = " << Nu);
                Y_CHEMICAL_PRINTLN("  NuT = " << NuT);

                {
                    Matrix Nu2(N,N);
                    tao::gram(Nu2,Nu);
                    if(!LU::build(Nu2))
                    {
                        throw exception("%s has singular equilibria",CLID);
                    }
                }

                //--------------------------------------------------------------
                //
                // building primary
                //
                //--------------------------------------------------------------
                Y_CHEMICAL_PRINTLN("  " << PrimaryEnter);;
                for(const ENode *node=eqs->head();node;node=node->next)
                {
                    const Equilibrium     &eq = ***node;
                    const Primary::Pointer pp = new Primary(eq,NuT);
                    aliasing::_(primary).push_back_(pp);
                    if(Verbosity) pp->display(std::cerr,4);

                }
                Y_CHEMICAL_PRINTLN("  " << PrimaryLeave);;

                
                //--------------------------------------------------------------
                //
                // building lineage
                //
                //--------------------------------------------------------------
                Y_CHEMICAL_PRINTLN("  <Lineage>");
                for(const SNode *node=lib->head();node;node=node->next)
                {
                    const Species     &sp = ***node;
                    const iAccessible &nu = NuT[sp.indx];
                    Lineage           *l = new Lineage(sp);
                    Lineage::Pointer  lp = l;
                    std::cerr << "    " << sp << " #" << std::setw(3) << sp.rating << " :";
                    aliasing::_(lineage).push_back_(lp);
                    for(size_t i=1;i<=N;++i)
                    {
                        if(nu[i])
                        {
                            l->link(primary[i]);
                            std::cerr << ' ' << (**primary[i]).name;
                        }
                    }
                    
                    std::cerr << " => " << Primary::BoundedText(l->bounded) << std::endl;

                }

                Y_CHEMICAL_PRINTLN("  <Lineage/>");

                buildOmega();



            }

            Y_CHEMICAL_PRINTLN("<System/>");

        }
        

    }

}

#include "y/mkl/kernel/apk.hpp"
#include "y/yap/lcm.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Chemical
    {

        typedef vector<apq,Allocator> qVector;
        typedef arc_ptr<qVector>      qShared;

        template <typename ACTORS>
        static inline
        size_t disable(Flags &available, const ACTORS &arr)
        {
            const size_t n=arr.size();
            for(size_t jj=n;jj>0;--jj)
            {
                const Actor   &a = arr[jj];
                const Species &s = a.sp;
                const size_t   j = s.indx;
                assert(true==available[j]);
                available[j] = false;
            }
            return n;
        }

        void System::buildOmega()
        {
            Y_CHEMICAL_PRINTLN("  <Omega>");
            if(M>N)
            {
                Flags  available(M,true); //!< array of available species
                size_t remaining = M;     //!< count of remaining species
                size_t freeSpace = M-N;   //!< complementary degrees of freedom
                vector<qShared,Allocator> OmegaV(freeSpace,as_capacity);


                //--------------------------------------------------------------
                //
                // remove endless species
                //
                //--------------------------------------------------------------
                for(size_t j=M;j>0;--j)
                {
                    const Lineage &l = *lineage[j];
                    if(!l.bounded)
                    {
                        assert(remaining);
                        assert(available[j]);
                        available[j] = false;
                        --remaining;
                    }
                }


                //--------------------------------------------------------------
                //
                // transform spectators into unit constraint
                //
                //--------------------------------------------------------------
                for(size_t j=M;j>0;--j)
                {
                    const Lineage &l = *lineage[j];
                    if(l->rating<=0)
                    {
                        // this is a spectator
                        assert(l.bounded);
                        assert(remaining);
                        assert(available[j]);
                        assert(freeSpace);

                        qShared Q = new qVector(M,0);
                        OmegaV.push_back(Q);
                        (*Q)[j]       = 1;
                        available[j]  = false;
                        --freeSpace;
                        --remaining;
                    }

                }
                assert(Nc==freeSpace);




                lib.display(std::cerr << "    available0=" << std::endl,available,4) << std::endl;


                //--------------------------------------------------------------
                //
                // first pass: check primary constraints
                //
                //--------------------------------------------------------------
                for(size_t i=1;i<=N;++i)
                {
                    const Primary &pp = *primary[i];
                    std::cerr << "    " << *pp << " : " << pp.boundedText() << " => ";
                    if(Primary::LimitedByBoth==pp.kind)
                    {
                        std::cerr << "may conserve primary" << std::endl;
                        qShared Q = new qVector(M,0);
                        OmegaV.push_back(Q);
                        // take first item a indicator
                        const Actor &lhs = pp.reac.front();
                        const Actor &rhs = pp.prod.front();
                        (*Q)[lhs.sp.indx] = apq(1,lhs.nu);
                        (*Q)[rhs.sp.indx] = apq(1,rhs.nu);

                        // disable all primaries
                        assert(pp.reac.size()+pp.prod.size()<=remaining);
                        remaining -= disable(available,pp.reac);
                        remaining -= disable(available,pp.prod);

                        // simplify
                        {
                            const apz fac = yap::lcm::of_denominators( & (*Q)[1], M);
                            tao::mulset(*Q,fac);
                        }
                        --freeSpace;
                    }
                    else
                    {
                        std::cerr << "no primary conservation" << std::endl;
                    }
                }

                lib.display(std::cerr << "    available1=" << std::endl,available,4) << std::endl;


                //--------------------------------------------------------------
                //
                // second pass: replica
                //
                //--------------------------------------------------------------
                for(size_t j=M;j>0;--j)
                {
                    const Lineage &l = *lineage[j];
                    const Species &s = *l;
                    if( (s.rating<2) || ( !available[j]) ) continue;
                    assert(l.bounded);
                    std::cerr << "    Looking at " << s << ", rating=" << s.rating << std::endl;
                    const iAccessible &nut = NuT[j];

                    for(size_t ii=l.primary.size();ii>0;--ii)
                    {
                        const Primary     &p  = *l.primary[ii]; assert(p.bounded);
                        const size_t       i  = p->indx;
                        const unit_t       nu = nut[i];
                        std::cerr << "      in " << *p << ", " << s << " is " << nu << std::endl;
                    }


                }


                const size_t dof = OmegaV.size();
                if(dof)
                {
                    iMatrix Omega(dof,M);
                    for(size_t i=dof;i>0;--i)
                    {
                        for(size_t j=M;j>0;--j)
                        {
                            Omega[i][j] = (*OmegaV[i])[j].num.cast_to<unit_t>();
                        }
                    }
                    std::cerr << "    Omega     = " << Omega     << std::endl;
                    std::cerr << "    remaining = " << remaining << std::endl;
                    std::cerr << "    freeSpace = " << freeSpace << std::endl;
                }



                vector<unit_t,Allocator> Z(M,0);
                unit_t sz=0;
                for(size_t j=M;j>0;--j)
                {
                    const Species &s = **lineage[j];
                    const unit_t   z = s.charge;
                    Z[j] = z;
                    sz  += abs_of(z);
                }
                std::cerr << "Z=" << Z << std::endl;

            }
            Y_CHEMICAL_PRINTLN("  <Omega/>");

        }
    }

}
