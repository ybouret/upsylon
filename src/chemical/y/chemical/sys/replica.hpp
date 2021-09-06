
//! \file
#ifndef Y_CHEMICAL_REPLICA_INCLUDED
#define Y_CHEMICAL_REPLICA_INCLUDED 1

#include "y/chemical/equilibria.hpp"
#include "y/type/authority.hpp"

namespace upsylon
{
    namespace Chemical
    {
        
        class Replica : public Object, public authority<const Species>
        {
        public:
            typedef arc_ptr<const Replica>    Pointer;
            typedef vector<Pointer,Allocator> Array;
            
            virtual ~Replica() throw();
            explicit Replica(const Species    &,
                             const Accessible &,
                             const ENode      *) throw();
            
            const Accessible    &nu; //!< vector of NuT
            const ENode * const  en; //!< corresponding first not nul nu[en->indx]
            
            template <typename OSTREAM> inline
            void display(OSTREAM &os, const size_t indent) const
            {
                displayProlog(os,indent);
                os << '\n';
            }
            
            template <typename OSTREAM> inline
            void display(OSTREAM &os, const Accessible &C, const size_t indent) const
            {
                displayProlog(os,indent);
                os << " = " << vformat("%.15g", -C[(**this).indx]);
                os << '\n';
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Replica);
            
            template <typename OSTREAM> inline
            void displayProlog(OSTREAM &os, const size_t indent) const
            {
                const ENode *node=en;
                displayHead(Library::Indent(os,indent),node);
                for(node=node->next;node;node=node->next)
                {
                    displayNext(os,node);
                }
                os << " >= -" << **this;
            }
            
            template <typename OSTREAM> inline
            void displayHead(OSTREAM &os, const ENode *node) const
            {
                const Equilibrium &eq   = ***node;
                const double       coef = nu[eq.indx];
                if(coef<0)
                {
                    if(coef<-1)
                        os << vformat("%g",coef);
                    else
                        os << '-';
                }
                else
                {
                    if(coef>1)
                        os << vformat("%g",coef);
                }
                os << eq.name;
            }
            
            template <typename OSTREAM> inline
            void displayNext(OSTREAM &os, const ENode *node) const
            {
                const Equilibrium &eq   = ***node;
                const double       coef = nu[eq.indx];
                if(coef<0)
                {
                    if(coef<-1)
                        os << vformat("%g",coef);
                    else
                        os << '-';
                }
                else
                {
                    if(coef>1)
                        os << vformat("+%g",coef);
                    else
                        os << '+';
                }
                os << eq.name;
            }
        };
        
    }
    
}

#endif
