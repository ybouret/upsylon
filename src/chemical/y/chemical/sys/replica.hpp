
//! \file
#ifndef Y_CHEMICAL_REPLICA_INCLUDED
#define Y_CHEMICAL_REPLICA_INCLUDED 1

#include "y/chemical/equilibria.hpp"

namespace upsylon
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! primary constraint definition per equilibrium
        //
        //______________________________________________________________________
        class Replica : public Object, public authority<const Species>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef arc_ptr<const Replica>    Pointer; //!< alias
            typedef vector<Pointer,Allocator> Array;   //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Replica(const Species     &,
                             const iAccessible &,
                             const ENode       *) throw(); //!< setup
            virtual ~Replica() throw();                    //!< cleanu


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const iAccessible  &nu; //!< NuT[sp.index]
            const ENode * const en; //!< first node with not zero coef

            //__________________________________________________________________
            //
            // display helpers
            //__________________________________________________________________

            //! formal display
            template <typename OSTREAM> inline
            void display(OSTREAM     &os,
                         const size_t indent) const
            {
                prolog(os,indent) << '\n';
            }

            //! formal display
            template <typename OSTREAM> inline
            void display(OSTREAM          &os,
                         const Accessible &C,
                         const size_t      indent) const
            {
                static const ios::scribe & _ = ios::scribe::query<Accessible::type>();
                prolog(os,indent);
                const string value = _.write( &C[ (**this).indx] );
                os << " = " << value;
                os << '\n';
            }
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Replica);


            template <typename OSTREAM> inline
            OSTREAM & prolog(OSTREAM     &os,
                             const size_t indent) const
            {
                Library::Indent(os,indent);

                displayFirstNu(os,nu[(***en).indx]) << '@' << ***en;
                for(const ENode *node=en->next;node;node=node->next)
                {
                    const Equilibrium &mine = ***node;
                    const unit_t       coef = nu[mine.indx];
                    if(coef)
                    {
                        displayOtherNu(os << ' ',coef) << '@' << mine;
                    }
                }
                os << " >= -" << **this;
                return os;
            }

            template <typename OSTREAM> static inline
            OSTREAM & displayFirstNu(OSTREAM &os, const unit_t nu)
            {
                assert(nu);
                if(nu>0)
                {
                    if(nu>1)
                    {
                        os << vformat("%d ", int(nu));
                    }
                    else
                    {
                        // do nothing
                    }
                }
                else
                    displayNegativeNu(os,nu);
                return os;
            }

            template <typename OSTREAM> static inline
            OSTREAM &displayOtherNu(OSTREAM &os, const unit_t nu)
            {
                if(nu>0)
                {
                    if(nu>1)
                    {
                        os << vformat("+%d", int(nu));
                    }
                    else
                    {
                        os << '+';
                    }
                }
                else
                    displayNegativeNu(os,nu);
                return os;
            }

            template <typename OSTREAM> static inline
            void displayNegativeNu(OSTREAM &os, const unit_t nu)
            {
                if(nu < -1)
                {
                    os << vformat("%d", int(nu));
                }
                else
                {
                    os << '-';
                }
            }

        };

    }

}

#endif
