
#include "y/alchemy/library.hpp"
#include "y/exception.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace alchemy
    {
        
        library:: ~library() throw()
        {
        }
        
        library:: library() :
        compilable(),
        db()
        {
        }
        
        const species::db & library:: bulk() const throw()
        {
            return db;
        }
        
        
        
        
        const char library::clid[]       = "alchemy::library";
        
        const species & library:: use(species *s)
        {
            assert(NULL!=s);
            const species::pointer sp(s);
            if(!db.insert(sp))
            {
                throw exception("%s: multiple '%s'",clid,*(s->name) );
            }
            aliasing::_(compiled) = false;
            return *s;
        }
        
       
        
        bool library:: owns(const species &s) const throw()
        {
            const species::pointer *pps = db.search(s.key());
            if(!pps)
            {
                return false;
            }
            else
            {
                return &s == &(**pps);
            }
        }
        
        void library:: on_compile()
        {
            const size_t       n  = db.size();
            size_t             w  = 0;
            
            {
                db_type::iterator  it = db.begin();
                for(size_t i=1;i<=n;++i,++it)
                {
                    const species &sp = **it;
                    aliasing::_(sp.indx) = i;
                    w = max_of(w,sp.name.size());
                }
            }
            
            
            {
                db_type::iterator  it = db.begin();
                for(size_t i=1;i<=n;++i,++it)
                {
                    aliasing::_((**it).width) = w;
                }
            }
            
        }
        
    }
}


