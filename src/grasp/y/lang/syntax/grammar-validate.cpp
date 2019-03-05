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
                    int        level;

                    std::ostream & indent(std::ostream &os) const
                    {
                        for(int i=level;i>0;--i) os << "..";
                        return os;
                    }

                    explicit RDB(const Tag &id, const size_t n, const bool v) :
                    RDB_Type(n,as_capacity), name(id), verbose(v), level(0)
                    {
                    }

                    virtual ~RDB() throw() { }

                    inline bool visited( const Rule *r )
                    {
                        assert(r);
                        const string &id = r->name;
                        unsigned *pCount = search(id);
                        if(!pCount)
                        {
                            Y_LANG_SYNTAX_VERBOSE( std::cerr << "{" << *name << "} "; indent(std::cerr) << "visiting <" << id << "> / " << r->typeName() << std::endl);
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
                            case Terminal::UUID: (void) visited(r);
                                break;

                            case Aggregate:: UUID:
                            case Alternate:: UUID:
                                if(!visited(r))
                                {
                                    assert(r->derived);
                                    const Compound *compound = static_cast<const Compound *>(r->derived);
                                    ++level;
                                    for(const Operand *op = compound->head;op;op=op->next)
                                    {
                                        visit( &(op->rule) );
                                    }
                                    --level;
                                }
                                break;

                            case Optional  ::UUID:
                            case OneOrMore ::UUID:
                            case ZeroOrMore::UUID:
                                if(!visited(r))
                                {
                                    ++level;
                                    assert(r->derived);
                                    visit( &(static_cast<const Joker *>(r->derived)->jk) );
                                    --level;
                                }
                                break;

                            default:
                                throw exception("{%s} unexpected Syntax::Rule UUID [%04x] for <%s>", **name, unsigned(r->uuid), *(r->name) );
                        }
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(RDB);
                };


            }

            void Grammar:: validate() const
            {
                Y_LANG_SYNTAX_VERBOSE(std::cerr << "{" << *name << "} validating..." << std::endl);
                if(rules.size<=0)
                {
                    throw exception("{%s} no top level rule", **name );
                }

                // prepare the database
                RDB rdb( name, rules.size, verbose);

                // recursively check rules
                rdb.visit(rules.head);

                for(const Rule *r=rules.head;r;r=r->next)
                {
                    if(! rdb.search(r->name) )
                    {
                        throw exception("{%s} standalone %s <%s>", **name, r->typeName(), *(r->name) );
                    }
                    if(verbose)
                    {
                        std::cerr << "\tChecking astTermCount for " << r->name << std::endl;
                    }
                }
                Y_LANG_SYNTAX_VERBOSE(std::cerr << "{" << *name << "} seems valid!" << std::endl);
            }

        }

    }
}

