//! \file

#ifndef Y_CHEMICAL_SEEKING_INCLUDED
#define Y_CHEMICAL_SEEKING_INCLUDED 1

#include "y/chemical/equilibrium.hpp"


namespace upsylon
{
    namespace Chemical
    {

        class Seeking : public Object
        {
        public:
            typedef arc_ptr<const Seeking>             Pointer;  //!< alias
            typedef vector<Seeking::Pointer,Allocator> Array;    //!< alias
            
            explicit Seeking(const Species &,
                             const iMatrix &,
                             const ENode   *);
            virtual ~Seeking() throw();

            const Species            &sp;
            const accessible<unit_t> &nu;
            const ENode * const       en;

            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const Seeking &s)
            {
                const ENode *node = s.en; assert(node); assert(0!=s.nu[(***node).indx]);

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

                os << " >= -" << s.sp;
                return os;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Seeking);

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
