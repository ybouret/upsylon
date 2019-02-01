
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

        Matching:: Matching(const Matching &other) throw() :
        Token(),
        motif( other.motif )
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

        bool Matching:: partly(const string &s)
        {
            clear();
            Source source( Module::OpenData(s,s) );
            while( ! motif->match(*this,source) )
            {
                if(!source.active()) return false;
                source.skip();
            }
            return true;
        }

        size_t Matching:: find( sequence<Token> &seq, const string &s )
        {
            clear();
            seq.free();
            Source source( Module::OpenData(s,s) );
            while( true )
            {
                if(motif->match(*this,source))
                {
                    {
                        const Token empty;
                        seq.push_back(empty);
                    }
                    seq.back().swap_with(*this);
                }
                else
                {
                    if(!source.active()) break;
                    source.skip();
                }

            }
            return seq.size();
        }


    }
}

namespace upsylon
{
    namespace Lang
    {
        MatchString::  MatchString(const char   *rx) : Matching(rx,0) {}
        MatchString::  MatchString(const string &rx) : Matching(rx,0) {}
        MatchString:: ~MatchString() throw() {}
        MatchString::  MatchString(const MatchString &other) throw() : Matching(other) {}

        bool MatchString::  operator()( const string &s ) { return exactly(s); }

        size_t MatchString:: operator()( sequence<Token> &m, const string &s ) { return find(m,s); }
    }
}

