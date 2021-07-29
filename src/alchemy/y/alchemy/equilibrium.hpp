//! \file

#ifndef Y_ALCHEMY_EQUILIBRIUM_INCLUDED
#define Y_ALCHEMY_EQUILIBRIUM_INCLUDED 1

#include "y/alchemy/actors.hpp"
#include "y/associative/hash/set.hpp"

namespace upsylon
{
    namespace alchemy
    {
        //______________________________________________________________________
        //
        //
        //! base class for equilibrium
        //
        //______________________________________________________________________
        class equilibrium : public object, public counted
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef intr_ptr<string,equilibrium> pointer;
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~equilibrium() throw();
            virtual  double K(double) const = 0;
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void operator()(const long nu, const species &sp);
            
            const string & key() const throw();
            std::ostream & display(std::ostream  &,
                                   const library &,
                                   const double  t,
                                   const size_t  eqw=0) const;
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name; //!< identifier
            const actors reac; //!< reactants
            const actors prod; //!< products
            
        protected:
            template <typename ID> inline
            explicit equilibrium(const ID &id) :
            name(id), reac(), prod()
            {
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(equilibrium);
        };
        
        //______________________________________________________________________
        //
        //
        //! a truly constant equilibrium
        //
        //______________________________________________________________________
        class constant_equilibrium : public equilibrium
        {
        public:
            virtual ~constant_equilibrium() throw();
            template <typename ID> inline
            explicit constant_equilibrium(const ID &id, const double k) :
            equilibrium(id), Kvalue(k)
            {
            }
            
            virtual double K(double) const;
            
        private:
            const double Kvalue;
            Y_DISABLE_COPY_AND_ASSIGN(constant_equilibrium);
        };
        
    }
    
}

#endif

