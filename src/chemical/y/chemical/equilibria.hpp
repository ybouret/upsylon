
//! \file

#ifndef Y_CHEMICAL_EQUILIBRIA_INCLUDED
#define Y_CHEMICAL_EQUILIBRIA_INCLUDED 1

#include "y/chemical/equilibrium.hpp"
#include "y/lua++/state.hpp"

namespace upsylon
{
    namespace Chemical
    {
        
        //______________________________________________________________________
        //
        //
        //! collection of unique equilibria
        //
        //______________________________________________________________________
        class Equilibria : public Freezable, public gateway<const Equilibrium::Set>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char CLID[]; //!< "Chemical::Equilibria"
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Equilibria();            //!< setup empty
            virtual ~Equilibria() throw();    //!< cleanup
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            
            //! insert and update
            Equilibrium & use(Equilibrium *);
            
            //! display
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const Equilibria &eqs)
            {
                os << '{' << '\n';
                Strings      raw(eqs->size(),as_capacity);
                const size_t w = eqs.rawStrings(raw);
                for(const ENode *node=eqs->head();node;node=node->next)
                {
                    const Equilibrium &eq = ***node;
                    {
                        const string      &rs = raw[eq.indx];
                        os << ' ' << rs;
                        for(size_t i=rs.size();i<w;++i) os << ' ';
                    }
                    const string ks = eq.Kstr(eqs.tdisp);
                    os << " (" << ks << ")\n";
                }
                os << '}';
                return os;
            }
            
            //! create from name and constant description
            Equilibrium &create(const string &name,
                                const string &kstr,
                                Lua::VM      &vm);
            
            //! simple parsing
            Equilibrium & load(const string &info,
                               Library      &lib,
                               Lua::VM      &vm);
            
            //! simple parsing, wrapper
            Equilibrium & load(const char  *info,
                               Library      &lib,
                               Lua::VM      &vm);
            
            //! using db
            void operator()(const string &rx,
                            Library      &lib,
                            Lua::VM      &vm);
            
            void operator()(const char   *rx,
                            Library      &lib,
                            Lua::VM      &vm);
            
            
            //! verify all
            void verify(const unsigned flags) const;
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibria);
            virtual const_type& bulk() const throw();
            Equilibrium::Set    edb;
            
            size_t rawStrings(Strings &raw) const;
            
            
        public:
            mutable double      tdisp; //!< to display K(t) for eqs
        };
    }
}

#endif

