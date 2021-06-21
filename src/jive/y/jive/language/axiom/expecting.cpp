
#include "y/jive/language/axiom/all.hpp"


namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {
            void Axiom:: Expecting(TermLedger  &terms,
                                   const Axiom &axiom,
                                   Registry    &visited,
                                   unsigned     depth)
            {
                const string &key = *axiom.name;
                if( visited.insert(key,(Axiom *)&axiom) )
                {
                    //__________________________________________________________
                    //
                    // first time db
                    //__________________________________________________________
                    switch(axiom.uuid)
                    {
                        case Terminal::UUID: {
                            Terminal              * the_term = & aliasing::_(axiom).as<Terminal>();
                            const Terminal *const * inserted = terms.search(key);
                            if(!inserted)
                            {
                                if(!terms.insert(key,the_term))
                                    throw exception("unexpected insertion failure for <%s>",*key);
                                Y_LANG_PRINTLN( '|' << ios::indent(depth,'_') << "expecting <" << key << ">");
                            }
                            else
                            {
                                if(the_term != *inserted)
                                    throw exception("corrupted grammar with Terminal <%s>",*key);
                            }
                        } break;

                        case Alternate::UUID: {
                            Y_LANG_PRINTLN( '|' << ios::indent(depth,'_') << "expecting alternate <" << key << ">");
                            ++depth;
                            for(const Axiom::Reference *ref = axiom.as<Alternate>().head;ref;ref=ref->next)
                            {
                                Expecting(terms,**ref,visited,depth);
                            }
                        } break;

                        case Option::UUID: Expecting(terms,axiom.as<Option>().axiom,visited,depth); break;
                        case Repeat::UUID: Expecting(terms,axiom.as<Repeat>().axiom,visited,depth); break;

                        case Aggregate::UUID: {
                            Y_LANG_PRINTLN( '|' << ios::indent(depth,'_') << "expecting aggregate <" << key << ">");
                            ++depth;
                            TermLedger sub;
                            Registry   subDB;
                            for(const Axiom::Reference *ref = axiom.as<Aggregate>().head;ref;ref=ref->next)
                            {
                                Expecting(sub,**ref,subDB,depth);
                                if(sub.size()>0)
                                {
                                    terms.merge(sub);
                                    return;
                                }
                            }
                        } break;

                        default:
                            throw exception("expecting unhandled [%s]", fourcc_(axiom.uuid) );
                            break;
                    }


                }
                else
                {
                    //__________________________________________________________
                    //
                    // already visited
                    //__________________________________________________________
                }
            }
        }

    }

}

