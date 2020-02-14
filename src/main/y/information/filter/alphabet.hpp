//! \file
#ifndef Y_INFORMATION_ALPHABET_INCLUDED
#define Y_INFORMATION_ALPHABET_INCLUDED 1

#include "y/information/qbits.hpp"
#include "y/ios/tools/vizible.hpp"
#include "y/sort/merge.hpp"

namespace upsylon {

    namespace information {

        //! base class for a dynamic alphabet
        class Alphabet
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------

            //__________________________________________________________________
            //
            //! operating on buffer or streams
            //__________________________________________________________________
            enum Mode
            {
                StreamMode, //!< will use EOS
                BufferMode  //!< will not use EOS
            };

            typedef size_t        FreqType;          //!< alias
            typedef size_t        CodeType;          //!< alias
            
            static const size_t   Chars = 256;       //!< encoding bytes
            static const CodeType NYT   = Chars;     //!< Not Yet Transmitted
            static const CodeType EOS   = NYT+1;     //!< End Of Stream
            static const size_t   Codes = EOS+1;     //!< maximum possible codes
            static const CodeType Built = Codes;     //!< built marker
            static const size_t   Alive = Chars+1;   //!< never more in alphabet
            static const size_t   Nodes = 2*Alive+1; //!< maximum number of nodes

            //__________________________________________________________________
            //
            //! node for alphabet/tree
            //__________________________________________________________________
            class Node : public ios::vizible
            {
            public:
                const CodeType symbol;    //!< encoding
                FreqType       frequency; //!< frequency
                CodeType       code;      //!< encoded char
                size_t         bits;      //!< current bits
                Node          *next;      //!< for List
                Node          *prev;      //!< for List
                Node          *left;      //!< for Tree
                Node          *right;     //!< for Tree


                explicit Node(const CodeType _code, const size_t _bits) throw(); //!< build a leaf
                explicit Node() throw();                                         //!< build internal

                virtual void vizCore(ios::ostream &) const; //!< for graphViz
                void    emit( qbits &io ) const;            //!< write code/bits to io

                //! handle priority by highest frequency
                class Priority
                {
                public:
                    Priority() throw();  //!< setup
                    ~Priority() throw(); //!< cleanup

                    //! highest frequency
                    int operator()(const Node&,const Node&) throw();
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Priority);
                };

                void display() const;     //!< display data

            private:
                virtual ~Node() throw();
                Y_DISABLE_COPY_AND_ASSIGN(Node);
            };

            //__________________________________________________________________
            //
            //! raw list of nodes
            //__________________________________________________________________
            typedef core::list_of<Node> List;

            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------

            explicit Alphabet(const Mode operating, const size_t extraBytes); //!< setup alphabet
            virtual ~Alphabet() throw(); //!< cleanup

            //! manage alphabet and return modified node
            Node        *send(qbits &io, const uint8_t u);

            //! manage alphabet and return modified node
            Node        *recv(const uint8_t u) throw();

            //! reduce all active frequencies
            void reduceFrequencies() throw();
            
            const Mode   mode;  //!< operating mode
            const size_t level; //!< number of different emitted chars
            const size_t shift; //!< memory shift to get extra
            const size_t bytes; //!< allocated bytes

            void               displayAlpha() const;           //!< display alphabet
            static const char *NameOf(const CodeType) throw(); //!< show node name

        protected:
            List         alpha; //!< currently used alphabet
            Node        *nodes; //!< linear access to nodes
            Node        *nyt;   //!< NYT node
            Node        *eos;   //!< EOS node
            
            void         initialize() throw(); //!< setup minimal alphabet
            void        *extra()      throw(); //!< get location of extra memory

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Alphabet);
            void placeBeforeControls(Node *) throw();

        public:
            const size_t added; //!< added byte
        };
        

    }
}

#endif

