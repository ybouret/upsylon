
#include "y/jargon/axiom.hpp"
#include "y/string/display.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Axiom::DB :: DB() : DB_Type()
        {
        }
        
        Axiom:: DB:: ~DB() throw()
        {
        }
        
        
        void Axiom::DB:: display(const char *pfx) const
        {
            if(!pfx) pfx="";
            size_t width = 0;
            for( const_iterator it=begin(); it!=end();++it)
            {
                const size_t tmp = it->as<Axiom>()->label->size();
                if(tmp>width)
                {
                    width = tmp;
                }
            }
            
            std::cerr << pfx << "<Axiom::DB entries=" << entries() << ">" << std::endl;
            for( const_iterator it=begin(); it!=end();++it)
            {
                const Axiom          *axiom   = it->as<Axiom>();
                string_display::align(std::cerr << pfx , *(axiom->label), width, "\t<", ">") << " <== ";
                {
                    const Axiom::DB &parents = (axiom->parents);
                    const size_t     np      = parents.entries();
                    const_iterator   jt      = parents.begin();
                    for(size_t i=1;i<=np;++i,++jt)
                    {
                        std::cerr << jt->as<Axiom>()->label;
                        if(i<np) std::cerr << ", ";
                    }
                }
                std::cerr << std::endl;
            }
            std::cerr << pfx << "<Axiom::DB/>" << std::endl;
            
        }
        
        static inline int compareAddr( const BEaddress &lhs, const BEaddress &rhs ) throw()
        {
            const string &L = *(lhs.as<Axiom>()->label);
            const string &R = *(rhs.as<Axiom>()->label);
            return string::compare(L,R);
        }
        
        void Axiom::DB:: sort()
        {
            sort_with(compareAddr);
        }
        
        void  Axiom::DB:: cat(exception &excp) const throw()
        {
            const size_t n = entries();
            if(n<=0)
            {
                excp.cat("unknown compound");
                
            }
            else
            {
                const size_t nm1 = n-1;
                size_t       i   = 1;
                for( const_iterator it=begin(); it != end(); ++it, ++i)
                {
                    excp.cat("%s", **((*it).as<Axiom>()->label) );
                    if(i<nm1)
                    {
                        excp.cat(", ");
                    }
                    else
                    {
                        if(i==nm1)
                        {
                            excp.cat(" or ");
                        }
                    }
                }
            }
        }
        
        
        
    }
    
}
