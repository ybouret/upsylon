#include "y/mpl/pfd.hpp"
#include <cerrno>
#include "y/exceptions.hpp"

namespace upsylon
{
    namespace mpl
    {

        _pfd:: _pfd(const natural &_p, const natural &_q) :
        p( _p ),
        q( _q )
        {
            if( q.is_zero() )
            {
                throw exception("_pfd: invalid zero exponent");
            }
        }

        _pfd:: ~_pfd() throw()
        {

        }

        _pfd:: _pfd( const _pfd &other ) :
        p( other.p ),
        q( other.q )
        {
            assert( ! q.is_zero() );
        }

    }
}

#if 0
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
#endif

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
        table()
        {
            setup(n);
        }

        pfd:: pfd( const word_t n ) :
        table()
        {
            const natural _(n);
            setup(_);
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

        void pfd:: setup(const natural &value)
        {
            if( value.is_byte(0) )
            {
                // do nothing
            }
            else
            {
                const _pfd::table &const_self = table;
                _pfd::table       &self       = (_pfd::table &)const_self;

                if ( value.is_byte(1) )
                {
                    self.reserve(1);
                    MPN & _ = MPN::instance();
                    const _pfd::pointer tmp = new _pfd( _._1, _._1 );
                    if(!self.insert(tmp))
                    {
                        throw exception("mpp: unexpected multiple 1!");
                    }
                }
                else
                {
                    assert(value>1);
                    core::list_of_cpp<Node> factors;

                    natural v = value; //!< current value
                    natural q;

                    for( natural p = 2; p<=v ; p = natural::next_prime(++p) )
                    {
                        q.clr();
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



                    self.reserve(factors.size);

                    for(const Node *node = factors.head; node; node=node->next )
                    {
                        const _pfd::pointer tmp = new _pfd( node->P, node->Q );
                        if(!self.insert(tmp))
                        {
                            throw exception("mpp: unexpected multiple factor in setup!");
                        }
                    }
                }
            }
        }

    }
}

