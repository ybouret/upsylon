

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

        
    }
    
}
