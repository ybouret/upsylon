

//! \file

#ifndef Y_CHEMICAL_ACTORS_INCLUDED
#define Y_CHEMICAL_ACTORS_INCLUDED 1

#include "y/chemical/actor.hpp"
#include "y/type/gateway.hpp"

namespace upsylon
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! database of unique actors
        //
        //______________________________________________________________________
        class Actors : public gateway<const Actor::Map>
        {
        public:
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
            //! register and increase species rating
            void operator()(const size_t, const Species &sp);
            
            //! find
            bool owns(const Species &sp) const throw();
            
            //! display
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const Actors &A)
            {
                const ANode *node = A->head();
                if(node)
                {
                    os << **node;
                    for(node=node->next;node;node=node->next)
                    {
                        os << '+' << **node;
                    }
                }
                return os;
            }
            
            //! signed total charge
            unit_t totalCharge() const throw();
            
            //! load coefficients, used to check minimal equilibrium
            template <typename T> inline
            void save(sequence<T> &coeff) const
            {
                for(const ANode *node = adb.head();node;node=node->next)
                {
                    typename sequence<T>::const_type temp( (**node).nu );
                    coeff.push_back(temp);
                }
            }
            
            size_t tellLeading() const throw();                //!< count leading here
            void   fillLeading(sequence<const Actor> &) const; //!< append leading

            //! Prod_j C[j]^nu[j]
            double massAction(double res, const Accessible &C) const throw();

            //!
            void massActionJ(const double ini,Addressable &J, const Accessible &C) const throw();



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Actors);
            virtual const_type &bulk() const throw();
            Actor::Map adb;
        };
    }
    
}

#endif

