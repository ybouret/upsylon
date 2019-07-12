//! \file
#ifndef Y_MPL_SPRP_INCLUDED
#define Y_MPL_SPRP_INCLUDED 1

#include "y/mpl/natural.hpp"
#include "y/core/node.hpp"
#include "y/core/list.hpp"

namespace upsylon
{
    namespace mpl
    {

        //! Strong PRobable Prime
        class sprp : public number_type
        {
        public:
            //! internal node to store exponents
            class node_type : public core::inode<node_type>
            {
            public:
                typedef core::inode<node_type> __node_type; //!< alias
                const mpn q;                                //!< exponent value
                inline node_type( const mpn       &value ) : __node_type(), q(value)   {} //!< setup
                inline node_type( const node_type &other)  : __node_type(), q(other.q) {} //!< copy
                inline virtual ~node_type() throw() {} //!< destructor

            private:
                Y_DISABLE_ASSIGN(node_type);
            };

            typedef core::list_of_cpp<node_type> list_type; //!< alias


            const natural   n; //!< 2^s*d+1, must be odd
            const natural   m; //!< n-1,     even
            const natural   s; //!< exponent
            const list_type l; //!< list of d*2^[0..s-1]

            virtual ~sprp() throw();
            sprp( const natural &value ); //!< construct from natural
            sprp( const word_t   value ); //!< construct from integral
            sprp( const sprp    &other ); //!< hard copy

            //! output
            friend std::ostream & operator<<( std::ostream &, const sprp & );

            //! test if is a-SPRP
            bool operator()( const mpn &a ) const;

            //! Miller's test
            bool testPrime() const;

        private:
            Y_DISABLE_ASSIGN(sprp);
            void setup();
        };

    }


}

#endif

