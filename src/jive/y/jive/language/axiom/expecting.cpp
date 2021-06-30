
#include "y/jive/language/axiom/all.hpp"


namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {
            void Axiom:: Expecting (TermLedger  &terms,
                                    const Axiom &axiom,
                                    unsigned     depth,
                                    const bool   verbose)
            {
                Registry visited;
                ExpectingCore(terms,axiom,visited,depth,verbose);
            }


            void Axiom:: ExpectingCore(TermLedger  &terms,
                                       const Axiom &axiom,
                                       Registry    &visited,
                                       unsigned     depth,
                                       const bool   verbose)
            {
                const string &key = *axiom.name;
                if( visited.insert(key,(Axiom *)&axiom) )
                {
                    //__________________________________________________________
                    //
                    // first time visited: check
                    //__________________________________________________________
                    switch(axiom.uuid)
                    {
                            //__________________________________________________
                            //
                            // a terminal expects itself
                            //__________________________________________________
                        case Terminal::UUID: {
                            Terminal              * the_term = & aliasing::_(axiom).as<Terminal>();
                            const Terminal *const * inserted = terms.search(key);
                            if(!inserted)
                            {
                                if(!terms.insert(key,the_term)) throw exception("unexpected insertion failure for <%s>",*key);
                                if(verbose) std::cerr << '|' << ios::indent(depth,'_') << "expecting <" << key << ">" << std::endl;
                            }
                            else
                            {
                                if(the_term != *inserted)
                                    throw exception("corrupted grammar with Terminal <%s>",*key);
                            }
                        } break;

                            //__________________________________________________
                            //
                            // an alternate expects its manifest
                            //__________________________________________________
                        case Alternate::UUID: {
                            if(verbose) std::cerr << '|' << ios::indent(depth,'_') << "expecting alternate <" << key << ">" << std::endl;
                            ++depth;
                            for(const Axiom::Reference *ref = axiom.as<Alternate>().head;ref;ref=ref->next)
                            {
                                ExpectingCore(terms,**ref,visited,depth,verbose);
                            }
                        } break;

                            //__________________________________________________
                            //
                            // transitive Option
                            //__________________________________________________
                        case Option::UUID: ExpectingCore(terms,axiom.as<Option>().axiom,visited,++depth,verbose); break;

                            //__________________________________________________
                            //
                            // transivite Repeat
                            //__________________________________________________
                        case Repeat::UUID: ExpectingCore(terms,axiom.as<Repeat>().axiom,visited,++depth,verbose); break;

                            //__________________________________________________
                            //
                            // an Aggregate expects the first non-empty member
                            //__________________________________________________
                        case Aggregate::UUID: {
                            if(verbose) std::cerr << '|' << ios::indent(depth,'_') << "expecting aggregate <" << key << ">" << std::endl;
                            ++depth;
                            TermLedger sub;
                            for(const Axiom::Reference *ref = axiom.as<Aggregate>().head;ref;ref=ref->next)
                            {
                                Expecting(sub,**ref,depth);
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

