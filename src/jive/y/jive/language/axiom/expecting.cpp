
#include "y/jive/language/axiom/all.hpp"


namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {
            void Axiom:: Expecting(TermLedger &first, const Axiom &axiom, Registry &db, unsigned depth)
            {
                const string &key = *axiom.name;
                if( db.insert(key,(Axiom *)&axiom) )
                {
                    Y_LANG_PRINTLN( '|' << ios::indent(depth,'_') << "expecting for " << key);
                    ++depth;
                    // first time db
                    switch(axiom.uuid)
                    {
                        case Terminal::UUID: {
                            Terminal              * the_term = & aliasing::_(axiom).as<Terminal>();
                            const Terminal *const * inserted = first.search(key);
                            if(!inserted)
                            {
                                if(!first.insert(key,the_term))
                                    throw exception("unexpected insertion failure for <%s>",*key);
                            }
                            else
                            {
                                if(the_term != *inserted)
                                    throw exception("corrupted grammar with Terminal <%s>",*key);
                            }
                        } break;

                        case Alternate::UUID: {
                            for(const Axiom::Reference *ref = axiom.as<Alternate>().head;ref;ref=ref->next)
                            {
                                Expecting(first,**ref,db,depth);
                            }
                        } break;

                        case Option::UUID: Expecting(first,axiom.as<Option>().axiom,db,depth); break;
                        case Repeat::UUID: Expecting(first,axiom.as<Repeat>().axiom,db,depth); break;

                        case Aggregate::UUID: {
                            
                        } break;

                        default:
                            throw exception("expecting unhandled [%s]", fourcc_(axiom.uuid) );
                            break;
                    }


                }
                else
                {
                    // already visited
                }
            }
        }

    }

}
