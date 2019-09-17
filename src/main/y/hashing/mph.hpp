//! \file
#ifndef Y_HASHING_MPH_INCLUDED
#define Y_HASHING_MPH_INCLUDED 1

#include "y/core/list.hpp"
#include "y/string.hpp"
#include "y/ios/ostream.hpp"
#include "y/sequence/array.hpp"

namespace upsylon
{
    namespace hashing
    {
        //! minimal perfect hashing
        class mperf
        {
        public:
            //! internal node
            class node_type : public object
            {
            public:
                typedef core::list_of_cpp<node_type> list_type; //!< internal list
                node_type      *next; //!< for list
                node_type      *prev; //!< for list
                const int       hash; //!< in-order hash code
                const uint8_t   code; //!< associated byte
                list_type       chld; //!< child(ren) nodes
                unsigned        freq; //!< frequency

                //! destructor
                virtual ~node_type() throw();
                //! constructor
                explicit node_type(const uint8_t C) throw();

                void viz( ios::ostream &fp ) const; //!< output graphviz
                void optimize() throw(); //!< order by decreasing frequency


            private:
                Y_DISABLE_COPY_AND_ASSIGN(node_type);
            };

            explicit mperf();          //!< default constructor

            //! construct using a database of C strings
            explicit mperf(const char **words,const size_t count);

            //! construct using a set of binary strings+unsigned code
            explicit mperf(const void *data,const size_t size);

            virtual ~mperf() throw();  //!< delete root

            //! insert a new data in tree, h>=0
            void insert(const void *data, const size_t size, const int h);

            //! wrapper for litterals
            inline void insert(const char *text, const int h)
            {
                insert(text,length_of(text),h);
            }

            //! wrapper for buffers
            inline void insert(const memory::ro_buffer &buf, const int h)
            {
                insert(buf.ro(),buf.length(),h);
            }

            //! propagate optimization
            inline void optimize() throw() { root->optimize(); }


            //! get the hash code
            int find(const void *data, const size_t size) const throw();

            //! get the hash code for litterals
            inline int operator()(const char *text) const throw()
            {
                return find(text);
            }

            //! get the hash code for buffers
            inline int operator()(const memory::ro_buffer &buf) const throw()
            {
                return find(buf.ro(),buf.length());
            }

            int hash(const void *data, const size_t size) const;


            //! release all
            void release() throw();

            //! hash and emit keywords+codes
            static void emit_defines(ios::ostream        &fp,
                                     const array<string> &keywords,
                                     const string        &prefix,
                                     const size_t         offset=0);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(mperf);
            node_type *root;



            //! get the hash assuming a '\0' terminated data
            int find(const void *data) const throw();

        public:
            const size_t nodes; //!< nodes in use
            void GraphViz( const string &filename ) const; //!< output GraphViz and render
            //! output GrapViz and render
            inline void
            GraphViz(const char *filename) const
            {
                const string _(filename); GraphViz(_);
            }
        };
    }

    //! to use litterals
#define Y_MPERF_FOR(WORDS) WORDS,sizeof(WORDS)/sizeof(WORDS[0])

   


}

#endif
