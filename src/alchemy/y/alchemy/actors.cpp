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
        
        
    

        
        
        Extent Actors:: findExtent(const Accessible &C) const throw()
        {
            double             xm   = 0;
            size_t             im   = 0;
            const Actor::Node *node = adb.head();
            if(node)
            {
                //______________________________________________________________
                //
                // initialize
                //______________________________________________________________
                {
                    const Actor &a = **node;
                    im = a->indx;
                    xm = a.maxExtent(C[im]);
                    if(xm<=0) goto FOUND;
                }

                //______________________________________________________________
                //
                // loop on remaining actors
                //______________________________________________________________
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

        double Actors:: massAction(double target, const Accessible &C) const throw()
        {
            for(const Actor::Node *node = adb.head();node;node=node->next)
            {
                target *= (**node).activity(C);
            }
            return target;
        }

        double Actors:: massAction(double            target,
                                   const Accessible &C,
                                   const double      xi) const throw()
        {
            for(const Actor::Node *node = adb.head();node;node=node->next)
            {
                target *= (**node).activity(C,xi);
            }
            return target;
        }


        double Actors:: jacobian(double            target,
                                 Addressable      &phi,
                                 const double      jscale,
                                 const Accessible &C) const throw()
        {

            for(const Actor::Node *node = adb.head();node;node=node->next)
            {
                const Actor &a = (**node);
                target        *= a.activity(C);

                double jac     = jscale * a.jacobian(C);
                for(const Actor::Node *sub=node->prev;sub;sub=sub->prev) jac *= (**sub).activity(C);
                for(const Actor::Node *sub=node->next;sub;sub=sub->next) jac *= (**sub).activity(C);
                phi[a->indx]   = jac;
                
            }
            return target;
        }

        void Actors:: guess(addressable<bool> &active) const throw()
        {
            for(const Actor::Node *node = adb.head();node;node=node->next)
            {
                assert( (**node)->active );
                active[ (**node)->indx ] = true;
            }
        }


    }
    
}

