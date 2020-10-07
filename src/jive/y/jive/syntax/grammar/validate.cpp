
#include "y/jive/syntax/grammar.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {

            void  Grammar:: validate() const
            {
                std::cerr << "validating " << name << std::endl;

                //--------------------------------------------------------------
                //
                // checking axiom
                //
                //--------------------------------------------------------------
                const size_t    na = axioms.size;
                if(na<=0)
                {
                    throw exception("%s has no axiom!!!", **name);
                }

                //--------------------------------------------------------------
                //
                // visit axioms
                //
                //--------------------------------------------------------------
                Axiom::Registry db;
                for(const Axiom *axiom=axioms.head;axiom;axiom=axiom->next)
                {
                    axiom->attach(db);
                }

                //--------------------------------------------------------------
                //
                // checking database
                //
                //--------------------------------------------------------------
                const size_t nr = db.entries();
                if(nr<na)
                {
                    const size_t bad = na-nr;
                    throw exception("%s has %u orphan axiom%s",**name,unsigned(bad),plural_s(bad));
                }
                else
                {
                    if(nr>na)
                    {
                        const size_t bad = nr-na;
                        throw exception("%s had %u foreign axiom%s",**name,unsigned(bad),plural_s(bad));
                    }
                    else
                    {
                        // check peers
                        for(const Axiom *axiom=axioms.head;axiom;axiom=axiom->next)
                        {
                            std::cerr << "<" << axiom->name << ">" << std::endl;
                            Axiom * const *ppA = db.search_by(*(axiom->name));
                            if(!ppA)        throw exception("%s unregistered <%s>!!!",**name, **(axiom->name) );
                            if(*ppA!=axiom) throw exception("%s mismatch  <%s>!!!",**name, **(axiom->name) );
                        }
                    }
                }

                //--------------------------------------------------------------
                
            }

        }

    }
}

