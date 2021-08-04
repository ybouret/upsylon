//! \file

#ifndef Y_ALCHEMY_ACTORS_INCLUDED
#define Y_ALCHEMY_ACTORS_INCLUDED 1

#include "y/alchemy/actor.hpp"
#include "y/alchemy/extent.hpp"

#include "y/sequence/addressable.hpp"
#include "y/associative/hash/set.hpp"
#include "y/type/gateway.hpp"

namespace upsylon
{
    namespace Alchemy
    {
        
        //______________________________________________________________________
        //
        //
        //! collection of actors with same role
        //
        //______________________________________________________________________
        class Actors : public gateway<const Actor::Set>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char            CLID[];         //!< identifier
            typedef type::const_iterator const_iterator; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Actors();          //!< setup empty
            virtual ~Actors() throw();  //!< cleanup
            
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void           operator()(const Species &, const unsigned long); //!< register a new actor

            //! display with widths for names and coefficients
            template <typename OSTREAM> inline
            friend OSTREAM &  operator<<(OSTREAM &os, const Actors &A)
            {
                if(A.adb.size()>=1)
                {
                    const_iterator it = A.adb.begin();
                    os << *it;
                    while( ++it != A.adb.end() )
                    {
                        os << " + " << *it;
                    }
                }
                return os;
            }

            //! find maximal extent
            Extent findExtent(const Accessible &C) const throw();

            //! topology with sign
            template <typename T>
            void fill(addressable<T> &Nu, long s) const
            {
                if(s>=0)
                {
                    // products
                    for(const_iterator it=adb.begin();it!=adb.end();++it)
                    {
                        const Actor &a = *it;
                        Nu[a->indx] = static_cast<long>( a.nu );
                    }
                }
                else
                {
                    // reactants
                    for(const_iterator it=adb.begin();it!=adb.end();++it)
                    {
                        const Actor &a = *it;
                        Nu[a->indx] = -static_cast<long>( a.nu );
                    }
                }

            }

            //! guess who is active
            void guess(addressable<bool> &active) const throw();

            //! compute jacobian with precomputed K
            /**
             'target' and 'jscale' depends on reactant/product
             */
            double jacobian(double            target,
                            Addressable      &phi,
                            const double      jscale,
                            const Accessible &C) const throw();

            //! mass action law
            double massAction(double target, const Accessible &C) const throw();

            //! mass action after extent (careful with the sign of xi)
            double massAction(double target, const Accessible &C, const double xi) const throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Actors);
            virtual const_type & bulk() const throw();

            Actor::Set adb; //!< actors data base
        };

    }
    
}

#endif

