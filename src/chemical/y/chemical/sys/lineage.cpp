
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

            if(nu<0)
            {
                aliasing::_(consumers).push_back( new Consumer(nu,p) );
            }
            else
            {
                aliasing::_(producers).push_back( new Producer(nu,p) );
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


        void Lineage:: vizCore(ios::ostream &fp) const
        {

            fp << '[';
            outputLabel(fp,(**this).name);
            switch(linkage)
            {
                case Single:
                    appendShape(fp,"oval");
                    appendStyle(fp,"bold,dotted,filled");
                    break;

                case Inside:
                    appendShape(fp,"oval");
                    appendStyle(fp,"bold,filled");
                    break;

                case Intake:
                    appendShape(fp,"invhouse");
                    appendStyle(fp,"bold,filled");
                    break;

                case Output:
                    appendShape(fp,"house");
                    appendStyle(fp,"bold,filled");
                    break;

                case Source:
                    appendShape(fp,"trapezium");
                    appendStyle(fp,"bold,filled,dashed");
                    break;

                case Siphon:
                    appendShape(fp,"invtrapezium");
                    appendStyle(fp,"bold,filled,dashed");
                    break;
            }

            fp << ']';
            endl(fp);
        }
    }

}

