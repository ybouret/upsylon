

//! \file

#ifndef Y_JARGON_AXIOM_REPEAT_INCLUDED
#define Y_JARGON_AXIOM_REPEAT_INCLUDED 1

#include "y/jargon/axiom/wildcard.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //----------------------------------------------------------------------
        //
        //! repeat the same axiom at least minimalCount
        //
        //----------------------------------------------------------------------
        class Repeat : public Wildcard
        {
        public:
            const size_t minimalCount; //!< minimal repeats
            virtual ~Repeat() throw(); //!< cleanup
                        
        protected:
            //! setup for derived classes
            explicit Repeat(Axiom         &axiom,
                            const size_t   nmin,
                            const uint32_t user,
                            const char     mark);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Repeat);
            virtual bool accept(Y_JARGON_AXIOM_ARGS) const;
        };
        
        //----------------------------------------------------------------------
        //
        //! generic class to build [One|Zero]OrMore
        //
        //----------------------------------------------------------------------
        template <const size_t NMIN, const char MARK>
        class Rep : public Repeat
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            static const char     Mark         = MARK;                       //!< label marker
            static const uint32_t UUID         = Y_FOURCC('R','E','P',MARK); //!< UUID
            static const size_t   MinimalCount = NMIN;                       //!< for minimalCount
          
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            
            //! setup
            inline explicit Rep(Axiom &axiom) : Repeat(axiom,MinimalCount,UUID,Mark)
            {
            }
            
            //! cleanu
            inline virtual ~Rep() throw()
            {}

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Rep);
        };
        
        typedef Rep<0,'*'> ZeroOrMore; //!< repeat zero or more times
        typedef Rep<1,'+'> OneOrMore;  //!< repeat one or more times
        
    }
    
}

#endif
