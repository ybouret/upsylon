//! \file
#ifndef Y_LANG_SYNTAX_NODE_EXTENDED_INCLUDED
#define Y_LANG_SYNTAX_NODE_EXTENDED_INCLUDED 1

#include "y/lang/syntax/node/internal.hpp"

namespace upsylon {

    namespace Lang {

        namespace Syntax {

            //! an Extended Node is an internal node with data, for variable operator
            class ExtendedNode : public InternalNode
            {
            public:
                static const id_t UUID = Y_FOURCC(UPFX,'E','X','T');  //!< for I/O

                virtual              ~ExtendedNode() throw();         //!< destructor
                virtual  Node   *     clone() const;                  //!< hardcopy with shared data
                void                  vizCore(ios::ostream &) const;  //!< graphViz
                virtual const string *data() const throw();           //!< & *shared

            private:
                Y_DISABLE_ASSIGN(ExtendedNode);
                Data shared;

                ExtendedNode(const Rule &, const string &s);
                ExtendedNode(const ExtendedNode &node) throw();
                friend class Node;

                virtual const char *className() const throw();
                virtual size_t      serialize(ios::ostream&) const;

            };
        }

    }

}

#endif

