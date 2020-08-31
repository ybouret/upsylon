
#include "y/jive/pattern/first-chars.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/utest/sizeof.hpp"
#include "y/type/aliasing.hpp"
#include "y/code/utils.hpp"

using namespace upsylon;
using namespace Jive;

namespace {

    enum Ownership
    {
        OwnedByPrev,
        OwnedByThis,
        OwnedByNext
    };

    class Leading : public object
    {
    public:
        Leading      *next;
        Leading      *prev;
        const uint8_t kind;
        static const unsigned Byte = 0x01;
        static const unsigned Tier = 0x02;
        virtual ~Leading() throw()
        {
        }


        virtual Ownership whose(const uint8_t c) const throw() = 0;
        virtual Leading  *clone() const  = 0;
        virtual void      display( std::ostream &os ) const    = 0;
        virtual const void *self() const throw() = 0;

        bool owns(const uint8_t c) const throw()
        {
            return OwnedByThis == whose(c);
        }

        friend std::ostream & operator<<(std::ostream &os, const Leading &l)
        {
            l.display(os);
            return os;
        }

        typedef core::list_of_cloneable<Leading> List;

    protected:
        explicit Leading(const uint8_t k) throw() : next(0), prev(0), kind(k) {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(Leading);
    };

    class LeadingByte : public Leading
    {
    public:
        explicit LeadingByte(const uint8_t c) throw() :
        Leading(Byte),
        code(c),
        priv(0)
        {
        }

        virtual ~LeadingByte() throw()
        {

        }

        const uint8_t code;
        const uint8_t priv;

        virtual void display( std::ostream &os ) const
        {
            os << cchars::visible[code];
        }

        virtual Ownership whose(const uint8_t c) const throw()
        {
            if(c<code)
            {
                return OwnedByPrev;
            }
            else if(code<c)
            {
                return  OwnedByNext;
            }
            else
            {
                assert(c==code);
                return OwnedByThis;
            }
        }

        virtual Leading *clone() const
        {
            return new LeadingByte(code);
        }

        const void *self() const throw()
        {
            return static_cast<const LeadingByte *>(this);
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(LeadingByte);
    };

    class LeadingTier : public Leading
    {
    public:
        explicit LeadingTier(const uint8_t lo, const uint8_t up) throw() :
        Leading(Tier),
        lower(lo),
        upper(up)
        {
            assert(lower<upper);
        }

        virtual ~LeadingTier() throw()
        {

        }

        const uint8_t lower;
        const uint8_t upper;

        virtual Ownership whose(const uint8_t c) const throw()
        {
            if(c<lower)
            {
                return OwnedByPrev;
            }
            else if(upper<c)
            {
                return OwnedByNext;
            }
            else
            {
                return OwnedByThis;
            }
        }

        virtual Leading *clone() const
        {
            return new LeadingTier(lower,upper);
        }

        virtual void display( std::ostream &os ) const
        {
            os << cchars::visible[lower] << '-' << cchars::visible[upper];
        }

        const void *self() const throw()
        {
            return static_cast<const LeadingTier *>(this);
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(LeadingTier);
    };

    class LeadingChars
    {
    public:
        explicit LeadingChars() throw() : size(0) {}
        virtual ~LeadingChars() throw() {}

        const size_t size;

        bool search(const uint8_t c) const throw()
        {
            for(const Leading *node=lead.head;node;node=node->next)
            {
                switch( node->whose(c) )
                {
                    case OwnedByPrev: return false; //!< should have been detected
                    case OwnedByThis: return true;  //!< matched
                    case OwnedByNext: break;
                }
            }
            return false;
        }

        void insert(const uint8_t c)
        {
            switch(size)
            {
                case 0:   insert_first(c); return;
                case 256: assert(search(c)); return;
                default:
                    break;
            }
            assert(size>0);
            assert(size<256);


            {
                Leading::List lhs; // list of tested nodes
                while(lead.size)
                {
                    switch( lead.head->whose(c) )
                    {
                        case OwnedByPrev:
                            // not possible: make a new node
                            lhs.push_back( new LeadingByte(c) );
                            lhs.merge_back( lead );
                            lead.swap_with(lhs);
                            goto INSERTED;

                        case OwnedByThis:
                            // found
                            lead.merge_front(lhs);
                            return;

                        case OwnedByNext:
                            lhs.push_back( lead.pop_front() );
                            break;
                    }
                }

                // not found at all
                lhs.push_back( new LeadingByte(c) );
                lhs.swap_with(lead);
            }

            INSERTED:
            ++aliasing::_(size);
            compact();
        }

        void display(std::ostream &os) const
        {
            os << '{';
            if(lead.size>0)
            {
                const Leading *node = lead.head;
                node->display(os);
                for(node=node->next;node;node=node->next)
                {
                    node->display(os<<' ');
                }
            }
            os << '}';
        }

        friend inline std::ostream & operator<<(std::ostream &os, const LeadingChars &lc)
        {
            lc.display(os);
            return os;
        }

    private:
        Leading::List lead;

        void insert_first(const uint8_t c)
        {
            assert(0==size);
            assert(0==lead.size);
            lead.push_back( new LeadingByte(c) );
            ++aliasing::_(size);
        }


#define Y_JIVE_LEADING_STATUS(A,B) ( (A<<8) | B )

        void compact()
        {
            if(lead.size>1)
            {
                Leading::List temp;
                temp.push_back( lead.pop_front() );

                while(lead.size)
                {
                    const Leading *lhs = temp.tail;
                    const Leading *rhs = lead.head;
                    const unsigned Lk   = lhs->kind;
                    const unsigned Rk   = rhs->kind;
                    switch( Y_JIVE_LEADING_STATUS(Lk,Rk) )
                    {
                        case Y_JIVE_LEADING_STATUS(Leading::Byte,Leading::Byte):
                        {
                            const LeadingByte *L = static_cast<const LeadingByte *>(lhs->self());
                            const LeadingByte *R = static_cast<const LeadingByte *>(rhs->self());
                            assert(L->code<R->code);
                            if(L->code+1==R->code)
                            {
                                const uint8_t lower = L->code;
                                const uint8_t upper = R->code;
                                Leading    *node = temp.pop_back();
                                void       *addr = (void*)(node->self());
                                node->~Leading();
                                temp.push_back( new (addr) LeadingTier(lower,upper) );
                                delete lead.pop_front();
                                goto COMPACTED;
                            }
                        } break;

                        case Y_JIVE_LEADING_STATUS(Leading::Byte,Leading::Tier):
                        {
                            const LeadingByte *L = static_cast<const LeadingByte *>(lhs->self());
                            const LeadingTier *R = static_cast<const LeadingTier *>(rhs->self());
                            assert(L->code<R->lower);
                        } break;

                        case Y_JIVE_LEADING_STATUS(Leading::Tier,Leading::Byte): {
                            const LeadingTier *L = static_cast<const LeadingTier *>(lhs->self());
                            const LeadingByte *R = static_cast<const LeadingByte *>(rhs->self());
                            assert(L->upper<R->code);
                        } break;

                        case Y_JIVE_LEADING_STATUS(Leading::Tier,Leading::Tier): {
                            const LeadingTier *L = static_cast<const LeadingTier *>(lhs->self());
                            const LeadingTier *R = static_cast<const LeadingTier *>(rhs->self());
                            assert(L->upper<R->lower);
                        } break;

                        default:
                            throw exception("Jive::LeadingChars: corrupted!");
                    }
                    // no merge
                    temp.push_back(lead.pop_front());
                }
            COMPACTED:
                temp.merge_back(lead);
                lead.swap_with(temp);
            }
        }


    };



}

Y_UTEST(jive_fc)
{

    FirstChars fc;
    fc.complete();
    std::cerr << "fc=" << fc << std::endl;
    fc.free();
    std::cerr << "fc=" << fc << std::endl;

    Y_UTEST_SIZEOF(Leading);
    Y_UTEST_SIZEOF(LeadingTier);
    Y_UTEST_SIZEOF(LeadingByte);

    LeadingChars lc;

    {
        uint8_t ch[256];
        for(unsigned i=0;i<256;++i)
        {
            ch[i] = uint8_t(i);
        }
        alea.shuffle(ch,256);
        for(size_t i=0;i<256;++i)
        {
            lc.insert(ch[i]);
        }
    }
    Y_CHECK(256==lc.size);
    std::cerr << lc << std::endl;

}
Y_UTEST_DONE()
