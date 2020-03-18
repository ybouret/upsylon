
#include "y/jargon/module.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Module:: ~Module() throw()
        {
        }
        
        Module:: Module(const Stamp       &s,
                        const Input       &i,
                        const Char::Cache &c,
                        const Type         t) throw():
        CountedObject(),
        Context(s),
        input(i),
        cache(c),
        type(t)
        {
        }
        
        Module * Module:: OpenFile(const Char::Cache &c,
                                   const string      &f)
        {
            const Stamp s = new string(f);
            const Input i = Kernel::MakeInput::FromFile(f);
            return new Module(s,i,c,FileStream);
        }

        Module * Module:: OpenFile(const Char::Cache &c, const char *f)
        {
            const string _(f);
            return OpenFile(c,_);
        }
        
        Module * Module:: OpenData(const Char::Cache &c,
                                   const string      &dataName,
                                   const void        *data,
                                   const size_t       size)
        {
            assert(!(NULL==data&&size>0));
            const Stamp s = new string(dataName);
            const Input i = Kernel::MakeInput::FromData(data,size);
            return new Module(s,i,c,DataStream);
        }

        Module * Module:: OpenData(const Char::Cache &c,
                                   const char        *dataName,
                                   const void        *data,
                                   const size_t       size)
        {
            const string _(dataName);
            return OpenData(c,_,data,size);
        }
        
        Module * Module:: OpenData(const Char::Cache &c,
                                   const char        *data,
                                   const size_t       size)
        {
            const string dataName(data,size);
            return OpenData(c,dataName,data,size);
        }
        
        Module * Module:: OpenData(const Char::Cache &c,
                                   const string      &data)
        {
            return OpenData(c,data,*data,data.size());
        }


        void Module:: newLine() throw()
        {
            ++aliasing::_(line);
            aliasing::_(column) = 1;
        }

        Char * Module:: getChar()
        {
            char code = 0;
            if(input->query(code))
            {
                Char * ch = Char::Make(cache,*this,code);
                ++aliasing::_(column);
                return ch;
            }
            else
            {
                return NULL;
            }
        }
        

        
    }
    
}

