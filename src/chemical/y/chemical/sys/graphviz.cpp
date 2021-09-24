
#include "y/chemical/system.hpp"
#include "y/ios/tools/graphviz.hpp"
#include "y/ios/ocstream.hpp"



namespace upsylon
{

    namespace Chemical
    {

        template <typename ARR>
        static inline void saveNodes(ios::ostream &fp,
                                     const ARR    &arr)
        {
            for(size_t i=arr.size();i>0;--i)
            {
                arr[i]->vizSave(fp);
            }
        }


        static inline
        void saveNu(ios::ostream &fp, const unit_t nu)
        {
            fp("[label=\"%d\"\n]", int(nu));
        }

        template <typename APPLIANCES>
        static inline
        void saveLinks(ios::ostream     &fp,
                       const Lineage    &l,
                       const APPLIANCES &apps)
        {
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


        void System:: graphViz(const string &fileName) const
        {

            {
                ios::ocstream fp(fileName);
                Vizible::enterDigraph(fp,"G");
                saveNodes(fp,primary);
                saveNodes(fp,lineage);

                for(size_t j=M;j>0;--j)
                {
                    const Lineage      &l  = *lineage[j];
                    saveLinks(fp,l,l.consumers);
                    saveLinks(fp,l,l.producers);
                }
                

                Vizible::leaveDigraph(fp);
            }
            ios::GraphViz::Render(fileName);
            
        }

    }

}
