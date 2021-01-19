
#include "y/mkl/fitting/built-in/gaussians.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {
            
            namespace built_in
            {
                namespace bulk
                {
                    gaussians:: ~gaussians() throw()
                    {
                    }

                    size_t & make_geq_one(size_t &num)
                    {
                        if(num<=0) num=1;
                        return num;
                    }
                    gaussians:: gaussians(size_t num) :
                    qualified( 3* make_geq_one(num) ),
                    count(num)
                    {
                        assert(count>=1);
                        static const char * const label[3] = { "A", "k", "m" };

                        sequence<string> &id = aliasing::_(names);
                        if(num<=1)
                        {
                            id << label[0] << label[1] << label[2];
                        }
                        else
                        {
                            for(size_t i=1;i<=num;++i)
                            {
                                const string tag = vformat("%lu", (unsigned long)i );
                                for(size_t j=0;j<3;++j)
                                {
                                    const string l = label[j] + tag;
                                    id << l;
                                }
                            }
                        }

                    }

                }

            }

        }

    }

}

