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

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Logical);
        };

        //! logical AND
        class AND : public Logical
        {
        public:
            //! 0x &&
            static const char UUID = Y_FOURCC(' ','&', '&', ' ' );

            //! destructor
            inline virtual ~AND() throw() {}
            //! initialize
            inline explicit AND() throw() : Logical(UUID) {}

            //! clone
            inline virtual Pattern *clone() const
            {
                auto_ptr<AND> p = new AND(); p->add(*this); return p.yield();
            }

            //! must match all patterns
            virtual bool match( Token &tkn, Source &src ) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(AND);
        };

        //! logical OR
        class OR : public Logical
        {
        public:
            //! 0x ||
            static const char UUID = Y_FOURCC(' ','|', '|', ' ' );

            //! destructor
            inline virtual ~OR() throw() {}
            //! initialize
            inline explicit OR() throw() : Logical(UUID) {}
            //! clone
            inline virtual Pattern *clone() const
            {
                auto_ptr<OR> p = new OR(); p->add(*this); return p.yield();
            }

            //! match first pattern
            virtual bool match( Token &tkn, Source &src ) const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(OR);
        };

    }
}
#endif
