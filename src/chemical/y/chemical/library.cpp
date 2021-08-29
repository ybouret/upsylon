
#include "y/chemical/library.hpp"
#include "y/exception.hpp"
#include "y/type/utils.hpp"
#include "y/jive/regexp.hpp"
#include <cstdlib>

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
        jZ( Jive::RegExp("\\-+|\\++",NULL) ),
        jS( Jive::RegExp("[-+]?[:digit:]*",NULL) ),
        jB( Jive::RegExp("[:blank:]*",NULL) )
        {
            //jN->graphViz("jN.dot");
            //jZ->graphViz("jZ.dot");
            //jS->graphViz("jS.dot");
            //jB->graphViz("jB.dot");
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
        
        void Library:: noBlank(Jive::Source &source) const
        {
            Jive::Token token;
            (void) jB->accept(token,source);
        }
        
        string Library:: getName(Jive::Source &source) const
        {
            Jive::Token token;
            if(!jN->accept(token,source)) throw exception("%s no species name",CLID);
            return token.toString();
        }
        
        const Species &Library:: use(Jive::Source &source)
        {
            
            
            // extract name
            noBlank(source);
            string name   = getName(source);
            noBlank(source);
            
            unit_t charge = 0;
            {
                // extract charge
                Jive::Token token;
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
            }
            noBlank(source);
            
            // check done
            if(source.is_active()) throw exception("%s invalid char '%c' in species name",CLID, source.peek()->code);
            
            
            // create species
            return(*this)(name,charge);
        }
        
        static inline unit_t token2nu(Jive::Token &token) throw()
        {
            unit_t nu=0;
            switch(token.size)
            {
                    // empty
                case 0: nu=1; break;
                    
                    // one char
                case 1:
                {
                    const uint8_t c = token.head->code;
                    switch(c)
                    {
                        case '-': nu=-1; break;
                        case '+': nu=1;  break;
                        default: assert(c>='0'); assert(c<='9');
                            nu = c-uint8_t('0');
                            break;
                    }
                } break;
                    
                    // default
                default: {
                    const string s = token.toString();
                    nu = atol(*s);
                } break;
            }
            
            return nu;
        }
        
        unit_t Library:: get(Jive::Source &source, const Species **pps)
        {
            assert(NULL!=pps);
            assert(NULL==*pps);
            noBlank(source);
            Jive::Token token;
            const unit_t nu = jS->accept(token,source) ? token2nu(token) : 1;
            *pps            = & use(source);
            return nu;
        }
        
        
    }
}
