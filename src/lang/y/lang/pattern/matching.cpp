
#include "y/lang/pattern/matching.hpp"

namespace upsylon
{
    namespace Lang
    {
        Matching:: ~Matching() throw() {}

        Matching:: Matching( Pattern *p ) throw(): Token(), motif(p)
        {
        }

        Matching:: Matching( const string &rx, const Dictionary *dict ) :
        Token(), motif( RegExp(rx,dict) )
        {
        }

        Matching:: Matching( const char *rx, const Dictionary *dict ) :
        Token(), motif( RegExp(rx,dict) )
        {
        }

        bool Matching:: exactly(const string &s)
        {
            clear();
            Source source( Module::OpenData(s,s) );
            if( motif->match(*this,source) )
            {
                return !source.active();
            }
            else
            {
                return false;
            }
        }

    }
}
