
//! \file

#ifndef Y_JARGON_AXIOM_AGG_INCLUDED
#define Y_JARGON_AXIOM_AGG_INCLUDED 1

#include "y/jargon/axiom/compound.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //----------------------------------------------------------------------
        //
        //! Aggregate compound, must accept all members
        //
        //----------------------------------------------------------------------
        class Aggregate : public Compound
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            static const uint32_t UUID = Y_FOURCC('A', 'G', 'G', 'R'); //!< UUID
            static const char     CLID[]; //!< "Aggregate"
          
            //! properties
            enum Feature
            {
                Permanent, //!< a permanent aggregate, whatsoever
                Transient, //!< merge content to parent is only one child
                Vanishing  //!< merge all content to parent, ie multiple patterns recognition
            };
            typedef int2type<Transient> TransientType; //!< named type
            static const TransientType  AsTransient;   //!< helper to setup aggregates
            
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            
            //! cleanup
            virtual ~Aggregate() throw();
            
            //! setup a Permanent aggregate
            template <typename LABEL> inline
            explicit Aggregate(const LABEL &l) :
            Compound(l,UUID), feature(Permanent)
            {
                checkLabel(CLID);
                setup();
            }
            
            //! setup a Transient aggregate
            template <typename LABEL> inline
            explicit Aggregate(const LABEL &l, const TransientType &) :
            Compound(l,UUID), feature(Transient)
            {
                checkLabel(CLID);
                setup();
            }
            
            
            //! create a vanishing aggregate
            explicit Aggregate(unsigned &indx);
            
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const Feature feature; //!< feature of this aggregate
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Aggregate);
            virtual bool           accept(Y_JARGON_AXIOM_ARGS) const;
            virtual const char *   vizShape() const throw() { return "house"; }
            virtual ios::ostream & display(ios::ostream &)     const;

            void setup();

        };
    }
}
    
#endif
    
