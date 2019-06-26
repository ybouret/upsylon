//! \file
#ifndef Y_STRING_TOKENIZER_INCLUDED
#define Y_STRING_TOKENIZER_INCLUDED 1

#include "y/string.hpp"
#include "y/container/sequence.hpp"

namespace upsylon
{
    //! tokenizer for strings
    template <typename T>
    class tokenizer
    {
    public:

        //! initialize from a string, must not change!
        inline explicit tokenizer( const core::string<T> &input ) throw():
        curr_( *input ),
        last_( curr_+input.size() ),
        token_(NULL),
        units_(0),
        count_(0)
        {

        }

        //! destructor
        inline virtual ~tokenizer() throw() { }

        //! scan for next tokenizer
        template <typename FUNC>
        inline bool next( FUNC &is_separator ) throw()
        {
            token_ = NULL;
            units_ = 0;
            for(;;)
            {
                if( curr_ >= last_ ) return false;
                if( is_separator( *curr_ ) )
                {
                    ++curr_;
                    continue;
                }
                else
                    break;
            }
            assert( !is_separator( *curr_ ) );
            token_ = curr_;
            units_ = 1;
            ++curr_;

            for(;;)
            {
                if( curr_ >= last_)
                    break;

                if( is_separator(*curr_) )
                {
                    ++curr_;
                    break;
                }

                ++units_;
                ++curr_;
            }
            ++count_;
            return true;
        }

        inline const T *token() const throw() { return token_; } //!< token position
        inline size_t   units() const throw() { return units_; } //!< token length
        inline size_t   count() const throw() { return count_; } //!< token ID

        //! direct conversion to string
        inline core::string<T> to_string(void) const { return core::string<T>( token_, units_); }

        //! automatic splitting of a line
        template <typename FUNC> static inline
        size_t split(sequence< core::string<T> > &words,
                     const core::string<T>       &input,
                     FUNC                        &is_sep)
        {
            words.free();
            tokenizer tkn(input);
            while(tkn.next(is_sep))
            {
                const core::string<T> s( tkn.token_, tkn.units_ );
                words.push_back(s);
            }
            return words.size();
        }

        //! one pass count
        template<typename FUNC> static inline
        size_t count_for( const core::string<T> &input, FUNC &is_sep ) throw()
        {
            tokenizer self(input);
            while( self.next(is_sep) )
                ;
            return self.count_;
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(tokenizer);
        const T *curr_;
        const T *last_;
        const T *token_; //!< token position
        size_t   units_; //!< token length, may be 0
        size_t   count_; //!< token count
    };
}

#endif

