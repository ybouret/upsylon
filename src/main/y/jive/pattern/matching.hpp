//! \file

#ifndef Y_JIVE_PATTERN_MATCHING_INCLUDED
#define Y_JIVE_PATTERN_MATCHING_INCLUDED 1

#include "y/jive/regexp.hpp"
#include "y/container/sequence.hpp"
#include "y/associative/suffix/map.hpp"

namespace upsylon
{
    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //! matching API, acts as a clone pointer
        //
        //______________________________________________________________________
        class Matching : public CountedObject
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef arc_ptr<Matching>          Pointer; //!< alias
            typedef suffix_map<string,Pointer> MapType; //!< alias

            //! map type to store named matching
            class Map : public MapType
            {
            public:
                explicit Map();          //!< setup
                virtual ~Map() throw();  //!< cleanup
                Map(const Map &);        //!< copy

                //! try to register a new matching
                template <typename RX> inline
                Matching & create(const string &id, const RX &rx, const Dictionary *dict=NULL)
                {
                    Matching::Pointer p = new Matching(rx,dict);
                    return insertMatching(id,p);
                }

                //! try to register a new matching
                template <typename RX> inline
                Matching & create(const char *id, const RX &rx, const Dictionary *dict=NULL)
                {
                    const string _(id); return create(id,rx,dict);
                }

                Matching & relate(const string &id, Pattern *p); //!< relate id to pattern
                Matching & relate(const char   *id, Pattern *p); //!< relate id to pattern

                Matching & operator[](const string &) const; //!< access by string
                Matching & operator[](const char   *) const; //!< access by text

            private:
                Y_DISABLE_ASSIGN(Map);
                Matching & insertMatching(const string &, Matching::Pointer &);
            };


            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Matching() throw();             //!< cleanup
            Matching(const Matching &);              //!< copy by cloning

            //! build from an acceptable regular expression
            template <typename REGEXP> inline
            Matching(const REGEXP &rx, const Dictionary *dict =0 ) :
            CountedObject(),
            motif( RegExp(rx,dict) ),  token(), firstChars()
            {
                setup();
            }

            //! build from a pattern
            Matching(Pattern *);


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void         commute(Matching &other) throw(); //!< commute with other
            const Token *exactly_(Module *);               //!< full module content must be accepted
            const Token *foundIn_(Module *);               //!< first accepted token
            size_t       collect_(sequence<Token::Handle> &, Module *); //!< collect all tokens

            //! wrapper
            template <typename DATANAME, typename DATATYPE> inline
            const Token * isExactly( const DATANAME &dataName, const DATATYPE &data )
            {
                return exactly_( Module::OpenData(dataName,data) );
            }

            //! wrapper
            template <typename DATANAME>
            const Token *isExactly( const DATANAME &dataName )
            {
                return exactly_( Module::OpenData(dataName) );
            }

            //! wrapper
            template <typename DATANAME,typename DATATYPE> inline
            const Token *isFoundIn(const DATANAME &dataName, const DATATYPE &data )
            {
                 return foundIn_(Module::OpenData(dataName,data));
            }

            //! wrapper
            template <typename DATANAME> inline
            const Token *isFoundIn(const DATANAME &dataName )
            {
                return foundIn_(Module::OpenData(dataName));
            }

            

            //! wrapper
            template <typename DATANAME,typename DATATYPE> inline
            size_t collect(sequence<Token::Handle> &tokens, const DATANAME &dataName, const DATATYPE &data )
            {
                return collect_(tokens,Module::OpenData(dataName,data));
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Pattern * const motif;      //!< const motif
            Token                 token;      //!< local token
            const Leading         firstChars; //!< compiled firstChars

        private:
            Y_DISABLE_ASSIGN(Matching);
            void setup();
            void cleanup() throw();
        };

    }
}

#endif
