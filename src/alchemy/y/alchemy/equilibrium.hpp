//! \file

#ifndef Y_ALCHEMY_EQUILIBRIUM_INCLUDED
#define Y_ALCHEMY_EQUILIBRIUM_INCLUDED 1

#include "y/alchemy/actors.hpp"
#include "y/associative/hash/set.hpp"
#include "y/sequence/addressable.hpp"

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
        class equilibrium : public object, public counted, public compilable
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef intr_ptr<string,equilibrium>       pointer; //!< alias
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~equilibrium() throw();        //!< cleanup
            virtual  double K(double) const = 0;   //!< constant at given time
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void operator()(const long nu, const species &sp); //!< register
            const string & key() const throw();                //!< for hash_set

            //! create a formatted output, w/0 constant
            string format(const size_t name_width=0,
                          const size_t spec_width=0,
                          const size_t reac_width=0,
                          const size_t prod_width=0) const;

            
            //! compute K0*reac-prod
            double compute(const double             K0,
                           const accessible<double> &C) const throw();

            //! solve independently
            void   solve(addressable<double> &Cini,
                         const double         K0,
                         addressable<double> &Ctry) const throw();

            //! display compact format
            template <typename OSTREAM> inline
            OSTREAM &display_code(OSTREAM &os, const size_t name_width=0) const
            {
                os << '<' << name << '>';
                for(size_t i=name.size();i<name_width;++i) os << ' ';
                os << ' ';
                reac.display_code(os);
                prod.display_code(os << "->" );
                os << vformat(" (%.15g)", K(0));
                return os;
            }
           
            //! fill coefficients row
            void fill( addressable<long> &Nu ) const throw();
            
            //! limiting reactant
            size_t forward(double &xi, const accessible<double> &C) const throw();
            
            
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string name; //!< identifier
            const actors reac; //!< reactants
            const actors prod; //!< products
            
        protected:
            //! initialize
            template <typename ID> inline
            explicit equilibrium(const ID &id) :
            name(id), reac(), prod()
            {
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(equilibrium);
            virtual void on_compile();
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
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~constant_equilibrium() throw(); //!< cleanup

            //! initialize
            template <typename ID> inline
            explicit constant_equilibrium(const ID &id, const double k) :
            equilibrium(id), Kvalue(k) {}

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            virtual double K(double) const; //!< return constant value
            
        private:
            const double Kvalue;
            Y_DISABLE_COPY_AND_ASSIGN(constant_equilibrium);
        };
        
    }
    
}

#endif

