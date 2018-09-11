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

        //! macro to mark priv as class
#define Y_LANG_PATTERN_IS(CLASS) do { priv = static_cast<CLASS*>(this); } while(false)

        //! a pattern to accept from a source
        class Pattern : public Object
        {
        public:
            const uint32_t uuid; //!< unique ID for the class
            Pattern       *next; //!< for List
            Pattern       *prev; //!< for List
            void          *priv; //!< pointer on derived type for optimization/compilation

            virtual ~Pattern() throw(); //!< destructor

            //! emit its address for GraphViz
            void  tag( ios::ostream &os ) const;
            //! create a directed link for GraphViz
            void  link( const Pattern *p, ios::ostream  &os ) const;

            //! clone
            virtual Pattern *clone() const = 0;
            //! try to match
            virtual bool     match( Token &tkn, Source &src) const = 0;
            //! GraphViz appearance
            virtual void     __viz( ios::ostream &fp )       const = 0;
            //! binary output
            virtual void     write( ios::ostream &fp )       const = 0;

            //! list of cloneable patterns
            typedef core::list_of_cloneable<Pattern> List;

            //! emit GraphViz node
            void viz( ios::ostream &os ) const; //!< tag+__viz

            //! write a directed graph
            void        GraphViz( const string &fn, bool keepFile=false) const;
            //! write a directed graph
            inline void GraphViz( const char   *fn, bool keepFile=false) const { const string _ = fn; GraphViz(_,keepFile); }

            //! load from an input stream
            static Pattern *Load( ios::istream &fp );


            //! optimize pattern
            static Pattern *Optimize( Pattern *p ) throw();

            //! test patterns

            //! binary "value"
            string to_binary() const; //!< use output to get a binary code
            string to_base64() const; //!< human readable code

            static bool AreEqual(const Pattern &lhs, const Pattern &rhs) throw();
            static void NoMultiple( List &ops ) throw();

        protected:
            //! build pattern from ID
            explicit Pattern(const uint32_t id)  throw();


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pattern);
        };

        
    }
}

#endif

