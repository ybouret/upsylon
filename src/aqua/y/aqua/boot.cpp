#include "y/aqua/boot.hpp"



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

        void Constraint:: fill(addressable<int> &P) const throw()
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
            os << cc.value << '=';
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
        Boot:: Boot() throw() : Constraint::List(), P(), S(), keep()
        {
            keep << aliasing::_(P);
            keep << aliasing::_(S);
        }

        Boot:: ~Boot() throw()
        {
        }

        void Boot:: quit() throw()
        {
            keep.release_all();
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


    }

}

#include "y/aqua/library.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/mkl/kernel/gram-schmidt.hpp"

namespace upsylon {

    namespace Aqua
    {

        using namespace mkl;

        bool  Boot:: init(Library &lib)
        {
            quit();
            try {
                const size_t M  = lib.entries();
                if(M<=0)
                {
                    return true;
                }
                lib.buildIndices();
                const size_t Nc = size;
                if(Nc<=0)
                {
                    return false;
                }
                aliasing::_(P).make(Nc,M);
                {
                    size_t i=1;
                    for(const Constraint *cc=head;cc;cc=cc->next,++i)
                    {
                        cc->fill( aliasing::_(P[i]) );
                    }
                }

                std::cerr << "P=" << P << std::endl;


                if(Nc<M)
                {

                    const size_t N = M-Nc;
                    aliasing::_(S).make(N,M);
                    iMatrix      R(P);

                    if(!GramSchmidt::iOrtho(R))
                    {
                        return false;
                    }
                    std::cerr << "R=" << R << std::endl;

                    iMatrix      I(N,N);
                    for(size_t i=1;i<=Nc;++i)
                    {
                        quark::set(I[i],P[i]);
                    }
                }

                return true;
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

