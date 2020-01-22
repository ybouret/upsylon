//! \file
#ifndef Y_LANG_PATTERN_INCLUDED
#define Y_LANG_PATTERN_INCLUDED 1

#include "y/lang/token.hpp"
#include "y/ios/ocstream.hpp"
#include "y/lang/source.hpp"
#include "y/type/fourcc.hpp"

namespace upsylon
{
    namespace Lang
    {

        //! a pattern to accept from a source
        class Pattern : public CountedObject
        {
        public:
            typedef core::list_of_cloneable<Pattern> List;   //!< list of cloneable patterns

            const uint32_t uuid; //!< unique ID for the class
            Pattern       *next; //!< for List
            Pattern       *prev; //!< for List
            void          *priv; //!< pointer on derived type for optimization/compilation

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual ~Pattern() throw();                                 //!< destructor
            virtual Pattern *clone() const = 0;                         //!< clone
            virtual void     __viz( ios::ostream &fp ) const = 0;       //!< GraphViz appearance
            virtual void     write( ios::ostream &fp ) const = 0;       //!< binary output
            virtual bool     weak() const throw() = 0;                  //!< a pattern is weak if it matches an empty expression
            virtual bool     match( Token &tkn, Source &src) const = 0; //!< try to match
            virtual bool     univocal() const throw() = 0;              //!< guess if univocal

            //__________________________________________________________________
            //
            // non virtual interface
            //__________________________________________________________________
            void        tag( ios::ostream &os ) const;                          //!< emit its address for GraphViz
            void        link( const Pattern *p, ios::ostream  &os ) const;      //!< create a directed link between 'this' and 'p' for GraphViz
            void        viz( ios::ostream &os ) const;                          //!< emit tag+__viz as a GraphViz node
            const char *vizStyle() const throw();                               //!< get GraphViz style for __viz if needed, based on weak()
            void        GraphViz( const string &fn, bool keepFile=false) const; //!< write a directed graph
            void        GraphViz( const char   *fn, bool keepFile=false) const; //!< write a directed graph, wrapper
            string      toBinary() const;                                       //!< use write() to get a binary code
            string      toBase64() const;                                       //!< human readable from toBinary()

            //__________________________________________________________________
            //
            // static interface
            //__________________________________________________________________
            static Pattern *Load( ios::istream &fp );                                 //!< load from an input stream
            static Pattern *Optimize( Pattern *p ) throw();                           //!< optimize pattern
            static bool     AreEqual(const Pattern &lhs, const Pattern &rhs) throw(); //!< test equality (Single only!!)
            static void     NoMultiple( List &ops ) throw();                          //!< remove multiple same patterns in a list of operands

        protected:
            explicit Pattern(const uint32_t id)  throw();  //!< build pattern with a given ID

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pattern);
        };

        //! macro to mark priv as class
#define Y_LANG_PATTERN_IS(CLASS) do { priv = static_cast<CLASS*>(this); } while(false)

        
    }
}

#endif

