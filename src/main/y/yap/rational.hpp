
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
            static const itype   i_one = 1;
            static const utype   u_one = 1;

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
            friend std::ostream & operator<<(std::ostream &, const rational &);
            double to_double() const;

            //__________________________________________________________________
            //
            // full comparision
            //__________________________________________________________________
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
#define Y_APQ_DECL(RETURN,CALL) \
static RETURN CALL (const rational &lhs, const rational &rhs);\
static RETURN CALL (const rational &lhs, const integer  &rhs);\
static RETURN CALL (const integer  &lhs, const rational &rhs);\
static RETURN CALL (const rational &lhs, const itype     rhs);\
static RETURN CALL (const itype     lhs, const rational &rhs)

            Y_APQ_DECL(int,cmp);

#define Y_APQ_CMP_PARTIAL(OP) \
inline friend bool operator OP (const rational &lhs, const rational &rhs) { return cmp(lhs,rhs) OP 0; }\
inline friend bool operator OP (const rational &lhs, const integer  &rhs) { return cmp(lhs,rhs) OP 0; }\
inline friend bool operator OP (const integer  &lhs, const rational &rhs) { return cmp(lhs,rhs) OP 0; }\
inline friend bool operator OP (const rational &lhs, const itype     rhs) { return cmp(lhs,rhs) OP 0; }\
inline friend bool operator OP (const itype     lhs, const rational &rhs) { return cmp(lhs,rhs) OP 0; }

#define Y_APQ_CMP_PARTIAL_ALL()  \
Y_APQ_CMP_PARTIAL(<)             \
Y_APQ_CMP_PARTIAL(>)             \
Y_APQ_CMP_PARTIAL(<=)            \
Y_APQ_CMP_PARTIAL(>=)

            Y_APQ_CMP_PARTIAL_ALL()

            static int compare(const rational &lhs,const rational &rhs) { return cmp(lhs,rhs); }

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

}

#endif
