//! \file

#ifndef Y_AQUA_BOOT_INCLUDED
#define Y_AQUA_BOOT_INCLUDED 1

#include "y/aqua/boot/constraint.hpp"
#include "y/container/matrix.hpp"
#include "y/core/temporary-acquire.hpp"

namespace upsylon {

    namespace Aqua
    {
        class Library;    //!< forward declaration
        class Equilibria; //!< forward declaration
        class Engine; //!< forward declaration
        
        //______________________________________________________________________
        //
        //
        //! booting from constraints
        //
        //______________________________________________________________________
        class Boot : public Constraint::List
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Boot() throw(); //!< cleanup

            //! setup with an identifier
            template <typename ID> inline
            explicit Boot(const ID &id)  :
            Constraint::List(),
            name(id),
            R(),
            pL(),
            dL(pL.r_aux1),
            S(),
            pS(),
            dS(pS.r_aux1),
            keep()
            {

            }

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! create a new contraint
            Constraint & operator()( const double value );

            //! add from library
            void electroneutrality(const Library &);

            //! a=C0
            void conserve(const double, const Species &a);

            //! a+b=C0
            void conserve(const double, const Species &a, const Species &b);

            //! a+b+c=C0
            void conserve(const double, const Species &a, const Species &b, const Species &c);

            //! a+b+c+d=C0
            void conserve(const double, const Species &a, const Species &b, const Species &c, const Species &d);


            void  fill(addressable<double> &) const throw(); //!< fill a vector of Nc constraints
            void  quit() throw();                            //!< reset
            void  init(Library &,const Equilibria &eqs);     //!< buildIndices for library and build matrices
            void  find(addressable<double> &, Engine & );    //!< solve


            //! display
            friend std::ostream & operator<<(std::ostream &, const Boot &);

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const string        name; //!< identifier
            const iMatrix       R;    //!< constraint matrix     [NcxM]
            const iMatrix       pL;   //!< constraints to C      [MxNc]
            iAddressable       &dL;   //!< scaling of pL
            const iMatrix       S;    //!< supplementary matrix  [N*M]
            const iMatrix       pS;   //!< projection on S times dS: s* S'*inv(S*S')*S  [M*M]
            iAddressable       &dS;   //!< scaling of pS         [N]

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Boot);
            core::temporary_acquire<4> keep;
            void setup() throw();
        };

    }

}

#endif
