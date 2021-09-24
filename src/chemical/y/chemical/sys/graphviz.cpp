
#include "y/chemical/system.hpp"
#include "y/ios/tools/graphviz.hpp"
#include "y/ios/ocstream.hpp"



namespace upsylon
{

    namespace Chemical
    {




        static inline
        void saveNu(ios::ostream &fp, const unit_t nu)
        {
            fp("[label=\"%d\"\n]", int(nu));
        }

        template <typename APPLIANCES>
        static inline
        void saveLinks(ios::ostream     &fp,
                       const Lineage    &l,
                       const APPLIANCES &apps,
                       const bool        boundedOnly)
        {
            if(boundedOnly&&Flow::Endless==l.state) return;
            
            for(const typename APPLIANCES::node_type *app=apps.head;app;app=app->next)
            {
                const unit_t   nu = app->nu;
                const Primary &p  = **app;
                if(nu<0)
                {
                    l.vizJoin(fp,&p);
                    if(nu< -1) saveNu(fp,-nu);
                }
                else
                {
                    p.vizJoin(fp,&l);
                    if(nu>1) saveNu(fp,nu);
                }
                l.endl(fp);
            }
        }


        void System:: graphViz(const string &fileName, const bool boundedOnly) const
        {

            {
                ios::ocstream fp(fileName);
                Vizible::enterDigraph(fp,"G");
                for(size_t i=1;i<=N;++i)
                {
                    const Primary &p = *primary[i];
                    if(boundedOnly)
                    {
                        if(Flow::Bounded==p->state)
                        {
                            p.vizSave(fp);
                        }
                    }
                    else
                    {
                        p.vizSave(fp);
                    }
                }

                for(size_t j=1;j<=M;++j)
                {
                    const Lineage &l = *lineage[j];
                    if(boundedOnly)
                    {
                        if(Flow::Bounded==l.state)
                        {
                            l.vizSave(fp);
                        }
                    }
                    else
                    {
                        l.vizSave(fp);
                    }
                }


                for(size_t j=M;j>0;--j)
                {
                    const Lineage      &l  = *lineage[j];
                    saveLinks(fp,l,l.consumers,boundedOnly);
                    saveLinks(fp,l,l.producers,boundedOnly);
                }
                

                Vizible::leaveDigraph(fp);
            }
            ios::GraphViz::Render(fileName);
            
        }

    }

}
