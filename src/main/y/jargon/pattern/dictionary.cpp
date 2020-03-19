

#include "y/jargon/pattern/dictionary.hpp"

namespace upsylon {
    
    namespace Jargon {
     
        
        Dictionary:: Dictionary() : DictionaryType()
        {
        }
        
        Dictionary:: ~Dictionary() throw()
        {
        }
        
        Dictionary:: Dictionary(const Dictionary &d) : DictionaryType(d)
        {
        }
        
        bool Dictionary:: insert(const string &name, const Pattern *p)
        {
            assert(p!=NULL);
            const Motif m = p;
            return insert_by(name,m);
        }
        
        
        bool Dictionary:: insert(const char *name, const Pattern *p)
        {
            assert(p!=NULL);
            const Motif m = p;
            return insert_by(name,m);
        }
        
        const Motif * Dictionary:: search(const string &name) const throw()
        {
            return search_by(name);
        }
        
        const Motif * Dictionary:: search(const char *name) const throw()
        {
            return search_by(name);
        }
        
       
        
        const Motif & Dictionary:: operator[](const char *name) const
        {
            const Dictionary &self = *this;
            const string      _    =  name;
            return self[_];

        }
    }
    
}

#include "y/exception.hpp"
namespace upsylon {
    
    namespace Jargon {
        
        const Motif & Dictionary:: operator[](const string &name) const
        {
            const Motif *motif = search(name);
            if( !motif ) throw exception("no Jargon::Dictionary['%s']", *name);
            return *motif;
        }
        
        
      
        
    }
    
}
