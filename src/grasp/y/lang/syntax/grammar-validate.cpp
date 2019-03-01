#include "y/lang/syntax/grammar.hpp"
#include "y/exception.hpp"
#include "y/associative/map.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            namespace
            {
                typedef map<string,unsigned,RuleReference::KeyHasher,RuleReference::Memory> RDB_Type;

                class RDB : public RDB_Type
                {
                public:
                    const Tag  name;
                    const bool verbose;

                    explicit RDB(const Tag &id, const size_t n, const bool v) :
                    RDB_Type(n,as_capacity), name(id), verbose(v)
                    {
                    }

                    virtual ~RDB() throw() { }

                    inline bool visited( const string &id )
                    {
                        unsigned *pCount = search(id);
                        if(!pCount)
                        {
                            Y_LANG_SYNTAX_VERBOSE("{" << *name << "} visiting '" << id << "'" << std::endl);
                            if(!insert(id,1)) throw exception("{%s} unexpected visited failure", **name);
                            return false;
                        }
                        else
                        {
                            assert(*pCount>0);
                            ++(*pCount);
                            return true;
                        }
                    }




                    void visit( const Rule *r )
                    {
                        switch(r->uuid)
                        {
                            case Terminal::UUID: (void) visited(r->name);
                                break;

                            case Aggregate:: UUID:
                            case Alternate:: UUID:
                                if(!visited(r->name))
                                {
                                    assert(r->derived);
                                    const Compound *compound = static_cast<const Compound *>(r->derived);
                                    for(const Operand *op = compound->head;op;op=op->next)
                                    {
                                        visit( &(op->rule) );
                                    }
                                }
                                break;

                            case Optional  ::UUID:
                            case OneOrMore ::UUID:
                            case ZeroOrMore::UUID:
                                if(!visited(r->name))
                                {
                                    assert(r->derived);
                                    visit( static_cast<const Rule *>(r->derived) );
                                }
                                break;

                            default:
                                throw exception("{%s} unexpected Syntax::Rule UUID [%04x] for '%s'", **name, unsigned(r->uuid), *(r->name) );
                        }
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(RDB);
                };


            }

            void Grammar:: validate() const
            {
                Y_LANG_SYNTAX_VERBOSE("{" << *name << "} validating..." << std::endl);
                if(rules.size<=0)
                {
                    throw exception("{%s} no top level rule", **name );
                }
                RDB rdb( name, rules.size, verbose);
                for(const Rule *r=rules.head;r;r=r->next)
                {
                    rdb.visit(r);
                }

                for(const Rule *r=rules.head;r;r=r->next)
                {
                    if(! rdb.search(r->name) )
                    {
                        throw exception("{%s} standalone rule %s '%s'", **name, r->typeName(), *(r->name) );
                    }
                }
                Y_LANG_SYNTAX_VERBOSE("{" << *name << "} seems valid!" << std::endl);
            }

        }

    }
}

