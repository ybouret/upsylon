
//! \file

#ifndef Y_AP_RATIONAL_INCLUDED
#define Y_AP_RATIONAL_INCLUDED 1

#include "y/yap/integer.hpp"

namespace upsylon {


    namespace yap {

        //______________________________________________________________________
        //
        //
        //! rational class
        //
        //______________________________________________________________________

        class rational : public number
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char    CLASS_NAME[]; //!< "apq"
            static const itype   i_one = 1;    //!< signed one
            static const utype   u_one = 1;    //!< unsigned one

            //__________________________________________________________________
            //
            // C++ and setup
            //__________________________________________________________________
            virtual ~rational() throw();               //!< cleanup
            rational();                                //!< 0/1
            rational(const rational &);                //!< copy
            rational(const integer &,const natural &); //!< setup
            rational(const itype    ,const utype    ); //!< setup
            rational(const itype    ,const natural &); //!< setup
            rational(const integer &,const utype    ); //!< setup
            rational & operator=(const rational &);    //!< assign
            rational(const itype);                     //!< setup/1
            rational(const integer &);                 //!< setup/1
            rational(const natural &);                 //!< setup/1
            rational(randomized::bits &,const size_t n, const size_t m); //!< setup random

            //__________________________________________________________________
            //
            // serializable
            //__________________________________________________________________
            virtual const char *className() const throw();                    //!< CLASS_NAME
            virtual size_t      serialize(ios::ostream &) const;              //!< output
            static  rational    read(ios::istream &, size_t &, const char *); //!< reload

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void xch(rational &q) throw(); //!< no-throw exchange

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const integer num; //!< numerator
            const natural den; //!< denominator

            //__________________________________________________________________
            //
            // output
            //__________________________________________________________________
            friend std::ostream & operator<<(std::ostream &, const rational &); //!< output
            double to_double() const;                                           //!< signed ration

            //__________________________________________________________________
            //
            // full comparision
            //__________________________________________________________________
            //! implementing full comparison
#define Y_APQ_CMP_FULL(OP,CALL) \
inline friend bool operator OP (const rational &lhs, const rational &rhs) throw() { return CALL(lhs,rhs.num,rhs.den); }\
inline friend bool operator OP (const rational &lhs, const integer  &rhs) throw() { return CALL(lhs,rhs,u_one);       }\
inline friend bool operator OP (const integer  &lhs, const rational &rhs) throw() { return CALL(rhs,lhs,u_one);       }\
inline friend bool operator OP (const rational &lhs, const itype     rhs) throw() { return CALL(lhs,rhs,u_one);       }\
inline friend bool operator OP (const itype     lhs, const rational &rhs) throw() { return CALL(rhs,lhs,u_one);       }\


            Y_APQ_CMP_FULL(==,eq)
            Y_APQ_CMP_FULL(!=,neq)

            //__________________________________________________________________
            //
            // partial comparisons
            //__________________________________________________________________
            //! declaring functions
#define Y_APQ_DECL(RETURN,CALL) \
static RETURN CALL (const rational &lhs, const rational &rhs);\
static RETURN CALL (const rational &lhs, const integer  &rhs);\
static RETURN CALL (const integer  &lhs, const rational &rhs);\
static RETURN CALL (const rational &lhs, const itype     rhs);\
static RETURN CALL (const itype     lhs, const rational &rhs)

            Y_APQ_DECL(int,cmp); //!< aliases to cmp

            //! implement a partial comparison
#define Y_APQ_CMP_PARTIAL(OP) \
inline friend bool operator OP (const rational &lhs, const rational &rhs) { return cmp(lhs,rhs) OP 0; }\
inline friend bool operator OP (const rational &lhs, const integer  &rhs) { return cmp(lhs,rhs) OP 0; }\
inline friend bool operator OP (const integer  &lhs, const rational &rhs) { return cmp(lhs,rhs) OP 0; }\
inline friend bool operator OP (const rational &lhs, const itype     rhs) { return cmp(lhs,rhs) OP 0; }\
inline friend bool operator OP (const itype     lhs, const rational &rhs) { return cmp(lhs,rhs) OP 0; }

            //! implement all partial comparisons
#define Y_APQ_CMP_PARTIAL_ALL()  \
Y_APQ_CMP_PARTIAL(<)             \
Y_APQ_CMP_PARTIAL(>)             \
Y_APQ_CMP_PARTIAL(<=)            \
Y_APQ_CMP_PARTIAL(>=)

            Y_APQ_CMP_PARTIAL_ALL()

            //! for sorting algorithms
            static int compare(const rational &lhs,const rational &rhs) { return cmp(lhs,rhs); }

            //__________________________________________________________________
            //
            // additions
            //__________________________________________________________________
            //! declaring all overloaded functions
#define Y_APQ_WRAP_API(CALL)    Y_APQ_DECL(rational,CALL)

            //! implementing all operators
#define Y_APQ_WRAP_OPS(OP,CALL) \
inline friend rational  operator OP   (const rational &lhs, const rational &rhs) { return CALL(lhs,rhs); } \
inline friend rational  operator OP   (const rational &lhs, const integer  &rhs) { return CALL(lhs,rhs); } \
inline friend rational  operator OP   (const integer  &lhs, const rational &rhs) { return CALL(lhs,rhs); } \
inline friend rational  operator OP   (const rational &lhs, const itype     rhs) { return CALL(lhs,rhs); } \
inline friend rational  operator OP   (const itype     lhs, const rational &rhs) { return CALL(lhs,rhs); }\
inline        rational &operator OP##=(const rational &rhs) { rational tmp = CALL(*this,rhs); xch(tmp); return *this; }\
inline        rational &operator OP##=(const integer  &rhs) { rational tmp = CALL(*this,rhs); xch(tmp); return *this; }\
inline        rational &operator OP##=(const itype     rhs) { rational tmp = CALL(*this,rhs); xch(tmp); return *this; }

            Y_APQ_WRAP_API(add); //!< aliases
            Y_APQ_WRAP_OPS(+,add)

            rational   operator+() const; //!< unary '+'
            rational & operator++();      //!< prefix  ++ operator
            rational   operator++(int);   //!< postfix ++ operator

            //__________________________________________________________________
            //
            // subtraction
            //__________________________________________________________________
            Y_APQ_WRAP_API(sub); //!< aliases
            Y_APQ_WRAP_OPS(-,sub)

            rational   operator-() const; //!< unary '-'
            rational & operator--();      //!< prefix  -- operator
            rational   operator--(int);   //!< postfix -- operator

            //__________________________________________________________________
            //
            // multiplication
            //__________________________________________________________________
            Y_APQ_WRAP_API(mul); //!< aliases
            Y_APQ_WRAP_OPS(*,mul)

            //__________________________________________________________________
            //
            // division
            //__________________________________________________________________
            Y_APQ_WRAP_API(divide); //!< aliases
            Y_APQ_WRAP_OPS(/,divide)


        private:
            void update();
            template <typename NUM,typename DEN>
            static inline bool eq(const rational &q, const NUM &n, const DEN &d) throw()
            {
                return (q.num==n) && (q.den==d);
            }

            template <typename NUM,typename DEN>
            static inline bool neq(const rational &q, const NUM &n, const DEN &d) throw()
            {
                return (q.num!=n) || (q.den!=d);
            }

        };

    }
    
    typedef yap::rational apq; //!< nickname

}

#endif
