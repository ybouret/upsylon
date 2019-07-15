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
                throw exception("pfd: invalid zero exponent");
            }
        }

        _pfd:: ~_pfd() throw()
        {

        }

        _pfd:: _pfd( const _pfd &other ) :
        object(),
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
#include "y/mpl/mpn.hpp"

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
        object(),
        counted_object(),
        table()
        {
            const natural _(n);
            setup(_);
        }

        pfd:: pfd( const pfd &other ) :
        object(),
        counted_object(),
        table( other.table )
        {
        }

        void pfd:: ldz() throw()
        {
            _pfd::table &self  = (_pfd::table &)table;
            self.release();
            assert( is_zero() );
        }

        void pfd:: ld1()
        {
            _pfd::table &self  = (_pfd::table &)table;
            const MPN   & _    = MPN::instance();

            self.release();
            self.ensure(1);
            const _pfd::pointer tmp = new _pfd(_._1,_._1);
            if(!self.insert(tmp))
            {
                throw exception("mpl.pfd.ld1: unexpected multiple 1!");
            }
            assert( is_one() );
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
                        while( v.is_divisible_by(p) )
                        {
                            ++q;
                            v /= p;
                        }
                        if(q.is_positive())
                        {
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
            if( is_zero() )     return;             //!< won't change a thing
            if( other.is_one()) return;             //!< won't change a thing
            if( other.is_zero()) { ldz(); return; } //!< in any case

            // general case
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
                if(self.size()>1) self.no( MPN::instance()._1 );
            }


        }

        void pfd:: mul_by( const natural &n )
        {
            const pfd _(n);
            mul_by(_);
        }

        void pfd:: mul_by( const word_t n )
        {
            const pfd _(n);
            mul_by(_);
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


        bool pfd:: is_zero() const throw()
        {
            return table.size() <= 0;
        }

        bool pfd:: is_one() const throw()
        {
            if( 1 == table.size() )
            {
                const table_t::const_iterator i = table.begin();
                const _pfd &F = **i;
                const mpn  &p = F.p; assert(p>0);
                if( p.is_byte(1) )
                {
                    assert(F.q.is_byte(1));
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        bool operator==( const pfd &lhs, const pfd &rhs ) throw()
        {
            const _pfd::table &L = lhs.table;
            const _pfd::table &R = rhs.table;
            size_t             N = L.size();
            if(N!=R.size())
            {
                return false;
            }
            else
            {
                _pfd::table::const_iterator l = L.begin(), r=R.begin();
                while(N-->0)
                {
                    const _pfd &ll = **l;
                    const _pfd &rr = **r;
                    if( (ll.p != rr.p) || (ll.q!=rr.q) )
                    {
                        return false;
                    }
                    ++l;
                    ++r;
                }
                return true;
            }
        }

        bool operator!=( const pfd &lhs, const pfd &rhs ) throw()
        {
            return !(lhs==rhs);
        }


        natural pfd:: value() const
        {
            if( is_zero() )
            {
                return mpn();
            }
            else if( is_one() )
            {
                return mpn(1);
            }
            else
            {
                mpn ans(1);
                for( table_t::const_iterator i = table.begin();i!=table.end();++i)
                {
                    const _pfd &F = **i;
                    for(mpn j;j<F.q;++j)
                    {
                        ans *= F.p;
                    }
                }
                return ans;
            }
        }

        pfd pfd:: factorial(word_t n)
        {
            pfd f(1);
            while(n>1)
            {
                f.mul_by(n);
                --n;
            }

            return f;
        }


    }
}

