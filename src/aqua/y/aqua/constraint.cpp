
#include "y/aqua/constraint.hpp"

namespace upsylon
{

    namespace Aqua
    {
        Constraint::Actor:: ~Actor() throw()
        {
            aliasing::_(weight) = 0;
        }

        Constraint:: Actor:: Actor(const Species &s, const Int w) throw() :
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

namespace upsylon
{

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
            size_t       i  = 0;
            for(const Constraint::Actor *a=cc.actors.head;a;a=a->next,++i)
            {
                os << *a;
            }
            return os;
        }

    }

}
