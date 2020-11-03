#include "y/hashing/mph.hpp"
#include "y/exception.hpp"
#include "y/code/utils.hpp"

namespace upsylon
{

    namespace hashing
    {
        minimal_perfect:: ~minimal_perfect() throw()
        {
        }

        minimal_perfect:: minimal_perfect() : minimal_perfect_tree()
        {
        }

        void minimal_perfect::check(const_type h) const
        {
            if(invalid==h) throw exception("minimal perfect hashing: invalid key");
        }
        
        minimal_perfect:: minimal_perfect(const minimal_perfect &other) : minimal_perfect_tree(other)
        {
        }

        minimal_perfect:: minimal_perfect(const char **words, const unsigned count) :
        minimal_perfect_tree()
        {
            assert(words);
            minimal_perfect &self = *this;
            for(unsigned i=0;i<count;++i)
            {
                const char *word = words[i];
                self(word,i);
            }
        }

        void minimal_perfect:: found(const node_type *node) const
        {
            assert(node);
            exception excp;
            node->format(excp);
            excp.hdr("minimal_perfect: multiple ");
            throw excp;
        }


    }

}

#include "y/ios/ostream.hpp"
#include "y/string.hpp"
#include "y/memory/buffers.hpp"
#include "y/ios/align.hpp"

namespace upsylon
{
    namespace hashing
    {
        void minimal_perfect:: emit(ios::ostream &fp) const
        {
            const size_t           nmax = max_depth();
            memory::cppblock<char> path( nmax );
            for(const data_node *node=head();node;node=node->next)
            {
                const node_type *curr=node->hook;
                const size_t     clen=curr->encode(path);
                string           name( *path, clen );
                const_type       hash = node->data;

                fp << "#define " << ios::align(name,ios::align::left,nmax) << ' ';
                fp("0x%08X",hash);
                fp << '\n';
            }
        }
    }
}
