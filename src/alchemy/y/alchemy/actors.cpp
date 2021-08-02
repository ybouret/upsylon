#include "y/alchemy/actors.hpp"
#include "y/exception.hpp"


namespace upsylon
{
    namespace Alchemy
    {

        const char Actors:: CLID[] = "Actors";
        
        Actors:: Actors() :
        adb()
        {
        }
        
        Actors:: ~Actors() throw()
        {

        }
        
        
        void Actors:: operator()(const Species &sp, const unsigned long nu)
        {
            if(nu<=0)    throw exception("%s(nul coefficient for '%s')", CLID, *sp.name);
            const Actor a(sp,nu);
            if( !adb.insert(a) ) throw exception("%s(multiple '%s')", CLID, *sp.name);
        }
        

        
        Actors::const_type & Actors::bulk()  const throw()
        {
            return adb;
        }
        
        

        
        
        Extent Actors:: findExtent(const accessible<double> &C) const throw()
        {
            double        xm   = 0;
            size_t        im   = 0;
#if 0
            size_t        ii  = size;
            if(ii)
            {
                const size_t *idx = indx;
                const size_t *cof = coef;
                
                // initialize
                im = idx[ii];       assert(im>0); assert(im<=C.size());
                xm = C[im]/cof[ii];
                
                // scan
                for(--ii;ii>0;--ii)
                {
                    const size_t it = idx[ii]; assert(it>0); assert(it<=C.size());
                    const double xt = C[ it ]/cof[ii];
                    if(xt<xm)
                    {
                        xm = xt;
                        im = it;
                    }
                }
            }
#endif
            return Extent(im,xm);
        }

    }
    
}

