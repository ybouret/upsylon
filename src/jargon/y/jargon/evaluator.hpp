//! \file

#ifndef Y_JARGON_EVALUATOR_INCLUDED
#define Y_JARGON_EVALUATOR_INCLUDED 1

#include "y/jargon/xnode.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //----------------------------------------------------------------------
        //
        //! browsing an ast
        //
        //----------------------------------------------------------------------
        class Evaluator : public virtual Object
        {
        public:
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            
            //! cleanup
            virtual ~Evaluator() throw();
            
            //! setup
            template <typename LABEL> inline
            explicit Evaluator(const LABEL &id) :
            label( Tags::Make(id) ),
            depth(0)
            {
            }
            
            //------------------------------------------------------------------
            //
            // walk down
            //
            //------------------------------------------------------------------
            void browse(const XNode &root); //!< initialize and browse
            
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const Tag label; //!< identifier
            
        protected:
            int depth;       //!< current depth
            
            //! helper to ouput
            std::ostream & indent(std::ostream &) const;
            
            //------------------------------------------------------------------
            //
            // virtual methods
            //
            //------------------------------------------------------------------
            
            //! default call when a terminal is met
            virtual void onTerminal(const Tag &, const Token &);
            
            //! default call when an internal is met
            virtual void onInternal(const Tag &, const size_t );

            //! default call when an operator is met
            virtual void onOperator(const Tag &, const Token &, const size_t );
           
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Evaluator);
            void __browse(const XNode &);
            void __browse(const XList &);
            virtual void onBrowsing() = 0; //! initialize extra data
            virtual void onFinalize() = 0; //! finalize   extra data
        };
        
        //----------------------------------------------------------------------
        //
        //! browsing an ast
        //
        //----------------------------------------------------------------------
        class Evaluator_ : public Evaluator
        {
        public:
            //! setup
            template <typename LABEL> inline
            explicit Evaluator_(const LABEL &id) : Evaluator(id)
            {
            }
            
            //! cleanup
            virtual ~Evaluator_() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Evaluator_);
            virtual void onBrowsing() throw();
            virtual void onFinalize() throw();
            
        };
    }
    
}

#endif

