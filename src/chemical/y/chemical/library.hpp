
//! \file

#ifndef Y_CHEMICAL_LIBRARY_INCLUDED
#define Y_CHEMICAL_LIBRARY_INCLUDED 1

#include "y/chemical/species.hpp"
#include "y/chemical/freezable.hpp"
#include "y/type/gateway.hpp"
#include "y/jive/pattern.hpp"
#include "y/ios/scribe.hpp"

namespace upsylon
{
    namespace randomized
    {
        class bits;
    }
    
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
            static const char        CLID[];         //!< "Chemical::Library"
            static const int         CMinPow = -14;  //!< for randomC
            static const int         CMaxPow =   1;  //!< for randomC
            
            //! a radom concentration
            static double     RandomC(randomized::bits &) throw();
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
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
            
            //! declare one new species by parsing
            template <typename RX> inline
            const Species & operator()(const RX &rx)
            {
                Jive::Source source(Jive::Module::OpenData(rx) );
                return use(source);
            }
            
            //! helper for equilibria
            unit_t get(Jive::Source &source, const Species **pps);

            //! load multiple ':' separated species
            void load(const string &multipleSpecies);

            //! load multiple ':' separated species, wrapper
            void load(const char *multipleSpecies);

            //! load one or multiple species
            template <typename RX> inline
            Library & operator<<( const RX &rx )
            {
                Library &self = *this;
                load(rx);
                return self;
            }


            //! output
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const Library &lib)
            {
                os << '{' << '\n';
                for(const SNode *node=lib->head();node;node=node->next)
                {
                    const Species &sp = ***node;
                    os << ' ' << sp;
                    os << vformat(" : @%2u", unsigned(sp.indx));
                    os << vformat(" : z =%3d", int(sp.charge) );
                    os << vformat(" : r =%3u", unsigned(sp.rating));
                    os << ' ' << '(' << Species::Status(sp.rating) << ')';
                    os << '\n';
                }
                os << '}';
                return os;
            }
            
            //! display array mapped to species
            template <typename OSTREAM, typename ARR> inline
            OSTREAM & display(OSTREAM &os, ARR &arr, const size_t indent=0) const
            {
                static const ios::scribe &_ = ios::scribe::query<typename ARR::mutable_type>();
                Indent(os,indent) << '{' << '\n';
                for(const SNode *node=sdb.head();node;node=node->next)
                {
                    const Species &sp = ***node;
                    Indent(os,indent) << ' ' << sp << " = " << _.write( &arr[sp.indx] ) << '\n';
                }
                Indent(os,indent) << '}';
                return os;
            }

            //! indent function
            template <typename OSTREAM> static inline
            OSTREAM & Indent(OSTREAM &os, size_t indent)
            {
                while(indent-- >0) os << ' ';
                return os;
            }
            
            //! draw a random concentration
            void   drawC(Addressable &,randomized::bits &) const throw();
            size_t spectators()   const throw(); //!< count spectators
            size_t countWorking() const throw(); //!< count working species
            size_t countPrimary() const throw(); //!< count primary species
            size_t countReplica() const throw(); //!< count replica species

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Library);
            virtual const_type &bulk() const throw();
            Species::Set       sdb;
            const Jive::Motif  jN; //!< for name
            const Jive::Motif  jZ; //!< for charge
            const Jive::Motif  jS; //!< for coefficient
            const Jive::Motif  jB; //!< for blanks
            
            const Species & use(Species *);
            void            update() throw();
            const Species & use(Jive::Source &source);
            string          getName(Jive::Source &) const; //!< get name
            void            noBlank(Jive::Source &) const; //!< skip blanks



        };
    }
}

#endif

