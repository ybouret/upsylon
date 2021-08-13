

//! \file

#ifndef Y_ALCHEMY_LEADING_INCLUDED
#define Y_ALCHEMY_LEADING_INCLUDED 1

#include "y/alchemy/equilibria.hpp"
#include "y/alchemy/library.hpp"

namespace upsylon
{
    namespace Alchemy
    {
        //______________________________________________________________________
        //
        //
        //! Leading condition
        //
        //______________________________________________________________________
        class Leading
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef vector<const Leading,Allocator> Array;

            static const char EXT_[]; //!< prefix
            static const char LEQ_[]; //!< symbol
            static const char GEQ_[]; //!< symbol

            //! information
            enum Kind
            {
                LEQ, //!< <= -[something]
                GEQ  //!< >=  [something]
            };

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            Leading(const Equilibrium &,
                    const Species     &,
                    const size_t       ,
                    const Kind)           throw(); //!< setup
            ~Leading()                    throw(); //!< cleanup
            Leading(const Leading &other) throw(); //!< opy

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! formal output
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

            //! numeric output
            template <typename OSTREAM> inline
            OSTREAM & print(OSTREAM          &os,
                            const Library    &lib,
                            const Equilibria &eqs,
                            const Accessible &C) const
            {
                static const ios::scribe &writeC = ios::scribe::query( typeid(double) );
                if(nu>1) {
                    static const ios::scribe &writeNu = ios::scribe::query( typeid(nu) );
                    os << writeNu.write( &nu ) << '*';
                }
                eqs.print(os << EXT_,eq) << ' ';
                double value = C[sp.indx];
                switch(id)
                {
                    case LEQ: os << LEQ_; break;
                    case GEQ: os << GEQ_; value = - value; break;
                }
                lib.print(os,sp) << " = " << writeC.write(&value);
                return os;
            }



            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Equilibrium &eq; //!< equilibrium with indx
            const Species     &sp; //!< species     with indx
            const size_t       nu; //!< POSITIVE    coefficient
            const Kind         id; //!< to check

        private:
            Y_DISABLE_ASSIGN(Leading);
        };

        
    }

}

#endif
