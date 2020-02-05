//! \file
#ifndef Y_LANG_PATTERN_LOGICAL_INCLUDED
#define Y_LANG_PATTERN_LOGICAL_INCLUDED 1

#include "y/lang/pattern.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon {

    namespace Lang {

        //----------------------------------------------------------------------
        //
        //! Logical operands
        //
        //----------------------------------------------------------------------
        class Logical : public Pattern
        {
        public:
            //__________________________________________________________________
            //
            // types and definition, C++ code
            //__________________________________________________________________
            Pattern::List operands;                         //!< list of operands
            virtual      ~Logical() throw();                //!< destructor

            //__________________________________________________________________
            //
            // interface: Serializable
            //__________________________________________________________________
            virtual size_t serialize(ios::ostream &) const; //!< [UUID] [#operands] [operands]

            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________

            //! syntax helper
            template <typename PATTERN>
            inline PATTERN * add(PATTERN *p ) throw() { (void)operands.push_back(p); return p; } //!< syntax helper
            inline size_t    size() const throw()     { return operands.size;         }          //!< syntax helper
            inline Pattern * remove() throw()         { return operands.pop_back();   }          //!< remove last pattern helper

            //__________________________________________________________________
            //
            // static interface
            //__________________________________________________________________
            static Pattern *Equal(const string &); //!< matching exact string
            static Pattern *Among(const string &); //!< one character of the string
            static Pattern *Equal(const char   *); //!< matching exact string
            static Pattern *Among(const char   *); //!< one character of the string

        protected:
            explicit  Logical(const uint32_t) throw(); //!< initialize
            Pattern * __clone(Logical *) const;        //!< finalize cloning by copy of operands
            void      vizlink(ios::ostream &) const;   //!< link to operands in graphviz

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Logical);
        };

        //----------------------------------------------------------------------
        //
        //! logical AND
        //
        //----------------------------------------------------------------------
        class AND : public Logical
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const uint32_t UUID = Y_PATTERN_CC3('A','N','D'); //!< [0xAND]
            static const char     CLID[8];                            //!< UUID

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual         ~AND() throw();                            //!< destructor
            explicit         AND() throw();                            //!< initialize

            //__________________________________________________________________
            //
            // interface: Pattern
            //__________________________________________________________________
            virtual Pattern *clone() const;                            //!< clone
            virtual void     vizCore(ios::ostream &)  const;           //!< GraphViz output
            virtual bool     match(Token &, Source &) const;           //!< must match all patterns
            virtual bool     weak()     const throw();                 //!< if all operands are weak
            virtual bool     univocal() const throw();                 //!< true is all are univocal

            //__________________________________________________________________
            //
            // interface: Serializable
            //__________________________________________________________________
            const char *     className() const throw();                //!< CLID

        private:
            Y_DISABLE_COPY_AND_ASSIGN(AND);
        };

        //----------------------------------------------------------------------
        //
        //! logical OR
        //
        //----------------------------------------------------------------------
        class OR : public Logical
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const uint32_t UUID = Y_PATTERN_CC2('O','R');      //!< [0xOR]
            static const char     CLID[8];                            //!< UUID

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual         ~OR() throw();                             //!< destructor
            explicit         OR() throw();                             //!< initialize

            //__________________________________________________________________
            //
            // interface: Pattern
            //__________________________________________________________________
            virtual Pattern *clone() const;                            //!< clone
            virtual void     vizCore(ios::ostream &)  const;           //!< GraphViz
            virtual bool     match(Token &, Source &) const;           //!< true if finds a matching first operands
            virtual bool     weak()     const throw();                 //!< true if one is weak
            virtual bool     univocal() const throw();                 //!< true if single univocal operand

            //__________________________________________________________________
            //
            // interface: Serializable
            //__________________________________________________________________
            const char *     className() const throw();                //!< CLID

        private:
            Y_DISABLE_COPY_AND_ASSIGN(OR);
        };

        //----------------------------------------------------------------------
        //
        //! accept something but NONE of the patterns
        //
        //----------------------------------------------------------------------
        class NONE : public Logical
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            static const uint32_t UUID = Y_PATTERN_CC4('N','O', 'N', 'E' ); //!< [0xNONE]
            static const char     CLID[8];                                  //!< UUID

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual         ~NONE() throw();                           //!< destructor
            explicit         NONE() throw();                           //!< initialize

            //__________________________________________________________________
            //
            // interface: Pattern
            //__________________________________________________________________

            //! match none of the patterns, returns single next char of false if no char
            virtual bool     match(Token &, Source &) const;
            virtual Pattern *clone() const;                            //!< clone
            virtual void     vizCore(ios::ostream &) const;            //!< GraphViz
            virtual bool     weak()     const throw();                 //!< false
            virtual bool     univocal() const throw();                 //!< false, even if could exceptionnaly be true if only 1 possible choice...

            //__________________________________________________________________
            //
            // interface: Serializable
            //__________________________________________________________________
            const char *     className() const throw();                //!< CLID

        private:
            Y_DISABLE_COPY_AND_ASSIGN(NONE);
        };

    }
}
#endif
