//! \file

#ifndef Y_CHEMICAL_SEEKING_INCLUDED
#define Y_CHEMICAL_SEEKING_INCLUDED 1

#include "y/chemical/equilibrium.hpp"


namespace upsylon
{
    namespace Chemical
    {

        //______________________________________________________________________
        //
        //
        //! characteristic for seeking species
        //
        //______________________________________________________________________
        class Seeking : public Object
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef arc_ptr<const Seeking>             Pointer;  //!< alias
            typedef vector<Seeking::Pointer,Allocator> Array;    //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! setup s
            explicit Seeking(const Species &,
                             const iMatrix &,
                             const ENode   *);
            //! cleanup
            virtual ~Seeking() throw();


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //! display info
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const Seeking &s)
            {
                s.displayLHS(os) << " >= -" << s.sp;
                return os;
            }

            template <typename OSTREAM> inline
            OSTREAM & display(OSTREAM &os, const Accessible &C) const
            {
                displayLHS(os) << " >= -" << sp << " = " << -C[sp.indx];
                return os;
            }



            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Species            &sp; //!< species
            const accessible<unit_t> &nu; //!< corresponding in NuT
            const ENode * const       en; //!< first not-zero nu[en->indx]

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Seeking);

            template <typename OSTREAM> inline
            OSTREAM & displayLHS(OSTREAM &os) const
            {
                const Seeking &s    = *this;
                const ENode   *node = s.en; assert(node); assert(0!=s.nu[(***node).indx]);

                Disp(os,s.nu[(***node).indx],true) << (***node).name;
                for(node=node->next;node;node=node->next)
                {
                    const Equilibrium &eq = ***node;
                    const unit_t       nv = s.nu[eq.indx];
                    if(nv)
                    {
                        Disp(os,nv,false) << eq.name;
                    }
                }
                return os;
            }


            template <typename OSTREAM> static inline
            OSTREAM & Disp(OSTREAM &os, const unit_t nu, const bool first)
            {
                if(nu>0)
                {
                    if(!first)
                    {
                        os << '+';
                    }
                    if(nu>1)
                    {
                        os << vformat("%d*",int(nu));
                    }
                }
                else
                {
                    if(-1==nu)
                    {
                        os << '-';
                    }
                    else
                    {
                        os << vformat("%d*",int(nu));
                    }
                }
                return os;
            }
        };

    }

}

#endif
