
#include "y/jive/language/grammar.hpp"
#include "y/ios/align.hpp"
#include "y/code/textual.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {

            namespace
            {

#define Y_AXIOM_FILL_WILDCARD(CLASS) \
case CLASS::UUID: fillDB(db, &(axiom->as<CLASS>().axiom) ); break

#define Y_AXIOM_FILL_COMPOUND(CLASS) \
/**/    case CLASS::UUID:\
/**/        for(const Axiom::Reference *ref=axiom->as<CLASS>().head;ref;ref=ref->next) fillDB(db, & **ref);\
/**/        break


                static inline
                void fillDB(Axiom::Registry &db, const Axiom *axiom)
                {
                    assert(axiom);
                    if(db.insert(*(axiom->name),(Axiom *)axiom))
                    {
                        // was not recorded
                        switch(axiom->uuid)
                        {
                                Y_AXIOM_FILL_WILDCARD(Repeat);
                                Y_AXIOM_FILL_WILDCARD(Option);
                                Y_AXIOM_FILL_COMPOUND(Aggregate);
                                Y_AXIOM_FILL_COMPOUND(Alternate);
                            default:
                                break;
                        }
                    }

                }

            }
            
            
            void Grammar:: validateWith(const Lexer *lexer) const
            {
                const char  *id   = **name;
                if(Axiom::Verbose) std::cerr << "<" << id << ">" << std::endl;
                const Axiom *root = getRoot();
                if(!root)
                {
                    throw exception("%s has not root Axiom",id);
                }

                Axiom::Registry db;
                fillDB(db,root);
                size_t linked = 0;
                size_t orphan = 0;
                size_t terms  = 0;
                string orphans;
                for(const Axiom *axiom = axioms.head; axiom; axiom=axiom->next)
                {
                    const string &aname = *(axiom->name);
                    if(Axiom::Verbose)
                    {
                        std::cerr << ios::align(aname, ios::align::left, aligned) << " : ";
                    }

                    switch(axiom->uuid)
                    {
                        case Terminal::UUID:
                            ++terms;
                            if(lexer)
                            {
                                if(! lexer->queryRule(aname))
                                {
                                    throw exception("%s is missing lexical <%s>",**name,*aname);
                                }
                            }
                            break;
                            
                        case Alternate::UUID:
                            if(axiom->as<Alternate>().size <= 0)
                            {
                                throw exception("%s has empty alternate <%s>",**name,*aname);
                            }
                            break;
                            
                        case Aggregate::UUID: {
                            const Aggregate &A = axiom->as<Aggregate>();
                            if(A.size <= 0)
                            {
                                throw exception("%s has empty aggregate <%s>",**name,*aname);
                            }
                            
                        } break;
                            
                        default:
                            break;
                    }

                    if(db.search(aname))
                    {
                        if(Axiom::Verbose) std::cerr << "linked";
                        ++linked;
                    }
                    else
                    {
                        if(Axiom::Verbose) std::cerr << "orphan";
                        ++orphan;
                        orphans << ' ' << aname;
                    }

                    TermLedger       ft;
                    Axiom::Expecting(ft,*axiom);
                    if(Axiom::Verbose)
                    {
                        std::cerr << " expect: " << ft.size();
                    }

                    if(Axiom::Verbose) std::cerr << std::endl;
                }

                if(orphan>0)
                {
                    throw exception("%s grammar has orphan%s:%s", **name, textual::plural_s(orphan), *orphans);
                }
                
                if(terms<=0)
                {
                    throw exception("%s grammar has no terminal!", **name);
                }



                if(Axiom::Verbose) std::cerr << "<" << id << "/>" << std::endl;

            }

        }

    }
}


