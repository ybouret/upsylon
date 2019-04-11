//! \file
#ifndef Y_MPL_PFQ_INCLUDED
#define Y_MPL_PFQ_INCLUDED

#include "y/mpl/pfd.hpp"

namespace upsylon
{
    namespace mpl
    {
        //! prime factors quotient
        class pfq : public counted_object
        {
        public:
            const pfd n; //!< numerator
            const pfd d; //!< denominator

            virtual ~pfq() throw();                   //!< destructor
            pfq();                                    //!< 0/1
            pfq(const pfq &other);                    //!< copy
            pfq( const pfd     &N, const pfd     &D); //!< N/D
            pfq( const natural &N, const natural &D); //!< N/D
            pfq( const word_t   N, const word_t  D);  //!< N/D
            pfq( const pfd     &N);                   //!< N/1
            pfq( const natural &N);                   //!< N/1
            pfq( const word_t   N);                   //!< N/1
            void mul_by( const pfd &other );          //!< *= other
            void div_by( const pfd &other );          //!< /= other
            void mul_by( const pfq &other );          //!< *= other
            void div_by( const pfq &other );          //!< /= other

            //! output
            friend std::ostream & operator<<( std::ostream &, const pfq &);
            
            static pfd A(const word_t N, const word_t K); //!< arrangements N!/(N-K)!
            static pfd C(const word_t N, const word_t K); //!< combinations N!/(N-K)!/K!


        private:
            Y_DISABLE_ASSIGN(pfq);
            void update();
        };
    }

}


#endif

