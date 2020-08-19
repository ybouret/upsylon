

//! \file
#ifndef Y_AP_INTEGER_INCLUDED
#define Y_AP_INTEGER_INCLUDED 1

#include "y/yap/natural.hpp"

namespace upsylon {



    namespace yap {

        //! integer type
        class integer : public number
        {
        public:
            static const char CLASS_NAME[];


            //C++
            integer(); //!< 0
            virtual ~integer() throw();
            integer(const integer &);
            integer(const itype    );
            integer(const sign_type, const natural &);
            integer & operator=(const integer &);
            integer & operator=(const itype    );

            integer(const sign_type, randomized::bits &, const size_t nbits);
            integer(randomized::bits &, const size_t nbits);

            // methods
            void xch(integer &) throw();

            // serializable
            virtual const char *className() const throw();
            virtual size_t      serialize(ios::ostream &) const;
            static  integer     read(ios::istream &, size_t &, const char *); //!< relaod

            // helpers
            static uint8_t   sign2byte(const sign_type) throw();
            
            // members
            const sign_type s;
            const natural   n;

            friend std::ostream & operator<<( std::ostream &, const integer &z);

            // comparisons
            
#define Y_APZ_DECL_NO_THROW(RETURN,CALL) \
static RETURN CALL(const integer &lhs, const integer &rhs) throw();\
static RETURN CALL(const integer &lhs, const itype    rhs) throw();\
static RETURN CALL(const itype    lhs, const integer &rhs) throw()
            
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

        private:
            void update() throw();
            Y_APZ_DECL_NO_THROW(bool,eq);
            Y_APZ_DECL_NO_THROW(bool,neq);
            Y_APZ_DECL_NO_THROW(int,cmp);



        };

    }

}


#endif
