#include "y/mpl/pfd.hpp"
#include <cerrno>
#include "y/exceptions.hpp"

namespace upsylon
{
    namespace mpl
    {

        _pfd:: _pfd(const natural &_p, const natural &_q) :
        counted_object(),
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
        counted_object(),
        p( other.p ),
        q( other.q )
        {
            assert( ! q.is_zero() );
        }

        const natural &  _pfd::  key() const throw() { return p; }

        std::ostream & operator<<( std::ostream &os, const _pfd &entry )
        {
            assert( entry.q.is_positive() );
            os << entry.p;
            if( !entry.q.is_byte(1) ) os << '^' << entry.q;
            return os;
        }

        int _pfd:: compare_data( const pointer &lhs, const pointer &rhs ) throw()
        {
            return natural::compare(lhs->p,rhs->p);
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

        pfd:: pfd( const pfd &other ) :
        counted_object(),
        table( other.table )
        {
        }
        

        namespace
        {
            class Node : public core::inode<Node>
            {
            public:
                inline Node( const natural &p, const natural &q, bool already=false) :
                P(p), Q(q), exists(already)
                {
                }

                inline virtual ~Node() throw()
                {}

                const natural  P;
                const natural  Q;
                const bool     exists;

                inline void put( _pfd::table &self ) const
                {
                    const _pfd::pointer tmp = new _pfd(P,Q);
                    if(!self.insert(tmp))
                    {
                        throw exception("mpl.pfd: unexpected multiple factor in setup!");
                    }
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);
            };
        }

        void pfd:: setup(const natural &value)
        {
            assert(0==table.size());

            if( value.is_byte(0) )
            {
                // do nothing
            }
            else
            {
                const MPN & _ = MPN::instance();

                const _pfd::table &const_self = table;
                _pfd::table       &self       = (_pfd::table &)const_self;

                if ( value.is_byte(1) )
                {
                    self.reserve(1);
                    const _pfd::pointer tmp = new _pfd( _._1, _._1 );
                    if(!self.insert(tmp))
                    {
                        throw exception("mpl.pfd: unexpected multiple 1!");
                    }
                }
                else
                {
                    assert(value>1);
                    core::list_of_cpp<Node> factors;

                    natural v = value; //!< current value
                    natural q;

                    for( natural p = 2; p<=v ; p = _.nextPrime(++p) )
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
                        node->put(self);
                    }
                }
            }
        }

        void pfd:: mul_by( const pfd &other )
        {

            const table_t &const_self = table;
            table_t       &self       = (table_t &)const_self;

            if( this == &other )
            {
                for( _pfd::table::iterator i = self.begin(); i != self.end(); ++i )
                {
                    _pfd    &F = **i;
                    natural &Q = F.q;
                    if( ! F.p.is_byte(1) )
                    {
                        Q += Q;
                    }
                }
            }
            else
            {
                // first pass: collect factors and check if exists already
                core::list_of_cpp<Node> factors;
                size_t                  count = 0;
                for( table_t::const_iterator i = other.table.begin(); i != other.table.end(); ++i )
                {
                    const _pfd &factor = **i;
                    factors.push_back( new Node(factor.p, factor.q, self.search(factor.p)) );
                    if(false==factors.tail->exists) ++count; // new factor
                }

                // second pass
                self.reserve(count);
                for( Node *node = factors.head; node; node=node->next )
                {
                    _pfd::pointer *pp = self.search(node->P);
                    if(node->exists)
                    {
                        if(!pp) throw exception("mpl.pfd.mul_by(unexpected missing factor)");
                        _pfd &F = **pp;
                        assert(F.p==node->P);
                        if( ! F.p.is_byte(1) )
                        {
                            F.q += node->Q;
                        }
                    }
                    else
                    {
                        if(pp) throw exception("mpl.pfd.mul_by(unexpected multiple factor)");
                        node->put(self);
                    }
                }

                // sort
                self.sort_data( _pfd::compare_data );
                if(self.size()>1)
                {
                    self.no( MPN::instance()._1 );
                }
            }
        }

        std::ostream & operator<<( std::ostream &os, const pfd &F )
        {
            const _pfd::table &self = F.table;
            const size_t       n    = self.size();
            switch(n)
            {
                case 0: os << '0'; break;
                case 1: os << **self.begin(); break;
                default: {
                    os << '(';
                    size_t count = 1;
                    for( pfd::table_t::const_iterator i=self.begin();i!=self.end();++i,++count)
                    {
                        if(count>1) os << '*';
                        os << (**i);
                    }
                    os << ')';
                }
            }
            return os;
        }

        void pfd:: run( hashing::function &H ) const throw()
        {
            for( table_t::const_iterator i=table.begin(); i != table.end(); ++i )
            {
                const _pfd &F = **i;
                H(F.p);
                H(F.q);
            }
        }

    }
}

