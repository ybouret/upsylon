//! \file
#ifndef Y_LANG_PATTERN_INCLUDED
#define Y_LANG_PATTERN_INCLUDED 1

#include "y/lang/token.hpp"
#include "y/lang/source.hpp"
#include "y/type/fourcc.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/serializable.hpp"
#include "y/ios/tools/vizible.hpp"

namespace upsylon
{
    namespace Lang
    {

        typedef ios::serializable Serializable; //!< alias
        typedef ios::vizible      Vizible;      //!< alias

        //! a pattern to accept from a source
        class Pattern : public CountedObject, public Serializable, public Vizible
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
            virtual bool     weak()        const throw()     = 0;   //!< a pattern is weak if it matches an empty expression
            virtual bool     match(Token &t, Source &) const = 0;   //!< try to match
            virtual bool     univocal() const throw()        = 0;   //!< guess if univocal

            //__________________________________________________________________
            //
            // non virtual interface
            //__________________________________________________________________
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
#define Y_LANG_PATTERN_CLID(CLASS) const char CLASS::CLID[8] = Y_FOURCC_CHAR8(CLASS::UUID)
        
    }
}

#endif

