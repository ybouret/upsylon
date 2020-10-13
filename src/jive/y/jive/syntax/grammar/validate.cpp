
#include "y/jive/syntax/grammar.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
#define Y_JIVE_GRAMLN(MSG) do { if(Axiom::Verbose) { std::cerr << "[[" << name << "]] " << MSG << std::endl; } } while(false)

            void  Grammar:: validate() const
            {
                Y_JIVE_GRAMLN("<validating>");

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
                axioms.head->attach(db);

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
                            Axiom * const *ppA = db.search_by(*(axiom->name));
                            if(!ppA)        throw exception("%s unregistered <%s>!!!",**name, **(axiom->name) );
                            if(*ppA!=axiom) throw exception("%s mismatch  <%s>!!!",**name, **(axiom->name) );
                        }
                    }
                }
                Y_JIVE_GRAMLN(db);

                //--------------------------------------------------------------
                // create 'then' lists
                //--------------------------------------------------------------
                for(Axiom *axiom=axioms.head;axiom;axiom=axiom->next)
                {
                    Y_JIVE_GRAMLN("\t<"  << axiom->name << "> <== " << axiom->from );

                }

                Y_JIVE_GRAMLN("<validating/>");

            }

        }

    }
}

