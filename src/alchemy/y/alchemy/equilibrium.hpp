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
            string format() const;

            
            //! compute K0*reac-prod
            double compute(const double             K0,
                           const accessible<double> &C) const throw();

            //! solve independently
            void   solve(addressable<double> &Cini,
                         const double         K0,
                         addressable<double> &Ctry) const throw();
           
            template <typename OSTREAM> inline
            friend OSTREAM &operator<<(OSTREAM &os, const equilibrium &eq)
            {
                const string fmt = eq.format();
                os << fmt << vformat("| (%.15g)", eq.K(eq.tdisp) );
                return os;
            }
            
            //! display compact format
            template <typename OSTREAM>
            OSTREAM &display_code(OSTREAM &os) const
            {
                os << '<' << name << '>';
                for(size_t i=name.size();i<width;++i) os << ' ';
                os << ' ';
                reac.display_code(os);
                prod.display_code(os << "->" );
                os << vformat(" (%.15g)", K(0));
                return os;
            }
           
            //! fill coefficients row
            void fill( addressable<long> &Nu ) const throw();
            
            
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string   name;  //!< identifier
            const actors   reac;  //!< reactants
            const actors   prod;  //!< products
            const size_t   width; //!< for name
            mutable double tdisp; //!< display time
            
        protected:
            //! initialize
            template <typename ID> inline
            explicit equilibrium(const ID &id) :
            name(id), reac(), prod(), width(0), tdisp(0)
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

