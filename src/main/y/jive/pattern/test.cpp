
#include "y/jive/pattern.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Jive
    {
        void Pattern:: test(Source &source) const
        {
            Char::List content;
            FirstChars fc;
            start(fc);

            while(source.alive())
            {
                Token token;
                if( accept(token,source) )
                {
                    std::cerr << '<' << token << '>';
                    if(token.size)
                    {
                        // someting was accepted, strong
                        if( !fc.search(token.head->code))
                        {
                            std::cerr << std::endl;
                            throw exception("invalid first token char '%s'", cchars::visible[token.head->code]);
                        }
                        content.merge_back(token);
                    }
                    else
                    {
                        // something was accepted, feeble
                        if(strong())
                        {
                            std::cerr << std::endl;
                            throw exception("a strong pattern accepted an empty token!!");
                        }
                        assert(source.alive());
                        Char *ch = source.get();
                        content.push_back(ch);
                        std::cerr << cchars::visible[ch->code];
                    }

                }
                else
                {
                    // not accepted: skip next char un source
                    assert(0==token.size);
                    Char *ch = source.get();
                    content.push_back(ch);
                    std::cerr << cchars::visible[ch->code];
                }

            }
            std::cerr << std::endl;

            source.unget(content);
        }
    }
}


