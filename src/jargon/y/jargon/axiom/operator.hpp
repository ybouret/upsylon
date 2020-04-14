
#ifndef Y_JARGON_AXIOM_OPERATOR_INCLUDED
#define Y_JARGON_AXIOM_OPERATOR_INCLUDED 1

#include "y/jargon/axiom/internal.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //----------------------------------------------------------------------
        //
        //! operator
        /**
         accept a UNIVOCAL lexeme with the same label, shall act as internal
         */
        //----------------------------------------------------------------------
        class Operator : public Internal
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            static const uint32_t UUID = Y_FOURCC('O','P','E','R'); //!< UUID
            static const char     CLID[];                           //!< "Operator"
            
            //! attribute of operator
            enum Attribute
            {
                Multiple, //!< multiple possibilities
                Univocal  //!< only one possibility
            };
            
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
           
            //! cleanup
            virtual ~Operator() throw();
            
            //! setup, check valid name
            template <typename LABEL>
            explicit Operator(const LABEL  &id, const Attribute attr) :
            Internal(id,UUID),
            attribute(attr)
            {
                setup();
            }
            
            //! attribute, detected by the regular expression
            const Attribute attribute;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Operator);
            void setup();
            
            virtual const char *   vizual() const throw();
            virtual bool           accept(Y_JARGON_AXIOM_ARGS) const;
            virtual ios::ostream & display(ios::ostream &)     const;
        };
        
    }
    
}

#endif


