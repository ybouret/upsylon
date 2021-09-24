
#include "y/chemical/system.hpp"
#include "y/ios/tools/graphviz.hpp"
#include "y/ios/ocstream.hpp"



namespace upsylon
{

    namespace Chemical
    {

        template <typename ARR>
        static inline void saveArray(ios::ostream &fp,
                                     const ARR    &arr)
        {
            for(size_t i=arr.size();i>0;--i)
            {
                arr[i]->vizSave(fp);
            }
        }

        void System:: graphViz(const string &fileName) const
        {

            {
                ios::ocstream fp(fileName);
                Vizible::enterDigraph(fp,"G");
                saveArray(fp,primary);
                saveArray(fp,lineage);
                


                Vizible::leaveDigraph(fp);
            }
            ios::GraphViz::Render(fileName);
            
        }

    }

}
