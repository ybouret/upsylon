
#include "y/yap/rational.hpp"
#include "y/yap/library.hpp"
#include "y/type/aliasing.hpp"
#include "y/os/error.hpp"
#include <iostream>

namespace upsylon {


    namespace yap {

        std::ostream & operator<<(std::ostream &os, const rational &q)
        {
            if(q.den==1)
            {
                os << q.num;
            }
            else
            {
                os << '(' << q.num << '/' << q.den << ')';
            }
            return os;
        }

        double rational:: to_double() const
        {
            switch(num.s)
            {
                case __negative:  return -natural::ratio_of(num.n,den);
                case __zero:      break;
                case __positive:  return natural::ratio_of(num.n,den);
            }
            return 0.0;
        }


        const number::itype rational::i_one;
        const number::utype rational::u_one;

        rational:: rational() : num(0), den(1)
        {
        }

        rational:: ~rational() throw()
        {

        }

        void rational:: update()
        {
            natural::simplify(aliasing::_(num.n), aliasing::_(den));
        }

#define Y_APQ_CTOR(LHS,RHS) \
rational:: rational(const LHS n, const RHS d) : num(n), den(d) { update(); }

        Y_APQ_CTOR(integer&,natural&)
        Y_APQ_CTOR(itype,utype)
        Y_APQ_CTOR(itype,natural&)
        Y_APQ_CTOR(integer&,utype)

        rational:: rational(const itype    n) : num(n), den(1) {}
        rational:: rational(const integer &n) : num(n), den(1) {}
        rational:: rational(const natural &n) : num(n), den(1) {}


        rational::rational(const rational &q) : number(), num(q.num), den(q.den)
        {
        }

        rational & rational:: operator=(const rational &q)
        {
            rational temp(q);
            xch(temp);
            return *this;
        }

        //======================================================================
        //
        // serialize
        //
        //======================================================================
        const char rational:: CLASS_NAME[] = "yapq";

        const char *rational::className() const throw() { return CLASS_NAME; }


        size_t rational:: serialize(ios::ostream &fp) const
        {
            size_t ans = num.serialize(fp);
            return ans + den.serialize(fp);
        }

        rational rational:: read(ios::istream &fp, size_t &delta, const char *which)
        {
            size_t         del_num = 0;
            const  integer the_num = integer::read(fp,del_num,which);
            size_t         del_den = 0;
            const natural  the_den = natural::read(fp,del_den,which);
            delta = del_num + del_den;
            return rational(the_num,the_den);

        }

        //======================================================================
        //
        // exchange
        //
        //======================================================================
        void rational:: xch(rational &q) throw()
        {
            aliasing::_(num).xch( aliasing::_(q.num) );
            aliasing::_(den).xch( aliasing::_(q.den) );
        }

        //======================================================================
        //
        // comparison
        //
        //======================================================================
        int rational:: cmp(const rational &lhs, const rational &rhs)
        {
            switch(lhs.num.s)
            {

                    // lhs < 0
                case __negative:
                    switch(rhs.num.s)
                    {
                        case __negative:
                        {
                            const natural L = lhs.num.n * rhs.den;
                            const natural R = rhs.num.n * lhs.den;
                            return natural::compare(R,L);
                        }
                        case __zero:
                        case __positive: return -1;
                    }
                    break;

                    // lhs = 0
                case __zero:
                    switch(rhs.num.s)
                    {
                        case __negative: return  1;
                        case __zero:     return  0;
                        case __positive: return  -1;
                    }
                    break;

                    // lhs > 0
                case __positive:
                    switch(rhs.num.s)
                    {
                        case __negative:
                        case __zero:     return 1;
                        case __positive:
                        {
                            const natural L = lhs.num.n * rhs.den;
                            const natural R = rhs.num.n * lhs.den;
                            return natural::compare(L,R);
                        }
                    }
                    break;
            }
            fatal_error("rational corrupted sign");
            return 0;
        }

        template <typename T>
        static inline int cmp_proto(const rational &lhs, const T &i)
        {
            const rational rhs(i);
            return rational::cmp(lhs,rhs);
        }

        template <typename T>
        static inline int cmp_proto(const T &i, const rational &rhs)
        {
            const rational lhs(i);
            return rational::cmp(lhs,rhs);
        }

        int rational:: cmp(const rational &lhs, const integer &rhs)
        {
            return cmp_proto(lhs,rhs);
        }

        int rational:: cmp(const rational &lhs, const itype rhs)
        {
            return cmp_proto(lhs,rhs);
        }

        int rational:: cmp(const integer &lhs, const rational &rhs)
        {
            return cmp_proto(lhs,rhs);
        }

        int rational:: cmp(const itype lhs, const rational &rhs)
        {
            return cmp_proto(lhs,rhs);
        }
    }

}
