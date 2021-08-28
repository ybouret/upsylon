
#include "y/chemical/library.hpp"
#include "y/exception.hpp"
#include "y/type/utils.hpp"
#include "y/jive/regexp.hpp"

namespace upsylon
{
    namespace Chemical
    {
        Library:: ~Library() throw()
        {
        }
        
        Library:: Library() :
        sdb(),
        name(   Jive::RegExp("[:upper:][:word:]*",NULL) ),
        charge( Jive::RegExp("\\-+|\\++",NULL) )
        {
            //name->graphViz("name.dot");
            //charge->graphViz("charge.dot");
        }
        
        Library::const_type & Library::bulk() const throw()
        {
            return sdb;
        }
        
        const char Library:: CLID[] = "Chemical::Library";
        
        const Species & Library:: use(Species *s)
        {
            const Species::Pointer sp(s);
            if(frozen) throw exception("%s is frozen",CLID);
            
            const Species::Pointer *pps = sdb.search(s->name);
            if(pps)
            {
                const Species::Pointer &my = *pps;
                if(my->charge!=sp->charge) throw exception("%s mismatch charge for '%s'", CLID, *(s->name));
                return *my;
            }
            else
            {
                if(!sdb.insert(sp)) throw exception("%s unexpected failure to use '%s'",CLID,*(s->name));
                update();
                return *sp;
            }
            
        }
        
        void Library:: update() throw()
        {
            size_t nmax = 0;
            for(const Species::Node *node=sdb.head();node;node=node->next)
            {
                nmax = max_of(nmax,(***node).name.size());
            }
            
            for(const Species::Node *node=sdb.head();node;node=node->next)
            {
                aliasing::_((***node).nmax) = nmax;
            }
        }


    }
}
