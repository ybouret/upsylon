
#include "y/jive/syntax/grammar.hpp"
#include "y/exception.hpp"
#include "y/ios/align.hpp"
#include "y/code/textual.hpp"

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
                const size_t nr = db.size();
                if(nr<na)
                {
                    const size_t bad = na-nr;
                    throw exception("%s has %u orphan axiom%s",**name,unsigned(bad),textual::plural_s(bad));
                }
                else
                {
                    if(nr>na)
                    {
                        const size_t bad = nr-na;
                        throw exception("%s had %u foreign axiom%s",**name,unsigned(bad),textual::plural_s(bad));
                    }
                    else
                    {
                        // check peers
                        for(const Axiom *axiom=axioms.head;axiom;axiom=axiom->next)
                        {
                            Axiom * const *ppA = db.search(*(axiom->name));
                            if(!ppA)        throw exception("%s unregistered <%s>!!!",**name, **(axiom->name) );
                            if(*ppA!=axiom) throw exception("%s mismatch  <%s>!!!",**name, **(axiom->name) );
                        }
                    }
                }
                Y_JIVE_GRAMLN(db);

                //--------------------------------------------------------------
                // checking
                //--------------------------------------------------------------
                for(Axiom *axiom=axioms.head;axiom;axiom=axiom->next)
                {
                    Y_JIVE_GRAMLN("\t"<< ios::align(*(axiom->name),ios::align::left,maxNameLength) << " <== " << axiom->from );
                    switch(axiom->uuid)
                    {
                        case Aggregate::UUID: {
                            if(axiom->as<Aggregate>().size<=0)
                            {
                                throw exception("%s: aggregate <%s> is empty!",**name,**(axiom->name));
                            }
                        } break;

                        case Alternate::UUID: {
                            if(axiom->as<Alternate>().size<=0)
                            {
                                throw exception("%s: alternate <%s> is empty!",**name,**(axiom->name));
                            }
                        } break;

                        default:
                            break;
                    }

                }

                Y_JIVE_GRAMLN("<validating/>");

            }

        }

    }
}

