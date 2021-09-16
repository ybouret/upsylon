
#include "y/chemical/sys/strain.hpp"

namespace upsylon
{
    namespace Chemical
    {

        Appliance:: ~Appliance() throw()
        {
        }

        Appliance:: Appliance(const unit_t coef, const Primary &self) throw() :
        Object(),
        authority<const Primary>(self),
        dnode<Appliance>(),
        nu(coef)
        {
        }


        Strain:: ~Strain() throw()
        {

        }

        Strain:: Strain(const Species &sp) throw() :
        Object(),
        authority<const Species>(sp),
        Flow(Bounded),
        consumers(),
        producers()
        {
            
        }

        void  Strain:: link(const unit_t nu, const Primary &p)
        {
            assert(nu!=0);
            assert(nu==p->stoichiometry(**this));

            Appliance *app = new Appliance(nu,p);
            if(nu<0)
            {
                aliasing::_(consumers).push_back(app);
            }
            else
            {
                aliasing::_(producers).push_back(app);
            }

            if( p->state == Endless )
            {
                aliasing::_(state) = Endless;
            }
        }

        bool Strain:: isIntake() const throw()
        {
            return   (Bounded==state) && (1==(**this).rating) && (consumers.size>0);
        }
        
        bool Strain:: isOutput() const throw()
        {
            return   (Bounded==state) && (1==(**this).rating) && (producers.size>0);
        }

    }

}

