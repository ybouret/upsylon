
#include "y/yap/prime/ratio.hpp"
#include "y/type/aliasing.hpp"
#include "y/exceptions.hpp"
#include <cerrno>
#include <iostream>

namespace upsylon {

    namespace yap {

        const char prime_ratio:: CLASS_NAME[] = "yapQ";

        const char * prime_ratio:: className() const throw()
        {
            return CLASS_NAME;
        }

        size_t prime_ratio:: serialize(ios::ostream &fp) const
        {
            const size_t tmp = num.serialize(fp);
            return       tmp + den.serialize(fp);
        }

        void prime_ratio:: check() const
        {
            if( den.is_zero() )
                throw libc::exception(EDOM,"%s division by zero", className());
        }

        prime_ratio:: prime_ratio() :
        num(),
        den(1)
        {
        }

        prime_ratio:: ~prime_ratio() throw()
        {
        }

        prime_ratio:: prime_ratio(const prime_ratio &other) :
        num(other.num),
        den(other.den)
        {

        }

        prime_ratio & prime_ratio:: operator=( const prime_ratio &other )
        {

            prime_ratio tmp(other);
            xch(tmp);
            return *this;
        }

        void prime_ratio:: xch(prime_ratio &q) throw()
        {
            aliasing::_(num).xch( aliasing::_(q.num) );
            aliasing::_(den).xch( aliasing::_(q.den) );

        }

        std::ostream & operator<<(std::ostream &os, const prime_ratio &q)
        {
            assert(!q.den.is_zero());
            if(q.den.is_one())
            {
                os << q.num;
            }
            else
            {
                os << '(' << q.num << '/' << q.den << ')';
            }
            return os;
        }

        void prime_ratio:: update() throw()
        {
            assert(!den.is_zero());

            // regularise
            if(num.is_zero())
            {
                if(!den.is_one())
                {
                    prime_factors d(1);
                    aliasing::_(den).xch(d);
                    assert(den.is_one());
                }
                return;
            }

            if(den.is_one()||num.is_one()) return;

            pf_list &n = aliasing::_(num.factors);
            pf_list &d = aliasing::_(den.factors);
            {
                pf_list N,D;

                while(n.size&&d.size)
                {
                    switch( natural::scmp(n.head->p,d.head->p) )
                    {
                        case __negative: N.push_back( n.pop_front() ); break;
                        case __positive: D.push_back( d.pop_front() ); break;
                        case __zero:
                        {
                            if(n.head->n<d.head->n)
                            {
                                
                            }
                        } break;
                    }
                }
            }

        }


    }

}

