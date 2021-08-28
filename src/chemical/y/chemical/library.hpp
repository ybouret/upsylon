
//! \file

#ifndef Y_CHEMICAL_LIBRARY_INCLUDED
#define Y_CHEMICAL_LIBRARY_INCLUDED 1

#include "y/chemical/species.hpp"
#include "y/chemical/freezable.hpp"
#include "y/type/gateway.hpp"
#include "y/jive/pattern.hpp"

namespace upsylon
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! information about a (unique) species
        //
        //______________________________________________________________________
        class Library : public Freezable, public gateway<const Species::Set>
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const char CLID[]; //!< "Chemical::Library"
            
            explicit Library();         //!< setup empty
            virtual ~Library() throw(); //!< cleanup
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            
            //! declare a new species, allows multiple SAME
            template <typename NAME> inline
            const Species & operator()(const NAME   &name,
                                       const unit_t  charge)
            {
                return use( new Species(name,charge) );
            }
            
            //! declare new species by parsing
            template <typename RX> inline
            const Species & operator()(const RX &rx)
            {
                return use( Jive::Module::OpenData(rx) );
            }
            
            //! output
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const Library &lib)
            {
                os << '{' << '\n';
                for(const Species::Node *node=lib->head();node;node=node->next)
                {
                    const Species &sp = ***node;
                    os << sp;
                    os << vformat(" : @%u", unsigned(sp.indx));
                    os << vformat(" : z =%3d", int(sp.charge) );
                    os << vformat(" : r =%3u", unsigned(sp.rating));
                    os << ' ' << '(' << Species::Status(sp.rating) << ')';
                    os << '\n';
                }
                os << '}' << '\n';
                return os;
            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Library);
            virtual const_type &bulk() const throw();
            Species::Set       sdb;
            const Jive::Motif  jN;
            const Jive::Motif  jZ;
            
            const Species & use(Species *);
            void            update() throw();
            const Species & use(Jive::Module *);
            
        };
    }
}

#endif

