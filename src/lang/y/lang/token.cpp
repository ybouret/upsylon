#include "y/lang/token.hpp"
#include "y/code/utils.hpp"

namespace upsylon
{
    namespace Lang
    {
        string Token::to_string() const
        {
            string ans(size,as_capacity);
            for(const Char *ch=head;ch;ch=ch->next)
            {
                ans << char(ch->code);
            }
            return ans;
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
