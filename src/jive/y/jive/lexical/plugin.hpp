
//! \file

#ifndef Y_JIVE_LEXICAL_PLUGIN_INCLUDED
#define Y_JIVE_LEXICAL_PLUGIN_INCLUDED 1

#include "y/jive/lexical/scanner.hpp"
#include "y/jive/lexical/queue.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Lexical
        {

            //__________________________________________________________________
            //
            //
            //! specialized, loadable scanner
            //
            //__________________________________________________________________
            class Plugin : public Scanner
            {
            public:
                typedef arc_ptr<Plugin>  Pointer; //!< for lexer


                //--------------------------------------------------------------
                //
                // C++
                //
                //--------------------------------------------------------------
                virtual ~Plugin() throw();

                const string P; //!< entering  pattern
                Queue       &Q; //!< destination queue

                void Initialize(const Token &);


            protected:
                template <typename ID, typename RX>
                inline explicit Plugin(const ID &id,
                                       const RX &p,
                                       Queue    &q) :
                Scanner(id),
                P(p),
                Q(q)
                {
                }

                virtual void OnInit(const Token &) = 0;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Plugin);
            };

        }
    }
}

#endif
