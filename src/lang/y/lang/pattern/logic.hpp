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
            inline virtual ~Logical() throw() {}

            Pattern::List operands;
            inline void     add(const Logical &l) { operands.merge_back_copy(l.operands); }


        protected:
            inline explicit Logical(const uint32_t id) throw() : Pattern(id), operands() {}
            inline Pattern *__clone( Logical *l ) const
            {
                auto_ptr<Logical> p = l;
                l->add(*this);
                return p.yield();
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Logical);
        };

        //! logical AND
        class AND : public Logical
        {
        public:
            //! 0x' && '
            static const char UUID = Y_FOURCC(' ','&', '&', ' ' );

            //! destructor
            inline virtual ~AND() throw() {}
            //! initialize
            inline explicit AND() throw() : Logical(UUID) {}

            //! clone
            inline virtual Pattern *clone() const { return __clone( new AND() ); }
            virtual void            __viz( ios::ostream &fp ) const;     //!< GrapViz

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
            static const char UUID = Y_FOURCC(' ','|', '|', ' ' );

            //! destructor
            inline virtual ~OR() throw() {}
            //! initialize
            inline explicit OR() throw() : Logical(UUID) {}
            //! clone
            inline virtual Pattern *clone() const { return __clone( new OR() ); }
            virtual void            __viz( ios::ostream &fp ) const;     //!< GrapViz

            //! match first pattern
            virtual bool match( Token &tkn, Source &src ) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(OR);
        };

        class NONE : public Logical
        {
        public:
            //! 0x' !! '
            static const char UUID = Y_FOURCC(' ','!', '!', ' ' );

            //! destructor
            inline virtual ~NONE() throw() {}
            //! initialize
            inline explicit NONE() throw() : Logical(UUID) {}
            //! clone
            inline virtual Pattern *clone() const { return __clone( new NONE() ); }
            virtual void            __viz( ios::ostream &fp ) const;     //!< GrapViz

            //! match none of the patterns, returns single next char of false if not char
            virtual bool match( Token &tkn, Source &src ) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(NONE);
        };

    }
}
#endif
