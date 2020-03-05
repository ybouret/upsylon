
#include "y/lang/pattern.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/tools/graphviz.hpp"
#include "y/ios/osstream.hpp"
#include "y/type/block-zset.hpp"

namespace upsylon {

    namespace Lang {

        Pattern:: ~Pattern() throw()
        {
            _bzset(uuid);
        }



        Pattern:: Pattern(const uint32_t t) throw() : CountedObject(), uuid(t), next(0), prev(0), priv(0)
        {

        }

        bool Pattern:: multiple() const throw()
        {
            return !univocal();
        }



        const char * Pattern:: vizStyle() const throw()
        {
            return weak() ? "dashed" : "solid";
        }

        std::ostream & operator<<(std::ostream &os, const Pattern &p)
        {
            const string txt = p.to_printable();
            return (os<<txt);
        }

        void Pattern:: NoMultiple( List &ops ) throw()
        {
            {
                List                          tmp;
                while( ops.size )
                {
                    const Pattern *lhs      = ops.head;
                    bool           multiple = false;
                    for(const Pattern *rhs=tmp.head;rhs;rhs=rhs->next)
                    {
                        if( lhs->equals(*rhs) )
                        {
                            multiple = true;
                            break;
                        }
                    }

                    if(multiple)
                    {
                        delete ops.pop_front();
                    }
                    else
                    {
                        tmp.push_back( ops.pop_front() );
                    }
                }
                ops.swap_with(tmp);
            }
#ifndef NDEBUG
            for(const Pattern *lhs=ops.head;lhs;lhs=lhs->next)
            {
                for(const Pattern *rhs=lhs->next;rhs;rhs=rhs->next)
                {
                    assert( ! lhs->equals( *rhs ) );
                }
            }
#endif
        }

    }

}

