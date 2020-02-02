
//! \file
#ifndef Y_IOS_GRAPHVIZIBLE_INCLUDED
#define Y_IOS_GRAPHVIZIBLE_INCLUDED 1

#include "y/ios/iosfwd.hpp"
#include "y/strfwd.hpp"

namespace upsylon {

    namespace ios {
        
        //! GraphViz class helper for a tree-like structure
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

            bool         graphViz(const string &fileName, const bool keepFile=false) const; //!< save and render
            bool         graphViz(const char   *fileName, const bool keepFile=false) const; //!< save and render, wrapper

            //------------------------------------------------------------------
            //
            // static helpers
            //
            //------------------------------------------------------------------
            static void  endl(ostream &);                             //!< append ";\n"
            static void  enterDigraph(ostream &fp, const string &id); //!< "digraph id {\n"
            static void  enterDigraph(ostream &fp, const char   *id); //!< wrapper
            static void  leaveDigraph(ostream &fp );                  //!< "}\n"
            
        protected:
            explicit vizible() throw(); //! setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(vizible);
        };
        
    }
}

#endif

