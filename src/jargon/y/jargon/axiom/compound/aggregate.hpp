
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
            static const char     CLID[];                              //!< "Aggregate"
          
            //! properties
            enum Feature
            {
                Steady,    //!< a steady aggregate, whatsoever
                Acting,    //!< merge content to parent is only one child
                Design     //!< merge all content to parent, ie multiple patterns recognition
            };
            
            typedef int2type<Acting> ActingType; //!< named type
            static const ActingType  AsActing;   //!< helper to setup aggregates
            
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
            Compound(l,UUID), feature(Steady)
            {
                //checkLabel(CLID);
                setup();
            }
            
            //! setup a Transient aggregate
            template <typename LABEL> inline
            explicit Aggregate(const LABEL &l, const ActingType &) :
            Compound(l,UUID), feature(Acting)
            {
                //checkLabel(CLID);
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
            virtual const char *   vizual() const throw();
            virtual ios::ostream & display(ios::ostream &)     const;

            void setup();

        };
    }
}
    
#endif
    
