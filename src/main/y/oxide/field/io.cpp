
#include "y/oxide/field/io.hpp"
#include "y/mpl/rational.hpp"
#include "y/string.hpp"

namespace upsylon
{
    namespace Oxide
    {
        IO::Array & IO::Block:: interface() throw()
        {
            return *this;
        }

        const IO::Array & IO::Block:: interface() const throw()
        {
            return *this;
        }


        IO::Block:: Block( const size_t n) :
        Bytes(n,as_capacity),
        prologue()
        {
            memset(prologue,0,sizeof(prologue));
        }

        IO::Block:: ~Block() throw()
        {
            memset(prologue,0,sizeof(prologue));
        }

        void IO::Block:: write(char C)
        {
            push_back( C );
        }

        void IO::Block:: flush()
        {
            
        }

        IO::Block &   IO::Block:: setFast(const size_t n)
        {
            adjust(n,0);
            return *this;
        }

        IO::Block &  IO::Block:: setZero(const size_t n)
        {
            make(n,0);
            return *this;
        }



    }

}

#include "y/ios/omstream.hpp"
#include "y/ios/imstream.hpp"

namespace upsylon
{
    namespace Oxide
    {
        void IO::Block:: encodePrologue() const  
        {
            size_t        len = 0;
            ios::omstream out(prologue,Prologue);
            out.emit_upack( size(), &len );
            while(len<Prologue)
            {
                prologue[len++] = 0;
            }
        }

        size_t IO::Block:: decodePrologue() const
        {
            ios::imstream inp(prologue,Prologue);
            return inp.read_upack<size_t>();
        }

    }
}
