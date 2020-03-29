//! \file
#ifndef Y_MPL_PRIME_FACTOR_INCLUDED
#define Y_MPL_PRIME_FACTOR_INCLUDED 1

#include "y/mpl/natural.hpp"

#include "y/memory/pooled.hpp"
#include "y/associative/set.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon
{
    namespace mpl
    {

        //! a prime factor p^n
        class prime_factor : public number_type
        {
        public:
            //------------------------------------------------------------------
            //
            // types
            //
            //------------------------------------------------------------------
            typedef intr_ptr<const natural,prime_factor>                     pointer; //!< shared pointer
            typedef memory::pooled                                           memType; //!< medium sized factors
            typedef hashing::fnv                                             hfnType; //!< hashing
            typedef key_hasher<const natural,hfnType>                        pkhType; //!< prime key hasher
            typedef set<const natural,prime_factor::pointer,pkhType,memType> db;      //!< database


            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            prime_factor( const natural &prm, const size_t num ); //!< setup
            prime_factor( const prime_factor &);                  //!< copy
            virtual ~prime_factor() throw();                      //!< cleanup
            const natural & key() const throw();                  //!< for database
            natural         value() const;                        //!< compute product

            //! output
            friend std::ostream & operator<<( std::ostream &os, const prime_factor & );

            //! comparing keys to keep databases ordered
            static int compare_keys( const natural &lhs, const natural &rhs ) throw();

            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const natural p; //!< prime
            size_t        n; //!< count > 0

            //! test equality
            friend bool operator==( const prime_factor &lhs, const prime_factor &rhs ) throw();
            //! test difference
            friend bool operator!=( const prime_factor &lhs, const prime_factor &rhs ) throw();

            //__________________________________________________________________
            //
            // Serialize
            //__________________________________________________________________
            virtual const char  *className() const throw();               //!< CLASS_NAME
            virtual size_t       serialize( ios::ostream &fp ) const;     //!< num and den
            static const char    CLASS_NAME[];                            //!< "prmf"
            static  prime_factor read( ios::istream &fp, size_t &shift, const char *which); //!< read prime factor

        private:
            Y_DISABLE_ASSIGN(prime_factor);
        };


       

    }
}


#endif
