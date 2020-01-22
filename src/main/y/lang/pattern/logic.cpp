#include "y/lang/pattern/logic.hpp"

namespace upsylon {

    namespace Lang {

        Logical:: ~Logical() throw()
        {
        }


        Logical:: Logical(const uint32_t id) throw() : Pattern(id), operands()
        {
        }

        Pattern * Logical:: __clone( Logical *l ) const
        {
            auto_ptr<Logical> p = l;
            p->operands.merge_back_copy(operands);
            return p.yield();
        }

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
            fp.emit_net(uuid);
            fp.emit_upack(operands.size);
            for(const Pattern *p = operands.head;p;p=p->next)
            {
                p->write(fp);
            }
        }

    }

}

namespace upsylon {

    namespace Lang {

        AND::  ~AND() throw() {}

        AND:: AND() throw() : Logical(UUID) { Y_LANG_PATTERN_IS(AND); }

        Pattern * AND:: clone() const { return __clone( new AND() ); }

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

        bool AND:: univocal() const throw()
        {
            for(const Pattern *p=operands.head;p;p=p->next)
            {
                if(!p->univocal()) return false;
            }
            return true;
        }
    }

}

namespace upsylon {

    namespace Lang {

        OR:: ~OR() throw() {}

        OR::  OR() throw() : Logical(UUID) { Y_LANG_PATTERN_IS(OR); }

        Pattern * OR:: clone() const { return __clone( new OR() ); }

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
            if(operands.size<=0)
            {
                return true;
            }
            else
            {
                for(const Pattern *p=operands.head;p;p=p->next)
                {
                    if(p->weak()) return true;
                }
                return false; //! no weak
            }
        }

        bool OR:: univocal() const throw()
        {
            return (1==operands.size) && operands.head->univocal();
        }
    }
}

namespace upsylon {

    namespace Lang {

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

        bool NONE:: univocal() const throw()
        {
            return false;
        }
    }
}
