//! \file
#ifndef Y_LANG_PATTERN_LOGICAL_INCLUDED
#define Y_LANG_PATTERN_LOGICAL_INCLUDED 1

#include "y/lang/pattern.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon
{
    namespace Lang
    {
        //! Logical operands
        class Logical : public Pattern
        {
        public:
            //! destructor
            inline virtual ~Logical() throw() {}

            //! list of operands
            Pattern::List operands;

            //! syntactic helper
            template <typename PATTERN>
            inline PATTERN *add( PATTERN *p ) throw()
            {
                operands.push_back(p);
                return p;
            }

            static Pattern *Equal(const string &); //!< matching exact string
            static Pattern *Among(const string &); //!< one character of the string

            static inline Pattern *Equal(const char *s) { const string _ = s; return Equal(_); }
            static inline Pattern *Among(const char *s) { const string _ = s; return Among(_); }

            virtual void write( ios::ostream &fp ) const;
            
        protected:
            //! initialize
            inline explicit Logical(const uint32_t id) throw() : Pattern(id), operands() {}

            //! finalize cloning by copy of operands
            inline Pattern *__clone( Logical *l ) const
            {
                auto_ptr<Logical> p = l;
                p->operands.merge_back_copy(operands);
                return p.yield();
            }

            //! link in graphviz
            void vizlink( ios::ostream &fp ) const;



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Logical);
        };

        //! logical AND
        class AND : public Logical
        {
        public:
            //! 0x' && '
            static const uint32_t UUID = Y_FOURCC(' ','&', '&', ' ' );

            //! destructor
            inline virtual ~AND() throw() {}
            //! initialize
            inline explicit AND() throw() : Logical(UUID) { Y_LANG_PATTERN_IS(AND); }

            inline virtual Pattern *clone() const { return __clone( new AND() ); }  //!< clone
            virtual void            __viz( ios::ostream &fp ) const;                //!< GraphViz

            //! must match all patterns
            virtual bool match( Token &tkn, Source &src ) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(AND);
        };

        //! logical OR
        class OR : public Logical
        {
        public:
            //! 0x' || '
            static const uint32_t UUID = Y_FOURCC(' ','|', '|', ' ' );

            //! destructor
            inline virtual ~OR() throw() {}
            //! initialize
            inline explicit OR() throw() : Logical(UUID) { Y_LANG_PATTERN_IS(OR); }
            //! clone
            inline virtual Pattern *clone() const { return __clone( new OR() ); }   //!< clone
            virtual void            __viz( ios::ostream &fp ) const;                //!< GraphViz

            //! match first pattern
            virtual bool match( Token &tkn, Source &src ) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(OR);
        };

        //! accept something but NONE of the patterns
        class NONE : public Logical
        {
        public:
            //! 0x' !! '
            static const uint32_t UUID = Y_FOURCC(' ','!', '!', ' ' );

            //! destructor
            inline virtual ~NONE() throw() {}
            //! initialize
            inline explicit NONE() throw() : Logical(UUID) {Y_LANG_PATTERN_IS(NONE); }
            inline virtual Pattern *clone() const { return __clone( new NONE() ); } //!< clone
            virtual void            __viz( ios::ostream &fp ) const;                //!< GraphViz

            //! match none of the patterns, returns single next char of false if not char
            virtual bool match( Token &tkn, Source &src ) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(NONE);
        };

    }
}
#endif
