//! \file

#ifndef Y_AP_INTEGER_INCLUDED
#define Y_AP_INTEGER_INCLUDED 1

#include "y/yap/natural.hpp"

namespace upsylon {



    namespace yap {

        //______________________________________________________________________
        //
        //
        //! drop in replacement for integer type
        //
        //______________________________________________________________________
        class integer : public number
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char    CLASS_NAME[]; //!< "apz"
            static const unsigned __n = 0x01;  //!< negative marker
            static const unsigned __z = 0x02;  //!< zero     marker
            static const unsigned __p = 0x04;  //!< positive marker


            //__________________________________________________________________
            //
            // C++ and setup
            //__________________________________________________________________
            virtual ~integer() throw();                //!< cleanup
            integer();                                 //!< initialize to zero
            integer(const integer &);                  //!< copy
            integer(const itype    );                  //!< build from integral type
            integer(const sign_type, const natural &); //!< build from natural
            integer(const sign_type, const utype    ); //!< build from unsigned
            integer(const natural &);                  //!< build from unsigned
            integer & operator=(const integer &);      //!< assign integer
            integer & operator=(const itype    );      //!< assign integral

            integer(const sign_type, randomized::bits &, const size_t nbits); //!< specific random
            integer(randomized::bits &, const size_t nbits);                  //!< random with random sign

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void xch(integer &) throw(); //!< no-throw exchange

            //__________________________________________________________________
            //
            // serializable
            //__________________________________________________________________
            virtual const char *className() const throw();                    //!< CLASS_NAME
            virtual size_t      serialize(ios::ostream &) const;              //!< output
            static  integer     read(ios::istream &, size_t &, const char *); //!< reload

            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________
            static uint8_t   sign2byte(const sign_type) throw(); //!< encoding signs

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const sign_type s; //!< sign
            const natural   n; //!< absolute value

            //! display
            friend std::ostream & operator<<(std::ostream &, const integer &);

            //__________________________________________________________________
            //
            // comparisons
            //__________________________________________________________________

            //! complete API
#define Y_APZ_DECL_NO_THROW(RETURN,CALL) \
static RETURN CALL(const integer &lhs, const integer &rhs) throw();\
static RETURN CALL(const integer &lhs, const itype    rhs) throw();\
static RETURN CALL(const itype    lhs, const integer &rhs) throw()

            //! full comparison API
#define Y_APZ_CMP_FULL(OP,CALL) \
inline friend bool operator OP (const integer &lhs, const integer &rhs) throw() { return CALL(lhs,rhs); }\
inline friend bool operator OP (const integer &lhs, const itype    rhs) throw() { return CALL(lhs,rhs); }\
inline friend bool operator OP (const itype    lhs, const integer &rhs) throw() { return CALL(lhs,rhs); }
  
            Y_APZ_CMP_FULL(==,eq)
            Y_APZ_CMP_FULL(!=,neq)

            
            //! build partial comparators
#define Y_APZ_WRAP_CMP_PART(OP,CALL)\
inline friend bool operator OP (const integer &lhs, const integer &rhs) throw() { return CALL(lhs,rhs) OP 0; }\
inline friend bool operator OP (const itype    lhs, const integer &rhs) throw() { return CALL(lhs,rhs) OP 0; }\
inline friend bool operator OP (const integer &lhs, const itype    rhs) throw() { return CALL(lhs,rhs) OP 0; }

            //! build all partial comparators
#define Y_APZ_WRAP_CMP_PART_ALL() \
Y_APZ_WRAP_CMP_PART(<,cmp)        \
Y_APZ_WRAP_CMP_PART(<=,cmp)       \
Y_APZ_WRAP_CMP_PART(>,cmp)        \
Y_APZ_WRAP_CMP_PART(>=,cmp)

            Y_APZ_WRAP_CMP_PART_ALL()

            //! for different sorting algorithms
            static inline int compare(const integer &lhs, const integer &rhs) throw() { return cmp(lhs,rhs); }

            //__________________________________________________________________
            //
            // addition
            //__________________________________________________________________
            //! function declaration
#define Y_APZ_DECL(CALL)                                    \
static integer CALL(const integer &lhs, const integer &rhs);\
static integer CALL(const integer &lhs, const itype    rhs);\
static integer CALL(const itype    lhs, const integer &rhs)

            //! operators implementation
#define Y_APZ_WRAP(OP,CALL)                                                                                          \
inline friend integer operator OP (const integer &lhs, const integer &rhs) { return CALL(lhs,rhs); }                 \
inline friend integer operator OP (const integer &lhs, const itype    rhs) { return CALL(lhs,rhs); }                 \
inline friend integer operator OP (const itype    lhs, const integer &rhs) { return CALL(lhs,rhs); }                 \
inline integer &      operator OP##= (const integer &rhs) { integer tmp = CALL(*this,rhs); xch(tmp); return *this; } \
inline integer &      operator OP##= (const itype    rhs) { integer tmp = CALL(*this,rhs); xch(tmp); return *this; }

            Y_APZ_DECL(add);             //!< aliases
            Y_APZ_WRAP(+,add)
            integer   operator+() const; //!< unary '+'
            integer & operator++();      //!< prefix  ++ operator
            integer   operator++(int);   //!< postfix ++ operator

            //__________________________________________________________________
            //
            // subtraction
            //__________________________________________________________________
            Y_APZ_DECL(sub);             //!< aliases
            Y_APZ_WRAP(-,sub)
            integer   operator-() const; //!< unary '-'
            integer & operator--();      //!< prefix  -- operator
            integer   operator--(int);   //!< postfix -- operator

            //__________________________________________________________________
            //
            // multiplication
            //__________________________________________________________________
            Y_APZ_DECL(mul);           //!< aliases
            Y_APZ_WRAP(*,mul)
            integer mul_by(const natural &rhs) const;

            //__________________________________________________________________
            //
            // division
            //__________________________________________________________________
            Y_APZ_DECL(divide);           //!< aliases
            Y_APZ_WRAP(/,divide)


        private:
            void update() throw();
            Y_APZ_DECL_NO_THROW(bool,eq);
            Y_APZ_DECL_NO_THROW(bool,neq);
            Y_APZ_DECL_NO_THROW(int,cmp);
        };

    }

}


#endif
