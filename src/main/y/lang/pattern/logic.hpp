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
            Pattern::List operands;          //!< list of operands

            virtual     ~Logical() throw();           //!< destructor
            virtual void write(ios::ostream &) const; //!< [ID] [#operands] [operands]
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
            static const uint32_t UUID = Y_FOURCC(' ','&', '&', ' ' ); //!< [0x && ]
            static const char     CLID[8];                             //!< UUID

            virtual         ~AND() throw();                            //!< destructor
            explicit         AND() throw();                            //!< initialize
            virtual Pattern *clone() const;                            //!< clone
            virtual void     __viz(ios::ostream &) const;              //!< GraphViz output
            virtual bool     match(Token &, Source &) const;           //!< must match all patterns
            virtual bool     weak() const throw();                     //!< if all operands are weak
            virtual bool     univocal() const throw();                 //!< true is all are univocal
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
            static const uint32_t UUID = Y_FOURCC(' ','|', '|', ' ' ); //!< [0x || ]
            static const char     CLID[8];                             //!< UUID

            virtual         ~OR() throw();                             //!< destructor
            explicit         OR() throw();                             //!< initialize
            virtual Pattern *clone() const;                            //!< clone
            virtual void     __viz(ios::ostream &) const;              //!< GraphViz
            virtual bool     match(Token &, Source &) const;           //!< true if finds a matching first operands
            virtual bool     weak() const throw();                     //!< true if one is weak
            virtual bool     univocal() const throw();                 //!< true if single univocal operand
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
            static const uint32_t UUID = Y_FOURCC(' ','!', '!', ' ' ); //!< [0x !! ]
            static const char     CLID[8];                             //!< UUID

            virtual         ~NONE() throw();                           //!< destructor
            explicit         NONE() throw();                           //!< initialize
            virtual Pattern *clone() const;                            //!< clone
            virtual void     __viz(ios::ostream &) const;              //!< GraphViz
            virtual bool     weak() const throw();                     //!< false
            virtual bool     univocal() const throw();                 //!< false, even if could exceptionnaly be true if only 1 possible choice...
            const char *     className() const throw();                //!< CLID

            //! match none of the patterns, returns single next char of false if no char
            virtual bool match(Token &, Source &) const;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(NONE);
        };

    }
}
#endif
