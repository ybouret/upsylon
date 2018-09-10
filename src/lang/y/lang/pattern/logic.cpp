#include "y/lang/pattern/logic.hpp"

namespace upsylon
{
    namespace Lang
    {

        void Logical:: vizlink(ios::ostream &fp) const
        {
            for(const Pattern *p = operands.head;p;p=p->next)
            {
                p->viz(fp);
                tag(fp); fp << " -> "; p->tag(fp); fp << ";\n";
            }
        }


        bool AND::match(Token &tkn, Source &src) const
        {
            assert(tkn.size==0);
            for(const Pattern *p=operands.head;p;p=p->next)
            {
                Token tmp;
                if(p->match(tmp,src))
                {
                    tkn.merge_back_copy(tmp);
                }
                else
                {
                    assert(0==tmp.size);
                    src.unget(tkn);
                    return false;
                }
            }
            return true;
        }

        void AND:: __viz( ios::ostream &fp ) const
        {
            fp(" [shape=house,label=\"&\"];\n");
            vizlink(fp);
        }

        bool OR:: match( Token &tkn, Source &src ) const
        {
            assert(0==tkn.size);
            for(const Pattern *p=operands.head;p;p=p->next)
            {
                if(p->match(tkn,src))
                {
                    return true;
                }
                assert(0==tkn.size);
            }
            return false;
        }

        void OR:: __viz( ios::ostream &fp ) const
        {
            fp(" [shape=house,label=\"|\"];\n");
            vizlink(fp);
        }

        bool NONE:: match(Token &tkn, Source &src ) const
        {
            assert(0==tkn.size);
            for(const Pattern *p=operands.head;p;p=p->next)
            {
                if(p->match(tkn,src))
                {
                    src.unget(tkn);
                    return false;
                }
                assert(0==tkn.size);
            }
            Char *ch = src.get();
            if(ch)
            {
                tkn.push_back(ch);
                return true;
            }
            else
            {
                return false;
            }
        }

        void NONE:: __viz( ios::ostream &fp ) const
        {
            fp(" [shape=house,label=\"!\"];\n");
            vizlink(fp);
        }


    }
}
