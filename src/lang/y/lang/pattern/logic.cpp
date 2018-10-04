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
                link(p,fp);
            }
        }


        void Logical:: write( ios::ostream &fp ) const
        {
            fp.emit(uuid);
            fp.emit_upack(operands.size);
            for(const Pattern *p = operands.head;p;p=p->next)
            {
                p->write(fp);
            }
        }

    }

}

namespace upsylon
{
    namespace Lang
    {

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
            fp(" [shape=house,style=%s,label=\"&\"];\n",vizStyle());
            vizlink(fp);
        }

        bool AND:: weak() const throw()
        {
            for(const Pattern *p=operands.head;p;p=p->next)
            {
                if(!p->weak()) return false;
            }
            return true; // all weak...
        }
    }

}

namespace upsylon
{
    namespace Lang
    {
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
            fp(" [shape=house,style=%s,label=\"|\"];\n", vizStyle());
            vizlink(fp);
        }

        bool OR:: weak() const throw()
        {
            for(const Pattern *p=operands.head;p;p=p->next)
            {
                if(p->weak()) return true;
            }
            return false; //! no weak
        }
    }
}

namespace upsylon
{
    namespace Lang
    {

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

        bool NONE:: weak() const throw()
        {
            return false;
        }

    }
}
