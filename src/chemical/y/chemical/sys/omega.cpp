
#include "y/chemical/system.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"
#include <iomanip>




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


        static inline
        void simplify( qShared &Q )
        {
            const apz fac = yap::lcm::of_denominators( & (*Q)[1], Q->size() );
            tao::mulset(*Q,fac);
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
                    std::cerr << "    " << *pp << " : " << pp.boundedText() << " | ";
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
                        simplify(Q);
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
                // at this point, an available replica is bounded
                //
                //--------------------------------------------------------------
                for(size_t j=M;j>0;--j)
                {
                    //----------------------------------------------------------
                    //
                    // take a species and its lineage
                    //
                    //----------------------------------------------------------
                    const Lineage &l = *lineage[j];
                    const Species &s = *l;
                    if( (s.rating<2) || ( !available[j]) ) continue;

                    assert(l.bounded);
                    assert(available[j]);
                    assert(remaining>0);
                    assert(freeSpace>0);

                    std::cerr << "    Looking at " << s << ", rating=" << s.rating << std::endl;

                    //----------------------------------------------------------
                    //
                    // prepare vector
                    //
                    //----------------------------------------------------------
                    qShared Q = new qVector(M,0);
                    (*Q)[j] = 1;

                    //----------------------------------------------------------
                    //
                    // loop over depending equilibria (wrapped in primaries)
                    //
                    //----------------------------------------------------------
                    for(size_t ii=l.primary.size();ii>0;--ii)
                    {
                        const Primary           &p       = *l.primary[ii]; assert(p.bounded);
                        const size_t             i       = p->indx;
                        const unit_t             nu      = NuT[j][i];         assert(nu!=0);
                        const Primary::Limiting *xlim[2] = { &p.reac, &p.prod };
                        const Actor             *xact    = NULL;
                        const apq                xfac    = -nu;

                        std::cerr << "      in " << *p << ", " << s << " is " << std::setw(3) << nu;
                        std::cerr << " : primary | reac = " << p.reac << " | prod=" << p.prod << std::endl;



                        if(nu<0)
                        {
                            // preferences for positive signs
                            cswap(xlim[0],xlim[1]);
                        }



                        for(size_t I=0;I<2;++I)
                        {
                            const Primary::Limiting &probe = *xlim[I];
                            if(probe.size())
                            {
                                xact = &probe.front();
                                break;
                            }
                        }

                        if(xact)
                        {
                            const size_t J = xact->sp.indx;
                            apq temp = NuT[J][i];
                            std::cerr << "        Found " << *xact <<  " : nu=" << temp << std::endl;
                            (*Q)[J] = xfac/temp;

                            if(available[J])
                            {
                                available[J] = false;
                                --remaining;
                            }

                        }
                        else
                        {
                            std::cerr << "        Not Found..." << std::endl;
                        }
                    }

                    //----------------------------------------------------------
                    //
                    // create constraint and updated counters
                    //
                    //----------------------------------------------------------

                    simplify(Q);
                    std::cerr << "    Om" << l->name << "=" << *Q << std::endl;
                    OmegaV.push_back(Q);
                    --freeSpace;
                    available[j] = false;
                    --remaining;
                    
                }


                if(freeSpace>0)
                {

                    qShared Q       =  new qVector(M,0);
                    unit_t  sumAbsZ = 0;
                    for(size_t j=M;j>0;--j)
                    {
                        const Species &s = **lineage[j];
                        const unit_t   z = s.charge;
                        (*Q)[j] = z;
                        sumAbsZ += abs_of(z);
                    }
                    std::cerr << "    Z=" << *Q << std::endl;
                    if(sumAbsZ)
                    {
                        OmegaV.push_back(Q);
                        --freeSpace;
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
                            Omega[i][j] = (*OmegaV[i])[j].num.cast_to<unit_t>("Omega");
                        }
                    }
                    std::cerr << "    Omega     = " << Omega     << std::endl;
                    std::cerr << "    remaining = " << remaining << std::endl;
                    std::cerr << "    freeSpace = " << freeSpace << std::endl;
                }



            }
            Y_CHEMICAL_PRINTLN("  <Omega/>");

        }
    }

}
