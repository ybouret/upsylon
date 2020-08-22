
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
            // macro to overload
            //__________________________________________________________________

            //! q,q
#define Y_APQ_ARGS_Q_Q (const rational &lhs, const rational &rhs)
            //! q,z
#define Y_APQ_ARGS_Q_Z (const rational &lhs, const integer  &rhs)
            //! z,q
#define Y_APQ_ARGS_Z_Q (const integer  &lhs, const rational &rhs)
            //! q,itype
#define Y_APQ_ARGS_Q_I (const rational &lhs, const itype     rhs)
            //! itype,q
#define Y_APQ_ARGS_I_Q (const itype     lhs, const rational &rhs)

            //! method implementation helper
#define Y_APQ_METHOD_(PREFIX,ARGS,SUFFIX) PREFIX ARGS SUFFIX

            //! methods implementation helper
#define Y_APQ_METHODS(PREFIX,SUFFIX) \
Y_APQ_METHOD_(PREFIX,Y_APQ_ARGS_Q_Q,SUFFIX) \
Y_APQ_METHOD_(PREFIX,Y_APQ_ARGS_Q_Z,SUFFIX) \
Y_APQ_METHOD_(PREFIX,Y_APQ_ARGS_Z_Q,SUFFIX) \
Y_APQ_METHOD_(PREFIX,Y_APQ_ARGS_Q_I,SUFFIX) \
Y_APQ_METHOD_(PREFIX,Y_APQ_ARGS_I_Q,SUFFIX) \



            //__________________________________________________________________
            //
            // full comparision
            //__________________________________________________________________
            //! implementing full comparison, specific
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
#define Y_APQ_DECL(RETURN,CALL)    \
static RETURN CALL Y_APQ_ARGS_Q_Q; \
static RETURN CALL Y_APQ_ARGS_Q_Z; \
static RETURN CALL Y_APQ_ARGS_Z_Q; \
static RETURN CALL Y_APQ_ARGS_Q_I; \
static RETURN CALL Y_APQ_ARGS_I_Q

            Y_APQ_DECL(int,cmp); //!< aliases to cmp

            //! implement a partial comparison
#define Y_APQ_CMP_PARTIAL(OP) \
Y_APQ_METHODS(inline friend bool operator OP,{ return cmp(lhs,rhs) OP 0; })


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
Y_APQ_METHODS(inline friend rational  operator OP,{ return CALL(lhs,rhs); })\
inline rational &operator OP##=(const rational &rhs) { rational tmp = CALL(*this,rhs); xch(tmp); return *this; }\
inline rational &operator OP##=(const integer  &rhs) { rational tmp = CALL(*this,rhs); xch(tmp); return *this; }\
inline rational &operator OP##=(const itype     rhs) { rational tmp = CALL(*this,rhs); xch(tmp); return *this; }

            //! implement one operator from a function
#define Y_APQ_STANDARD(OP,CALL) Y_APQ_WRAP_API(CALL); Y_APQ_WRAP_OPS(OP,CALL)

            Y_APQ_STANDARD(+,add)

            rational   operator+() const; //!< unary '+'
            rational & operator++();      //!< prefix  ++ operator
            rational   operator++(int);   //!< postfix ++ operator

            //__________________________________________________________________
            //
            // subtraction
            //__________________________________________________________________
            Y_APQ_STANDARD(-,sub)

            rational   operator-() const; //!< unary '-'
            rational & operator--();      //!< prefix  -- operator
            rational   operator--(int);   //!< postfix -- operator

            //__________________________________________________________________
            //
            // multiplication
            //__________________________________________________________________
            Y_APQ_STANDARD(*,mul)
            static rational square_of(const rational &q); //!< q^2
            static rational abs_of(const rational &q);    //!< |q|
            static rational sqrt_of(const rational &q);   //!< sqrt(q)

            //__________________________________________________________________
            //
            // division
            //__________________________________________________________________
            Y_APQ_STANDARD(/,divide)


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

    namespace mkl {
        inline apq fabs_of(const apq &x) { return apq::abs_of(x);  } //!< for use in mkl
        inline apq sqrt_of(const apq &x) { return apq::sqrt_of(x); } //!< for use in mkl
        inline apq mod2_of(const apq &x) { return apq::square_of(x); } //!< for use in mkl

    }
}

#endif
