
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
        jN( Jive::RegExp("[:upper:][:word:]*",NULL) ),
        jZ( Jive::RegExp("\\-+|\\++",NULL) )
        {
            
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
            
            size_t i=0;
            for(const Species::Node *node=sdb.head();node;node=node->next)
            {
                const Species &sp = ***node;
                aliasing::_(sp.nmax) = nmax;
                aliasing::_(sp.indx) = ++i;
            }
        }
        
        const Species &Library:: use(Jive::Module *module)
        {
            assert(module!=NULL);
            Jive::Source source(module);
            
            Jive::Token token;
            if(!jN->accept(token,source)) throw exception("%s no species name",CLID);
            string       name   = token.toString();
            unit_t       charge = 0;
            token.release();
            if(jZ->accept(token,source))
            {
                assert(token.size>0);
                switch(token.head->code)
                {
                    case '+': charge =  unit_t(token.size); break;
                    case '-': charge = -unit_t(token.size); break;
                }
            }
            name += token.toString();
            token.release();
            if(source.is_active()) throw exception("%s invalid char '%c' in species name",CLID, source.peek()->code);
            
            
            
            return(*this)(name,charge);
        }
        
        
    }
}
