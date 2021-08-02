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
        
        
        void Actors:: fill(addressable<long> &Nu, long s) const throw()
        {
            if(s>=0)
            {
                // product
                for(const_iterator it=adb.begin();it!=adb.end();++it)
                {
                    const Actor &a = *it;
                    Nu[a->indx] = static_cast<long>( a.nu );
                }
            }
            else
            {
                // reactant
                for(const_iterator it=adb.begin();it!=adb.end();++it)
                {
                    const Actor &a = *it;
                    Nu[a->indx] = -static_cast<long>( a.nu );
                }
            }
        }

        
        
        Extent Actors:: findExtent(const accessible<double> &C) const throw()
        {
            //std::cerr << "Find Extent with C=" << C << std::endl;
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
            //std::cerr << "|_xm=" << xm << " @" << im << std::endl;
            return Extent(im,xm);
        }

        double Actors:: massAction(const double multiplier, const accessible<double> &C) const throw()
        {
            double result = multiplier;
            for(const Actor::Node *node = adb.head();node;node=node->next)
            {
                result *= (**node).activity(C);
            }
            return result;
        }

        double Actors:: massAction(const double              multiplier,
                                   const accessible<double> &C,
                                   const double              xi) const throw()
        {
            double result = multiplier;
            for(const Actor::Node *node = adb.head();node;node=node->next)
            {
                result *= (**node).activity(C,xi);
            }
            return result;
        }


    }
    
}

