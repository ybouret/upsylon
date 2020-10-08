
#include "y/jive/syntax/axiom.hpp"
#include "y/type/aliasing.hpp"

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
            self(0)
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

            bool Axiom:: accept( Y_JIVE_AXIOM_ACCEPT_ARGS ) const
            {
                try
                {
                    return accept_(tree,lexer,source,depth);
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
                return db.insert_by(*name,&impl);
            }


            std::ostream & Axiom:: indent(std::ostream &os, long depth) const
            {
                os << Prefix;
                while(depth-- > 0 )
                {
                    os << '.';
                }
                return os;
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

            Axiom:: Reference:: ~Reference() throw() {}

            Axiom:: Reference::  Reference(const Axiom &a) throw() :
            Object(),
            inode<Reference>(),
            axiom(a) {}

            const Axiom & Axiom:: Reference:: operator*() const throw()
            {
                return axiom;
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

