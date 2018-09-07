#include "y/chem/boot.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Chemical
    {
        Boot:: Actor:: Actor( Species &s, const int w ) throw() :
        species(&s),
        weight(w)
        {
        }

        Boot:: Actor:: ~Actor() throw()
        {
        }

        const string & Boot:: Actor:: key() const throw()
        {
            return species->name;
        }

        std::ostream & operator<<( std::ostream &os, const Boot::Actor &a)
        {
            const int w = a.weight;
            if(w==0)
            {
                return os;
            }

            if(w>0)
            {
                os << '+';
                if(w>1) os << w;
            }

            if( w<0 )
            {
                os << '-';
                if(w<-1) os << (-w);
            }
            
            os << '[' << a.species->name << ']';
            return os;
        }
    }
}

namespace upsylon
{
    namespace Chemical
    {
        Boot:: Constraint:: Constraint(const double v) throw() :
        value(v),
        actors()
        {
        }

        Boot:: Constraint:: ~Constraint() throw()
        {
        }

        Boot::Constraint &  Boot:: Constraint:: add( Species &species, const int weight )
        {
            Actor::Pointer *ppA = actors.search(species.name);
            if(!ppA)
            {
                Actor::Pointer pA = new Actor(species,weight);
                if(!actors.insert(pA)) throw exception("unexpected actor '%s*%d' insertion failure",*(species.name),weight);
            }
            else
            {
                Actor &a = **ppA;
                int   &w = (int &)(a.weight);
                w += weight;
                if(0==w)
                {
                    if( !actors.remove( species.name ) )
                    {
                        throw exception("unexpected failure to remove actor '%s'", *species.name );
                    }
                }
            }
            return *this;
        }

        std::ostream & operator<<( std::ostream &os, const Boot::Constraint &c)
        {
            os << c.value << '=';
            for( Boot::Actors::const_iterator i=c.actors.begin(); i!=c.actors.end();++i)
            {
                os << **i;
            }
            return os;
        }
    }

}

namespace upsylon
{
    namespace Chemical
    {

        Boot:: ~Boot() throw()
        {
        }

        Boot:: Boot() throw() : constraints()
        {
        }

        Boot::Constraint & Boot:: create(const double value)
        {
            Constraint::Pointer p = new Constraint(value);
            constraints.push_back(p);
            return *p;
        }


        std::ostream & operator<<( std::ostream &os, const Boot &b)
        {
            os << "<Chemical::Boot constraints='" << b.constraints.size() << "'>" << std::endl;
            for(Boot::Constraints::const_iterator i=b.constraints.begin();i!=b.constraints.end();++i)
            {
                os << "|_< " << **i << " >" << std::endl;
            }
            os << "<Chemical::Boot/>";
            return os;
        }


        void Boot:: conserve( const double C0, Species &sp1 )
        {
            create(C0).add(sp1);
        }

        void Boot:: conserve( const double C0, Species &sp1, Species &sp2 )
        {
            create(C0).add(sp1).add(sp2);
        }

        void Boot:: conserve( const double C0, Species &sp1, Species &sp2, Species &sp3 )
        {
            create(C0).add(sp1).add(sp2).add(sp3);
        }

        void Boot:: electroneutrality(Library &lib)
        {
            Constraint &EN = create(0);
            for(Library::iterator i=lib.begin();i!=lib.end();++i)
            {
                Species &s = **i;
                EN.add(s,s.z);
            }
        }

        void Boot:: osmolarity(const double osm, Library &lib)
        {
            Constraint &OSM = create(osm);
            for(Library::iterator i=lib.begin();i!=lib.end();++i)
            {
                OSM.add(**i);
            }
        }


    }
}

