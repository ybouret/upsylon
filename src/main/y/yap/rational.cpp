
#include "y/yap/rational.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {


    namespace yap {

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


        rational::rational(const rational &q) : num(q.num), den(q.den)
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


    }

}