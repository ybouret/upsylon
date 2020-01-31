
//! \file
#ifndef Y_IOS_GRAPHVIZIBLE_INCLUDED
#define Y_IOS_GRAPHVIZIBLE_INCLUDED 1

#include "y/ios/iosfwd.hpp"

namespace upsylon {

    namespace ios {
        
        //! GrapViz class helper
        class vizible
        {
        public:

            //------------------------------------------------------------------
            //
            // virtual interface
            //
            //------------------------------------------------------------------
            virtual     ~vizible() throw();            //!< cleanup
            virtual void vizCore(ostream &) const = 0; //!< effective GraphViz representation

            //------------------------------------------------------------------
            //
            // non-virtual interface
            //
            //------------------------------------------------------------------
            ostream &    vizName(ostream &) const;                   //!< write GrapViz compliant name
            ostream &    vizJoin(ostream &, const vizible *p) const; //!< join 'this' to 'p';
            void         vizSave(ostream &) const;                   //!< emit vizName() + vizCore()

            //------------------------------------------------------------------
            //
            // static helpers
            //
            //------------------------------------------------------------------
            static void  endl( ostream &);                            //!< append ";\n"

        protected:
            explicit vizible() throw(); //! setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(vizible);
        };
        
    }
}

#endif

