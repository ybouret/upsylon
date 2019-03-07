#include "y/lang/token.hpp"
#include "y/code/utils.hpp"

namespace upsylon
{
    namespace Lang
    {
        string Token::to_string(const size_t nskip, const size_t ntrim) const
        {
            const size_t ndrop = nskip+ntrim;
            if(ndrop>=size)
            {
                return string();
            }
            else
            {
                const size_t n = size-ndrop;
                string       ans(n,as_capacity);
                const Char  *ch = head;

                for(size_t i=0;i<nskip;++i)
                {
                    ch=ch->next;
                }

                for(size_t i=n;i>0;--i,ch=ch->next)
                {
                    ans << char(ch->code);
                }

                return ans;
            }
        }

        string Token::to_print() const
        {
            string ans(2*size,as_capacity);
            for(const Char *ch=head;ch;ch=ch->next)
            {
                ans << printable_char[ch->code];
            }
            return ans;
        }

        string Token::to_visible() const
        {
            string ans(3*size,as_capacity);
            for(const Char *ch=head;ch;ch=ch->next)
            {
                ans << visible_char[ch->code];
            }
            return ans;
        }

        void Token::viz( ios::ostream &fp ) const
        {
            fp << '\'';
            for(const Char *ch=head;ch;ch=ch->next)
            {
                fp << printable_char[ch->code];
            }
            fp << '\'';
        }
    }
}
