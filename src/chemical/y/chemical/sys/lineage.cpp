
#include "y/chemical/sys/lineage.hpp"

namespace upsylon
{
    namespace Chemical
    {
        

#define Y_LINKAGE(TYPE) case TYPE : return #TYPE

        const char * LinkageText(const Linkage l) throw()
        {
            switch(l)
            {
                    Y_LINKAGE(Single);
                    Y_LINKAGE(Intake);
                    Y_LINKAGE(Inside);
                    Y_LINKAGE(Output);
                    Y_LINKAGE(Source);
                    Y_LINKAGE(Siphon);
            }
            return unknown_text;
        }



        Lineage:: ~Lineage() throw()
        {

        }

        Lineage:: Lineage(const Species &sp) throw() :
        gvObject(),
        authority<const Species>(sp),
        Flow(Bounded),
        linkage(Single),
        consumers(),
        producers()
        {
            
        }

        void  Lineage:: link(const unit_t nu, const Primary &p)
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
        

        void  Lineage:: finalize() throw()
        {
            Linkage & _ = aliasing::_(linkage);
            switch(state)
            {
                case Bounded:
                    if(consumers.size>0)
                    {
                        if(producers.size>0)
                            _ = Inside;
                        else
                            _ = Intake;
                    }
                    else
                    {
                        assert(0==consumers.size);
                        if(producers.size>0)
                            _ = Output;
                        else
                            _ = Single;

                    }
                    break;

                case Endless:
                    if(consumers.size>0)
                    {
                        _ = Siphon;
                    }
                    else
                    {
                        assert(producers.size>0);
                        _ = Source;
                    }
                    break;
            };
        }

        const char * Lineage:: linkageText() const throw()
        {
            return LinkageText(linkage);
        }

        const char * Lineage:: label() const throw()
        {
            return * ( (**this).name );
        }
    }

}

