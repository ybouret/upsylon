
#include "y/yap/rational.hpp"
#include "y/yap/library.hpp"
#include "y/type/aliasing.hpp"
#include "y/os/error.hpp"
#include "y/exceptions.hpp"
#include <cerrno>
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

        apz rational:: to_integer(const char *when) const
        {
            if(!den.is(1))
            {
                libc::exception excp(EDOM,"rational is not an integer");
                if(when)
                {
                    excp.cat(" in %s",when);
                }
            }
            return num;
        }

        bool rational:: is_integer() const throw()
        {
            return den.is(1);
        }
        

        const number::itype rational::i_one;
        const number::utype rational::u_one;

        rational:: rational() : num(0), den(1)
        {
        }

        rational:: ~rational() throw()
        {

        }

        rational:: rational(randomized::bits &ran,const size_t n, const size_t m) :
        number(),
        num(ran,n),
        den(ran, (m<1) ? 1 : m)
        {
            update();
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

        //======================================================================
        //
        // addition
        //
        //======================================================================

        rational rational::add(const rational &lhs, const rational &rhs)
        {
            const  integer Lnum = lhs.num.mul_by(rhs.den);
            const  integer Rnum = rhs.num.mul_by(lhs.den);
            const  integer Anum = Lnum+Rnum;
            const  natural Aden = lhs.den * rhs.den;
            return rational(Anum,Aden);
        }

        template <typename T>
        static inline rational add_proto(const rational &lhs, const T &i)
        {
            const  rational rhs(i);
            return rational::add(lhs,rhs);
        }

        template <typename T>
        static inline rational add_proto(const T &i, const rational &rhs)
        {
            const  rational lhs(i);
            return rational::add(lhs,rhs);
        }

        rational rational:: add(const rational &lhs, const integer &rhs)
        {
            return add_proto(lhs,rhs);
        }

        rational rational:: add(const rational &lhs, const itype rhs)
        {
            return add_proto(lhs,rhs);
        }

        rational rational:: add(const integer &lhs, const rational &rhs)
        {
            return add_proto(lhs,rhs);
        }

        rational rational:: add(const itype lhs, const rational &rhs)
        {
            return add_proto(lhs,rhs);
        }

        rational rational:: operator+() const
        {
            return *this;
        }

        rational & rational:: operator++()
        {
            rational tmp = add_proto(*this,i_one);
            xch(tmp);
            return *this;
        }

        rational   rational:: operator++(int)
        {
            rational ans = *this;
            {
                rational tmp = add_proto(*this,i_one);
                xch(tmp);
            }
            return ans;
        }



        //======================================================================
        //
        // subtraction
        //
        //======================================================================

        rational rational::sub(const rational &lhs, const rational &rhs)
        {
            const  integer Lnum = lhs.num.mul_by(rhs.den);
            const  integer Rnum = rhs.num.mul_by(lhs.den);
            const  integer Anum = Lnum-Rnum;
            const  natural Aden = lhs.den * rhs.den;
            return rational(Anum,Aden);
        }

        template <typename T>
        static inline rational sub_proto(const rational &lhs, const T &i)
        {
            const  rational rhs(i);
            return rational::sub(lhs,rhs);
        }

        template <typename T>
        static inline rational sub_proto(const T &i, const rational &rhs)
        {
            const  rational lhs(i);
            return rational::sub(lhs,rhs);
        }

        rational rational:: sub(const rational &lhs, const integer &rhs)
        {
            return sub_proto(lhs,rhs);
        }

        rational rational:: sub(const rational &lhs, const itype rhs)
        {
            return sub_proto(lhs,rhs);
        }

        rational rational:: sub(const integer &lhs, const rational &rhs)
        {
            return sub_proto(lhs,rhs);
        }

        rational rational:: sub(const itype lhs, const rational &rhs)
        {
            return sub_proto(lhs,rhs);
        }

        rational rational:: operator-() const
        {
            const integer i = -num;
            return rational(i,den);
        }

        rational & rational:: operator--()
        {
            rational tmp = sub_proto(*this,i_one);
            xch(tmp);
            return *this;
        }

        rational   rational:: operator--(int)
        {
            rational ans = *this;
            {
                rational tmp = sub_proto(*this,i_one);
                xch(tmp);
            }
            return ans;
        }


        //======================================================================
        //
        // multiplication
        //
        //======================================================================

        rational rational::mul(const rational &lhs, const rational &rhs)
        {
            const integer Anum = lhs.num * rhs.num;
            const natural Aden = lhs.den * rhs.den;
            return rational(Anum,Aden);
        }

        template <typename T>
        static inline rational mul_proto(const rational &lhs, const T &i)
        {
            const  rational rhs(i);
            return rational::mul(lhs,rhs);
        }

        template <typename T>
        static inline rational mul_proto(const T &i, const rational &rhs)
        {
            const  rational lhs(i);
            return rational::mul(lhs,rhs);
        }

        rational rational:: mul(const rational &lhs, const integer &rhs)
        {
            return mul_proto(lhs,rhs);
        }

        rational rational:: mul(const rational &lhs, const itype rhs)
        {
            return mul_proto(lhs,rhs);
        }

        rational rational:: mul(const integer &lhs, const rational &rhs)
        {
            return mul_proto(lhs,rhs);
        }

        rational rational:: mul(const itype lhs, const rational &rhs)
        {
            return mul_proto(lhs,rhs);
        }

        rational rational:: square_of(const rational &q)
        {
            const integer Anum = integer::square_of(q.num);
            const natural Aden = natural::square_of(q.den);
            return rational(Anum,Aden);
        }

        rational rational:: abs_of(const rational &q)
        {
            rational a(q);
            aliasing::_(a.num.s) = number::product(a.num.s,a.num.s);
            return a;
        }

        rational rational:: sqrt_of(const rational &q)
        {
            const integer Anum = integer::sqrt_of(q.num); assert(Anum*Anum<=q.num);
            natural       Aden = natural::sqrt_of(q.den);
            const integer Anum2 = integer::square_of(Anum);
            const integer Amini = Anum2.mul_by(q.den);

            while(true)
            {
                const natural Aden2 = natural::square_of(Aden);
                const integer Acurr = q.num.mul_by(Aden2);
                if(Acurr<Amini) ++Aden; else break;
            }

            return rational(Anum,Aden);
        }

        //======================================================================
        //
        // division
        //
        //======================================================================
        rational rational::divide(const rational &lhs, const rational &rhs)
        {
            const integer Inum = lhs.num.mul_by(rhs.den);
            const integer Iden = rhs.num.mul_by(lhs.den);
            const integer Anum( product(Inum.s,Iden.s), Inum.n );
            return rational(Anum,Iden.n);
        }

        template <typename T>
        static inline rational div_proto(const rational &lhs, const T &i)
        {
            const  rational rhs(i);
            return rational::divide(lhs,rhs);
        }

        template <typename T>
        static inline rational div_proto(const T &i, const rational &rhs)
        {
            const  rational lhs(i);
            return rational::divide(lhs,rhs);
        }

        rational rational:: divide(const rational &lhs, const integer &rhs)
        {
            return div_proto(lhs,rhs);
        }

        rational rational:: divide(const rational &lhs, const itype rhs)
        {
            return div_proto(lhs,rhs);
        }

        rational rational:: divide(const integer &lhs, const rational &rhs)
        {
            return div_proto(lhs,rhs);
        }

        rational rational:: divide(const itype lhs, const rational &rhs)
        {
            return div_proto(lhs,rhs);
        }

    }

}
