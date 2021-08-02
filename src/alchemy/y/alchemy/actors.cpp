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
            double             xm   = 0;
            size_t             im   = 0;
            const Actor::Node *node = adb.head();
            if(node)
            {
                // initialize
                {
                    const Actor &a = **node;
                    im = a->indx;
                    xm = a.maxExtent(C[im]);
                    if(xm<=0) goto FOUND;
                }

                // loop
                for(node=node->next;node;node=node->next)
                {
                    const Actor &a    = **node;
                    const size_t itmp = a->indx;
                    const double xtmp = a.maxExtent(C[itmp]);
                    if(xtmp<xm)
                    {
                        xm = xtmp;
                        im = itmp;
                        if(xm<=0) goto FOUND;
                    }
                }

            }
            
        FOUND:
            return Extent(im,xm);
        }

    }
    
}

