
#include "y/jive/syntax/axiom.hpp"
#include "y/type/aliasing.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {

            Axiom:: Registry:: Registry() : RegistryType()
            {
            }

            Axiom:: Registry:: ~Registry() throw()
            {
            }

            std::ostream & operator<<(std::ostream &os, const Axiom::Registry &r)
            {
                return r.display(os);
            }

#if 0
            namespace {

                struct AxiomRegistryDisplay
                {
                    std::ostream *dest;
                    size_t        indx;
                    size_t        size;
                    inline bool operator()(const suffix_path    &,
                                           const Axiom::Pointer &ptr)
                    {
                        std::ostream &os = *dest;
                        ++indx;
                        os << ptr->name;
                        if(indx<size) os << ';';
                        return true;
                    }
                };
            }
#endif
            std::ostream & Axiom:: Registry:: display(std::ostream &os) const
            {
                //AxiomRegistryDisplay cb = { &os, 0, entries() };
                //for_each(cb);
                return os;
            }

            void Axiom:: Registry:: ensure(const string &who, Axiom &axiom)
            {
                static const char fn[] = "Axiom::Registry: ";

                const string  &key = *axiom.name;
                Axiom * const *ppA = search_by( key );
                if(!ppA)
                {
                    if(!insert(key,&axiom))
                    {
                        throw exception("%sunexpected failure to record <%s> for %s",fn,*key,*who);
                    }
                }
                else
                {
                    const Axiom &owned = **ppA;
                    if( &owned != &axiom )
                    {
                        throw exception("%sdifferent axioms <%s> for %s",fn,*key,*who);
                    }
                }
            }

            void Axiom:: called_by(const Axiom &parent) const
            {
                //if(Verbose) { std::cerr << "<" << name << "> <== <" << parent.name << ">" << std::endl; }
                aliasing::_(from).ensure(*name,aliasing::_(parent));
            }

        }

    }

}

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            bool Axiom::Verbose  = false;
            const char  Axiom::Prefix[] = "[JIVE] ";
            
            Axiom:: ~Axiom() throw()
            {
                aliasing::_(uuid) = 0;
                aliasing::_(self) = 0;
            }

            Axiom:: Axiom(const uint32_t u, const string *s) throw() :
            uuid(u),
            name(s),
            self(0),
            from()
            {
            }

            void Axiom:: I_am(const void *addr) throw()
            {
                assert(0==self);
                assert(0!=addr);
                aliasing::_(self) = addr;
            }

            bool Axiom:: isTerminal() const throw()
            {
                return TermUUID == uuid;
            }

            bool Axiom:: isInternal() const throw()
            {
                return TermUUID != uuid;
            }

            void Axiom:: Grow( Node * &tree, Node *leaf ) throw()
            {
                assert(NULL!=leaf);
                if(NULL==tree)
                {
                    tree = leaf;
                }
                else
                {
                    assert(tree->kind==Node::IsInternal);
                    tree->leaves().push_back(leaf);
                }
            }

            void  Observer::mind(const Lexeme *now) throw()
            {
                assert(now);
                if( (NULL==lexeme) || (now->stamp>lexeme->stamp) )
                {
                    lexeme=now;
                    parent=tested;
                }
            }

            
            bool Axiom:: accept( Y_JIVE_AXIOM_ACCEPT_ARGS ) const
            {
                try
                {
                    return accept_(tree,lexer,source,guess,depth);
                }
                catch(...)
                {
                    if(tree)
                    {
                        Node::Release(tree);
                        tree = 0;
                    }
                    throw;
                }
            }

            bool Axiom:: new_in(Axiom::Registry &db) const
            {
                Axiom &impl = aliasing::_(*this);
                return db.insert(*name,&impl);
            }


            std::ostream & Axiom:: indent(std::ostream &os, long depth) const
            {
                os << Prefix;
                while(depth-- > 0 )
                {
                    os << ' ' << ' ';
                }
                os << "|_[" << fourcc_(uuid) << "]<" << name <<"> ";
                return os;
            }

            
        }
    }
}


#include "y/string/convert.hpp"
namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            string Axiom:: gvName() const
            {
                return string_convert:: to_printable(*name);
            }
        }
    }
}

