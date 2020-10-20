#include "y/aqua/boot.hpp"
#include "y/mkl/kernel/svd.hpp"
#include "y/counting/comb.hpp"

namespace upsylon {

    namespace Aqua
    {

        Constraint::Actor:: ~Actor() throw()
        {
            aliasing::_(weight) = 0;
        }

        Constraint:: Actor:: Actor(const Species &s, const int w) throw() :
        species(s),
        weight(w)
        {

        }

        void Constraint:: fill(addressable<Int> &P) const throw()
        {
            for(const Actor *a=actors.head;a;a=a->next)
            {
                P[a->species.indx] = a->weight;
            }
        }


        std::ostream & operator<<(std::ostream &os, const Constraint::Actor &a)
        {
            const int coef = a.weight;
            switch(coef)
            {
                case 1: os << '+'; break;
                case 0: return os;
                case -1: os << '-'; break;
                default:
                    if(coef>0)
                    {
                        assert(coef>1);
                        os << '+' << coef << '*';
                    }
                    else
                    {
                        assert(coef< -1);
                        os << coef << '*';
                    }

            }

            os << '[' << a.species.name << ']';
            return os;
        }

    }

}

#include <iomanip>

namespace upsylon {

    namespace Aqua
    {
        Constraint:: Constraint(const double v) throw() :
        value(v),
        actors()
        {
        }

        Constraint:: ~Constraint() throw()
        {
        }

        void Constraint:: add(const Species &sp, const int weight)
        {

            if(weight!=0)
            {
                for(Actor *actor=actors.head;actor;actor=actor->next)
                {
                    if(&(actor->species) == &sp)
                    {
                        aliasing::_(actor->weight) += weight;
                        if(actor->weight==0)
                        {
                            delete actors.unlink(actor);
                        }
                    }
                }
                actors.push_back( new Actor(sp,weight) );
            }
        }

        std::ostream & operator<<(std::ostream &os, const Constraint &cc)
        {
            os << std::setw(8) << cc.value << '=';
            //const size_t nc = cc.actors.size;
            size_t       i  = 0;
            for(const Constraint::Actor *a=cc.actors.head;a;a=a->next,++i)
            {
                os << *a;
            }
            return os;
        }

    }

}

namespace upsylon {

    namespace Aqua
    {
        Boot:: Boot() throw() :
        Constraint::List(),
        R(),
        L(),
        d(0),
        S(),
        dS(0),
        pS(),
        keep()
        {
            keep << aliasing::_(R);
            keep << aliasing::_(L);
            keep << aliasing::_(S);
            keep << aliasing::_(pS);
        }

        Boot:: ~Boot() throw()
        {
        }

        void Boot:: quit() throw()
        {
            keep.release_all();
            aliasing::_(d) = 0;
        }

        Constraint & Boot:: operator()( const double value )
        {
            return * push_back( new Constraint(value) );
        }

        void Boot:: conserve(const double C0, const Species &a)
        {
            Constraint &cc = (*this)(C0);
            cc.add(a,1);
        }

        void Boot:: conserve(const double C0, const Species &a, const Species &b)
        {
            Constraint &cc = (*this)(C0);
            cc.add(a,1);
            cc.add(b,1);
        }

        void Boot:: conserve(const double C0, const Species &a, const Species &b, const Species &c)
        {
            Constraint &cc = (*this)(C0);
            cc.add(a,1);
            cc.add(b,1);
            cc.add(c,1);
        }


        void Boot:: conserve(const double C0, const Species &a, const Species &b, const Species &c, const Species &d)
        {
            Constraint &cc = (*this)(C0);
            cc.add(a,1);
            cc.add(b,1);
            cc.add(c,1);
            cc.add(d,1);
        }


        std::ostream & operator<<(std::ostream &os, const Boot &b)
        {
            os << "<boot #" << b.size << ">" << std::endl;
            for(const Constraint *cc=b.head;cc;cc=cc->next)
            {
                os << ' ' << ' ' << *cc << std::endl;
            }
            os <<  "<boot/>";
            return os;
        }

    }

}

#include "y/aqua/equilibria.hpp"
#include "y/aqua/library.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/gram-schmidt.hpp"
#include "y/mkl/kernel/adjoint.hpp"
#include "y/mkl/simplify.hpp"
#include "y/exception.hpp"
#include "y/aqua/engine.hpp"

namespace upsylon {

    namespace Aqua
    {

        using namespace mkl;

        void Boot:: fill(addressable<double> &Lambda) const throw()
        {
            assert(Lambda.size() == size);
            size_t i=1;
            for(const Constraint *cc=head;cc;cc=cc->next,++i)
            {
                Lambda[i] = cc->value;
            }
        }

        void  Boot:: init(Library          &lib,
                          const Equilibria &eqs)
        {
            static const char fn[] = "Aqua::Boot::init: ";
            quit();
            try
            {
                //--------------------------------------------------------------
                // parameters for R
                //--------------------------------------------------------------
                const size_t M  = lib.entries();
                const size_t N  = eqs.entries();
                const size_t Nc = size;
                if(N+Nc!=M)
                {
                    throw exception("%s: #constraint=%lu + #equilibrium=%lu != #species=%lu",fn, (unsigned long)Nc, (unsigned long)N, (unsigned long)M);
                }
                aliasing::_(R).make(Nc,M);
                aliasing::_(L).make(M,Nc);
                aliasing::_(S).make(N,M);
                aliasing::_(pS).make(M,M);

                lib.buildIndices();
                {
                    size_t i=1;
                    for(const Constraint *cc=head;cc;cc=cc->next,++i)
                    {
                        cc->fill( aliasing::_(R[i]) );
                    }
                }

                {
                    iMatrix R2(Nc,Nc);
                    tao::gram(R2,R);
                    aliasing::_(d) = ideterminant(R2);
                    if(0==d)
                    {
                        throw exception("%ssingular set of constraints",fn);
                    }
                    iMatrix aR2(Nc,Nc);
                    iadjoint(aR2,R2);
                    iMatrix tR(R,matrix_transpose);
                    tao::mmul(aliasing::_(L),tR,aR2);
                    (void) simplify<Int>::on( aliasing::_(L), aliasing::_(d) );
                }

                {
                    iMatrix F(M,M);
                    for(size_t i=Nc;i>0;--i)
                    {
                        tao::set(F[i],R[i]);
                    }
                    {
                        size_t i=1+Nc;
                        for(Equilibria::const_iterator it=eqs.begin();it!=eqs.end();++it,++i)
                        {
                            (**it).fillNu(F[i]);
                        }
                    }
                    if(!GramSchmidt::iOrtho(F))
                    {
                        throw exception("%sinvalid set of constraints",fn);
                    }
                    for(size_t i=N;i>0;--i)
                    {
                        tao::set(aliasing::_(S)[i],F[i+Nc]);
                    }
                }

                std::cerr << "R=" << R << std::endl;
                std::cerr << "L=" << L << std::endl;
                std::cerr << "d=" << d << std::endl;
                std::cerr << "S=" << S << std::endl;


                aliasing::_(dS) = Engine::Project(aliasing::_(pS),S,"supplementary boot space");
                



            }
            catch(...)
            {
                quit();
                throw;
            }
        }


    }

}


namespace upsylon {

    namespace Aqua
    {
        void Boot:: electroneutrality(const Library &lib)
        {
            Constraint &EN = (*this)(0);
            for(Library::const_iterator it=lib.begin();it!=lib.end();++it)
            {
                const Species &species = **it;
                EN.add(species,species.Z);
            }
        }
    }

}

namespace upsylon {

    namespace Aqua
    {
        bool  Boot:: find(addressable<double> &C, Engine &engine) throw()
        {
            assert(C.size()>=engine.M);

            // build Cstar
            const size_t   M  = engine.M;
            const size_t   Nc = size;
            vector<double> Cstar(M,0);
            vector<double> Lambda(Nc,0);

            fill(Lambda);
            tao::mul(Cstar,L,Lambda);
            tao::divset(Cstar,d);
            std::cerr << "Lambda=" << Lambda << std::endl;
            std::cerr << "Cstar =" << Cstar  << std::endl;

            return false;
        }

    }
}

