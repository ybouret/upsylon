

//! \file

#ifndef Y_ALCHEMY_LEADING_INCLUDED
#define Y_ALCHEMY_LEADING_INCLUDED 1

#include "y/alchemy/equilibria.hpp"
#include "y/alchemy/library.hpp"

namespace upsylon
{
    namespace Alchemy
    {

        //! Leading condition
        class Leading
        {
        public:
            static const char EXT_[];
            static const char LEQ_[];
            static const char GEQ_[];

            enum Kind
            {
                LEQ,
                GEQ
            };

            Leading(const Equilibrium &,
                    const Species     &,
                    const size_t       ,
                    const Kind)  throw();
            ~Leading() throw();
            Leading(const Leading &other) throw();

            const Equilibrium &eq; //!< equilibrium with indx
            const Species     &sp; //!< species     with indx
            const size_t       nu; //!< POSITIVE    coefficient
            const Kind         id; //!< to check

            template <typename OSTREAM> inline
            OSTREAM & print(OSTREAM          &os,
                            const Library    &lib,
                            const Equilibria &eqs) const
            {
                if(nu>1) {
                    static const ios::scribe &writeNu = ios::scribe::query( typeid(nu) );
                    os << writeNu.write( &nu ) << '*';
                }
                eqs.print(os << EXT_,eq) << ' ';
                switch(id)
                {
                    case LEQ: os << LEQ_; break;
                    case GEQ: os << GEQ_; break;
                }
                lib.print(os,sp);
                return os;
            }
            

        private:
            Y_DISABLE_ASSIGN(Leading);
        };

        
    }

}

#endif
