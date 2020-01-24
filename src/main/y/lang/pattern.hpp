//! \file
#ifndef Y_LANG_PATTERN_INCLUDED
#define Y_LANG_PATTERN_INCLUDED 1

#include "y/lang/token.hpp"
#include "y/lang/source.hpp"
#include "y/type/fourcc.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/serializable.hpp"

namespace upsylon
{
    namespace Lang
    {
        //! alias
        typedef ios::serializable Serializable;

        //! a pattern to accept from a source
        class Pattern : public CountedObject, public Serializable
        {
        public:
            //------------------------------------------------------------------
            //
            //
            // types and definitions
            //
            //
            //------------------------------------------------------------------
            typedef core::list_of_cloneable<Pattern> List;   //!< list of cloneable patterns

            //------------------------------------------------------------------
            //
            //
            // public members
            //
            //
            //------------------------------------------------------------------

            const uint32_t uuid; //!< unique ID for the class
            Pattern       *next; //!< for List
            Pattern       *prev; //!< for List
            void          *priv; //!< pointer on derived type for optimization/compilation


            //------------------------------------------------------------------
            //
            //
            // public methods
            //
            //
            //------------------------------------------------------------------

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual         ~Pattern() throw();                     //!< destructor
            virtual Pattern *clone() const                   = 0;   //!< clone
            virtual void     __viz(ios::ostream &) const     = 0;   //!< GraphViz appearance
            virtual bool     weak()        const throw()     = 0;   //!< a pattern is weak if it matches an empty expression
            virtual bool     match(Token &t, Source &) const = 0;   //!< try to match
            virtual bool     univocal() const throw()        = 0;   //!< guess if univocal

            //__________________________________________________________________
            //
            // non virtual interface
            //__________________________________________________________________
            void        tag(ios::ostream &) const;                              //!< emit its address for GraphViz
            void        link(const Pattern *, ios::ostream &) const;            //!< create a directed link between 'this' and 'p' for GraphViz
            void        viz(ios::ostream &) const;                              //!< emit tag+__viz as a GraphViz node
            const char *vizStyle() const throw();                               //!< get GraphViz style for __viz if needed, based on weak()
            void        GraphViz( const string &fn, bool keepFile=false) const; //!< write a directed graph
            void        GraphViz( const char   *fn, bool keepFile=false) const; //!< write a directed graph, wrapper
            string      toBinary() const;                                       //!< use write() to get a binary code
            string      toBase64() const;                                       //!< human readable from toBinary()
            bool        multiple() const throw();                               //!< !univocal

            //__________________________________________________________________
            //
            // static interface
            //__________________________________________________________________
            static Pattern *Load(ios::istream &);                                     //!< load from an input stream
            static Pattern *Optimize(Pattern *) throw();                              //!< optimize pattern
            static bool     AreEqual(const Pattern &lhs, const Pattern &rhs) throw(); //!< test equality (Single only!!)
            static void     NoMultiple(List &ops) throw();                            //!< remove multiple same patterns in a list of operands

        protected:
            explicit Pattern(const uint32_t id)  throw();  //!< build pattern with a given ID

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pattern);
        };

        //! macro to mark priv as class
#define Y_LANG_PATTERN_IS(CLASS) do { priv = static_cast<CLASS*>(this); } while(false)

        //! implementing class wide className
#define Y_LANG_PATTERN_CLID(CLASS) \
const char CLASS::CLID[8] = {      \
Y_FOURCC_AT(0,UUID),               \
Y_FOURCC_AT(1,UUID),               \
Y_FOURCC_AT(2,UUID),               \
Y_FOURCC_AT(3,UUID),               \
0,0,0,0 }
        
    }
}

#endif

