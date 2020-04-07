
#include "y/jargon/axiom.hpp"
#include "y/string/display.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Axiom::DB :: DB() : DB_Type()
        {
        }
        
        Axiom:: DB:: ~DB() throw()
        {
        }
        
        bool Axiom::DB:: insert(const Axiom &axiom)
        {
            const Axiom::Address addr = axiom;
            return insert_by(addr,addr);
        }
        
        bool Axiom:: DB:: search(const Axiom &axiom) const throw()
        {
            const Axiom::Address addr = axiom;
            return search_by(addr) != NULL;
        }
        
        void Axiom:: DB:: remove(const Axiom &axiom) throw()
        {
            const Axiom::Address addr = axiom;
            (void)remove_by(addr);
        }
        

        void Axiom:: DB:: secure(const Axiom &axiom)
        {
            (void)insert(axiom);
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
                const Axiom::DB      &parents = axiom->parents;
                string_display::align(std::cerr << pfx , *(axiom->label), width, "\t<", ">") << " <== ";
                const size_t   np = parents.entries();
                const_iterator jt = parents.begin();
                for(size_t i=1;i<=np;++i,++jt)
                {
                    std::cerr << jt->as<Axiom>()->label;
                    if(i<np) std::cerr << ", ";
                }
                std::cerr << std::endl;
            }
            std::cerr << pfx << "<Axiom::DB/>" << std::endl;
            
        }
        
        static inline int compareAddr( const Axiom::Address &lhs, const Axiom::Address &rhs ) throw()
        {
            const string &L = *(lhs.as<Axiom>()->label);
            const string &R = *(rhs.as<Axiom>()->label);
            return string::compare(L,R);
        }
        
        void Axiom::DB:: sort()
        {
            sort_with(compareAddr);
        }
        
        
        
    }
    
}
