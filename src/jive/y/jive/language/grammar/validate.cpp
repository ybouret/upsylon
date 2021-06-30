
#include "y/jive/language/grammar.hpp"
#include "y/ios/align.hpp"
#include "y/code/textual.hpp"
#include "y/sequence/slots.hpp"

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


            void   Grammar:: cleanAxioms() const throw()
            {
                Y_LANG_PRINTLN("\t<" << name << " cleaning>");
                for(const Axiom *axiom = axioms.tail; axiom; axiom=axiom->prev)
                {
                    if(axiom->priv)
                    {
                        Y_LANG_PRINTLN("\t\t\\_[" << fourcc_(axiom->uuid) << "] " << axiom->name);
                        delete static_cast<TermLedger *>(axiom->priv);
                        axiom->priv = 0;
                    }
                }
                Y_LANG_PRINTLN("\t<" << name << " cleaning/>");

            }


            
            void Grammar:: validateWith(const Lexer *lexer) const
            {
                try
                {
                    //----------------------------------------------------------
                    //
                    // initialize
                    //
                    //----------------------------------------------------------
                    const char  *id   = **name;
                    if(Axiom::Verbose) std::cerr << "<" << id << ">" << std::endl;
                    const Axiom *root = getRoot();
                    if(!root)
                    {
                        throw exception("%s has not root Axiom",id);
                    }

                    //----------------------------------------------------------
                    //
                    // visit all axioms
                    //
                    //----------------------------------------------------------
                    Axiom::Registry db;
                    fillDB(db,root);

                    //----------------------------------------------------------
                    //
                    // study result
                    //
                    //----------------------------------------------------------
                    size_t linked = 0;
                    size_t orphan = 0;
                    size_t terms  = 0;
                    string orphans;

                    for(const Axiom *axiom = axioms.head; axiom; axiom=axiom->next)
                    {
                        const string  &aname = *(axiom->name);
                        const uint32_t auuid = axiom->uuid;
                        if(Axiom::Verbose)
                        {
                            std::cerr << "[" << fourcc_(auuid) << "] " << ios::align(aname, ios::align::left, aligned) << " : ";
                        }

                        switch(auuid)
                        {
                            case Terminal::UUID:
                                ++terms;
                                if(lexer)
                                {
                                    if(!lexer->queryRule(aname))
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
                            if(Axiom::Verbose) std::cerr << "[linked]";
                            ++linked;
                        }
                        else
                        {
                            if(Axiom::Verbose) std::cerr << "[orphan]";
                            ++orphan;
                            orphans << ' ' << aname;
                        }

                        // and local first-terms
                        switch(auuid)
                        {
                            case Aggregate::UUID:
                            case Alternate::UUID: {
                                TermLedger *ft = new TermLedger();
                                axiom->priv    = ft;
                                Axiom::Expecting(*ft,*axiom);
                                if(Axiom::Verbose)
                                {
                                    std::cerr << " ==> {";
                                    list<string>                     names;
                                    ft->collect<string,list<string> >(names);
                                    for(size_t i=1;i<=names.size();++i) std::cerr << " <" << names[i] << ">";
                                    std::cerr << " }";
                                }
                            } break;

                            default:
                                break;
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

                    //----------------------------------------------------------
                    //
                    // build hosts
                    //
                    //----------------------------------------------------------
                    Y_LANG_PRINTLN("\t<" << name << " hosts>");
                    for(const Axiom *axiom = axioms.head; axiom; axiom=axiom->next)
                    {
                        if(Terminal::UUID==axiom->uuid)
                        {
                            const Terminal *guest = & axiom->as<Terminal>();
                            const string   &key   = *(guest->name);
                            std::cerr << "\t\t\\_for <" << key << ">" << std::endl;
                            for(const Axiom *node=axioms.head;node;node=node->next)
                            {
                                if(axiom->priv)
                                {

                                }
                            }


                        }
                    }
                    Y_LANG_PRINTLN("\t<" << name << " hosts/>");



                    //----------------------------------------------------------
                    //
                    // done
                    //
                    //----------------------------------------------------------
                    cleanAxioms();
                    if(Axiom::Verbose) std::cerr << "<" << id << "/>" << std::endl;
                }
                catch(...)
                {
                    cleanAxioms();
                    throw;
                }
            }

        }

    }
}


