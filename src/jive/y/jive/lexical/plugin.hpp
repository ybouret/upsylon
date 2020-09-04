
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

                //! when called by another scanner: OnInit()
                void Initialize(const Token &);


            protected:
                //! initialize label,P,Q
                template <typename ID, typename RX>
                inline explicit Plugin(const ID &id,
                                       const RX &p,
                                       Queue    &q) :
                Scanner(id),
                P(p),
                Q(q)
                {
                }
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Plugin);
                virtual void OnInit(const Token &) = 0;
            };

        }
    }
}

#endif
