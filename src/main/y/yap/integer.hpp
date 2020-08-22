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
            // macros to overload calls
            //__________________________________________________________________
            //! z,z
#define Y_APZ_ARGS_Z_Z (const integer &lhs, const integer &rhs)
            //! z,itype
#define Y_APZ_ARGS_Z_I (const integer &lhs, const itype    rhs)
            //! itype,z
#define Y_APZ_ARGS_I_Z (const itype    lhs, const integer &rhs)

            //! wrapper to declare no-throw methods
#define Y_APZ_DECL_SAFE_METHOD(RETURN,CALL) \
static RETURN CALL Y_APZ_ARGS_Z_Z throw();  \
static RETURN CALL Y_APZ_ARGS_Z_I throw();  \
static RETURN CALL Y_APZ_ARGS_I_Z throw()

            //! method implementation helper
#define Y_APZ_IMPL_METHOD_(PREFIX,ARGS,SUFFIX) PREFIX ARGS SUFFIX

            //! implement overloaded methods
#define Y_APZ_IMPL_METHODS(PREFIX,SUFFIX)        \
Y_APZ_IMPL_METHOD_(PREFIX,Y_APZ_ARGS_Z_Z,SUFFIX) \
Y_APZ_IMPL_METHOD_(PREFIX,Y_APZ_ARGS_Z_I,SUFFIX) \
Y_APZ_IMPL_METHOD_(PREFIX,Y_APZ_ARGS_I_Z,SUFFIX)

            //__________________________________________________________________
            //
            // total comparisons
            //__________________________________________________________________

            //! implement one total comparison
#define Y_APZ_IMPL_COMPARE_TOTAL_(OP,CALL) Y_APZ_IMPL_METHODS(inline friend bool operator OP,throw() { return CALL(lhs,rhs);  })

            //! implement all total comparions
#define Y_APZ_IMPL_COMPARE_TOTAL() \
Y_APZ_IMPL_COMPARE_TOTAL_(==,eq)   \
Y_APZ_IMPL_COMPARE_TOTAL_(!=,neq)

            //! implement one partial comparison
#define Y_APZ_IMPL_COMPARE_PARTIAL_(OP) Y_APZ_IMPL_METHODS(inline friend bool operator OP, throw() { return cmp(lhs,rhs) OP 0; })

            //! implement all partial comparions
#define Y_APZ_IMPL_COMPARE_PARTIAL() \
Y_APZ_IMPL_COMPARE_PARTIAL_(<)       \
Y_APZ_IMPL_COMPARE_PARTIAL_(>)       \
Y_APZ_IMPL_COMPARE_PARTIAL_(<=)      \
Y_APZ_IMPL_COMPARE_PARTIAL_(>=)

            Y_APZ_IMPL_COMPARE_TOTAL()
            Y_APZ_IMPL_COMPARE_PARTIAL()

            //! for different sorting algorithms
            static inline int compare(const integer &lhs, const integer &rhs) throw() { return cmp(lhs,rhs); }

            //__________________________________________________________________
            //
            // addition
            //__________________________________________________________________
            //! function declaration
#define Y_APZ_OVERLOAD(CALL)        \
static integer CALL Y_APZ_ARGS_Z_Z; \
static integer CALL Y_APZ_ARGS_Z_I; \
static integer CALL Y_APZ_ARGS_I_Z

            //! operator implementation
#define Y_APZ_OPERATOR(OP,CALL) \
Y_APZ_IMPL_METHODS(inline friend integer operator OP,{ return CALL(lhs,rhs); }) \
inline integer &      operator OP##= (const integer &rhs) { integer tmp = CALL(*this,rhs); xch(tmp); return *this; } \
inline integer &      operator OP##= (const itype    rhs) { integer tmp = CALL(*this,rhs); xch(tmp); return *this; }

            //! method+operator at once
#define Y_APZ_STANDARD(OP,CALL) \
Y_APZ_OVERLOAD(CALL);           \
Y_APZ_OPERATOR(OP,CALL);



            Y_APZ_STANDARD(+,add)
            integer   operator+() const; //!< unary '+'
            integer & operator++();      //!< prefix  ++ operator
            integer   operator++(int);   //!< postfix ++ operator

            //__________________________________________________________________
            //
            // subtraction
            //__________________________________________________________________
            Y_APZ_STANDARD(-,sub)
            integer   operator-() const; //!< unary '-'
            integer & operator--();      //!< prefix  -- operator
            integer   operator--(int);   //!< postfix -- operator

            //__________________________________________________________________
            //
            // multiplication
            //__________________________________________________________________
            Y_APZ_STANDARD(*,mul)
            integer mul_by(const natural &rhs) const; //!< optimized mul_by, for rational
            static integer square_of(const integer &x); //!< x*x
            static integer abs_of(const integer &x);    //!< |x|
            static integer sqrt_of(const integer &x);   //!< sqrt(x)
            
            //__________________________________________________________________
            //
            // division
            //__________________________________________________________________
            Y_APZ_STANDARD(/,divide)

            //__________________________________________________________________
            //
            // conversion
            //__________________________________________________________________
            string to_dec() const; //!< decimal string
                                   //! convert to an integral type
            template <typename T> inline
            bool to( T &target ) const throw()
            {
                static const T     tmin = limit_of<T>::minimum;
                static const T     tmax = limit_of<T>::maximum; assert(tmax>0);
                static const itype vmax(tmax);
                static const itype vmin(tmin);
                if( (*this<vmin) || (*this>vmax) )
                {
                    return false;
                }
                else
                {
                    switch(s)
                    {
                        case __negative: target = T( -itype(n.lsw())); break;
                        case __zero    : target = 0;                   break;
                        case __positive: target = T( itype(n.lsw()) ); break;
                    }
                    return true;
                }
            }
            
            //! wrapper
            template <typename T>
            inline T cast_to(const char *when = 0) const
            {
                T ans(0);
                if(!to<T>(ans))
                {
                    cast_overflow(when);
                }
                return ans;
            }
            
        private:
            void update() throw();
            static void cast_overflow(const char *which);
            
            Y_APZ_DECL_SAFE_METHOD(bool,eq);
            Y_APZ_DECL_SAFE_METHOD(bool,neq);
            Y_APZ_DECL_SAFE_METHOD(int,cmp);
        };

    }

    typedef yap::integer apz; //!< nickname

    namespace mkl
    {
        inline apz fabs_of(const apz &x) { return apz::abs_of(x);  } //!< for use in mkl
        inline apz sqrt_of(const apz &x) { return apz::sqrt_of(x); } //!< for use in mkl
    }


}


#endif
