#include "y/mpl/pfd.hpp"
#include <cerrno>
#include "y/exceptions.hpp"

namespace upsylon
{
    namespace mpl
    {

        pfd_entry:: pfd_entry(const natural &_p, const natural &_q) :
        p( _p ),
        q( _q )
        {
            if( q.is_zero() )
            {
                throw exception("mpp: invalid zero exponent");
            }
        }

        pfd_entry:: ~pfd_entry() throw()
        {

        }

        pfd_entry:: pfd_entry( const pfd_entry &other ) :
        p( other.p ),
        q( other.q )
        {
            assert( ! q.is_zero() );
        }

    }
}

namespace upsylon
{
    namespace mpl
    {
        pfd_table:: ~pfd_table() throw()
        {
        }


        pfd_table:: pfd_table() throw() : pfd_entry::table()
        {
        }

        void pfd_table:: add( const natural &p, const natural &q )
        {
            //assert( natural::is_prime(p) );
            if( q.is_zero() )
            {
                return;
            }
            else
            {
                pfd_entry::pointer *ppt = search(p);
                if(ppt)
                {
                    (**ppt).q += q;
                }
                else
                {
                    const pfd_entry::pointer tmp = new pfd_entry(p,q);
                    if(!insert(tmp))
                    {
                        throw exception("mpp: unexpected insert failure");
                    }
                }
            }
        }

        void pfd_table:: sub( const natural &p, const natural &q )
        {
            //assert( natural::is_prime(p) );
            if( q.is_zero() )
            {
                return;
            }
            else
            {
                pfd_entry::pointer *ppt = search(p);
                if(!ppt) throw exception("mpp.sub: no matching factor");
                (**ppt).q -= q;


            }
        }

    }
}

#include "y/core/node.hpp"
#include "y/core/list.hpp"

namespace upsylon
{
    namespace mpl
    {

        pfd:: ~pfd() throw()
        {
        }

        pfd:: pfd( const natural &n ) :
        value( n ),
        table(   )
        {
            setup();
        }

        pfd:: pfd( const word_t n ) :
        value( n ),
        table(   )
        {
            setup();
        }

        namespace
        {
            class Node : public core::inode<Node>
            {
            public:
                inline Node( const natural &p, const natural &q ) :
                P(p), Q(q)
                {
                }

                inline virtual ~Node() throw()
                {}

                const natural P;
                const natural Q;


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);
            };
        }

        void pfd:: setup()
        {
            if( value.is_byte(0) || value.is_byte(1) )
            {
                // do nothing
            }
            else
            {
                assert(value>1);
                core::list_of_cpp<Node> factors;

                natural v = value; //!< current value

                for( natural p = 2; p<=v ; p = natural::next_prime(++p) )
                {
                    natural q = 0;
                    std::cerr << v << "?" << p << std::endl;
                    while( v.is_divisible_by(p) )
                    {
                        ++q;
                        v /= p;
                    }
                    if(q.is_positive())
                    {
                        std::cerr << p << "^" << q << std::endl;
                        factors.push_back( new Node(p,q) );
                    }
                }
                

                const pfd_entry::table &const_self = table;
                pfd_entry::table       &self       = ((pfd_entry::table &)const_self);
                self.reserve(factors.size);

                for(const Node *node = factors.head; node; node=node->next )
                {
                    const pfd_entry::pointer tmp = new pfd_entry( node->P, node->Q );
                    if(!self.insert(tmp))
                    {
                        throw exception("mpp: unexpected multiple factor in setup!");
                    }
                }
            }
        }

    }
}

