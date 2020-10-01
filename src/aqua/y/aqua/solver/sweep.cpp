
#include "y/aqua/equilibrium.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/sort/heap.hpp"
#include "y/mkl/root/bisection.hpp"

namespace upsylon
{

    namespace Aqua
    {
        using namespace mkl;

        namespace {

            static inline
            double find_maximum_extent(const accessible<double> &C,
                                       const Component::List    &components,
                                       double                    arr[]) throw()
            {
                size_t num=0;
                for(const Component *r=components.head;r;r=r->next)
                {
                    const size_t j  = r->sp.indx;
                    const double Cj = C[j];
                    const int    nu = r->nu; assert(nu>0);
                    arr[num++] = Cj/nu;
                }
                hsort(arr,num,comparison::increasing<double>);
                return *arr;
            }

            class extent
            {
            public:
                const bool   limited;
                const double maximum;

                inline extent(const accessible<double> &C,
                              const Component::List    &components,
                              double                    arr[]) throw() :
                limited(components.size>0),
                maximum(limited ? find_maximum_extent(C,components,arr) : 0)
                {
                }

                inline ~extent()
                {

                }

                inline friend std::ostream & operator<<(std::ostream &os, const extent &E)
                {
                    if(E.limited)
                    {
                        os << "limited@" << E.maximum;
                    }
                    else
                    {
                        os << "unlimited";
                    }
                    return os;
                }



            private:
                Y_DISABLE_COPY_AND_ASSIGN(extent);
            };

            struct E_proxy
            {
                const Equilibrium        *self;
                const accessible<double> *pC0;
                addressable<double>      *pC1;
                double                    K0;

                inline double operator()(const double x) throw()
                {
                    self->evolve(*pC1,*pC0,x);
                    return self->computeQ(K0,*pC1);
                }

            };

        }

        void   Equilibrium:: evolve(addressable<double> &Ctry, const accessible<double> &C0, const double x) const throw()
        {
            quark::set(Ctry,C0);
            for(const Component *c=components.head;c;c=c->next)
            {
                const size_t j = c->sp.indx;
                Ctry[j] = max_of(0.0,Ctry[j]+c->nu*x);
            }
        }


        static const char fn[] = "[ sweep ] ";

#define Y_AQUA_PRINTLN(MSG) do { if(verbose) { std::cerr << fn << MSG << std::endl; } } while(false)


        bool Equilibrium:: sweep(const double         K0,
                                 addressable<double> &C0,
                                 double               arr[],
                                 const bool           verbose) const
        {
            Y_AQUA_PRINTLN("sweep <" << name << ">");

            const extent fwd(C0,reactants,arr);
            const extent rev(C0,products,arr);
            Y_AQUA_PRINTLN( "\tforward: " << fwd << " | reverse: " << rev);
            static const unsigned FWD_LIMITED   = 0x01;
            static const unsigned FWD_UNLIMITED = 0x02;
            static const unsigned REV_LIMITED   = 0x04;
            static const unsigned REV_UNLIMITED = 0x08;

            unsigned flags = 0;
            flags |= fwd.limited ? FWD_LIMITED : FWD_UNLIMITED;
            flags |= rev.limited ? REV_LIMITED : REV_UNLIMITED;

            double xmin = 0;
            double xmax = 0;
            lightweight_array<double> C1(arr,C0.size());

            E_proxy F = { this, &C0, &C1, K0 };
            double  Fmin = 0;
            double  Fmax = 0;

            switch(flags)
            {
                case FWD_LIMITED | REV_LIMITED:
                    if(fwd.maximum<=0&&rev.maximum<=0)
                    {
                        Y_AQUA_PRINTLN("\t<<blocked>>");
                        return false;
                    }
                    else
                    {
                        xmin = -rev.maximum; Fmin = F(xmin);
                        xmax =  fwd.maximum; Fmax = F(xmax);

                    }
                    break;

                case FWD_LIMITED | REV_UNLIMITED:
                    xmax = fwd.maximum; Fmax = F(xmax);
                    while( (Fmin=F(xmin))*Fmax >0 ) xmin -= 1.0;
                    break;

                case FWD_UNLIMITED | REV_LIMITED:
                    xmin = -rev.maximum; Fmin = F(xmin);
                    while( (Fmax=F(xmax))*Fmin >0 ) xmax += 1.0;
                    break;

                default:
                    Y_AQUA_PRINTLN("\t!!corrupted!!");
                    return false;
            }

            Y_AQUA_PRINTLN("\tF(" << xmin << ")=" << Fmin << " | F(" << xmax << ")=" << Fmax);

            triplet<double>   x = { xmin, 0, xmax };
            triplet<double>   f = { Fmin, 0, Fmax };
            bisection<double> zsolve;

            if( !zsolve(F,x,f) )
            {
                Y_AQUA_PRINTLN("failure");
                return false;
            }

            quark::set(C0,C1);
            Y_AQUA_PRINTLN("<" << name << "> = " << f.b << " @ " << C0);


            return true;
        }



    }

}

#include "y/aqua/solver.hpp"

namespace upsylon
{

    namespace Aqua
    {
        using namespace mkl;

        bool Solver:: sweep(addressable<double> &C, const size_t i) throw()
        {
            assert(C.size()>=M);
            for(size_t j=M;j>0;--j)
            {
                Cswp[j] = C[j];
            }
            if( equilibria[i]->sweep(K[i], Cswp, *Caux, forwardVerbose) )
            {
                for(size_t j=M;j>0;--j)
                {
                    C[j] = Cswp[j];
                }
                return true;
            }
            else
                return false;
        }

        bool Solver:: swept(addressable<double> &C) throw()
        {
            assert(C.size()>=M);
            for(size_t j=M;j>0;--j)
            {
                Cswp[j] = C[j];
            }

            bool success = false;
            for(size_t i=N;i>0;--i)
            {
                if( equilibria[i]->sweep(K[i], Cswp, *Caux, forwardVerbose) )
                {
                    success = true;
                }
            }
            if(success)
            {
                for(size_t j=M;j>0;--j)
                {
                    C[j] = Cswp[j];
                }
                return true;
            }
            else
            {
                return false;
            }

        }



        
    }

}

