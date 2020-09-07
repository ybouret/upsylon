
#include "y/jargon/module.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Module:: ~Module() throw()
        {
        }
        
        Module:: Module(const Tag   &s,
                        const Input &i,
                        const Type   t) throw():
        CountedObject(),
        Context(s),
        input(i),
        type(t)
        {
        }
        
        Module * Module:: OpenFile(const string &f)
        {
            const Tag   s = new string(f);
            const Input i = Kernel::MakeInput::FromFile(f);
            return new Module(s,i,FileStream);
        }

        Module * Module:: OpenFile(const char *f)
        {
            const string _(f);
            return OpenFile(_);
        }
        
        Module * Module:: OpenData(const string &dataName,
                                   const void   *data,
                                   const size_t  size)
        {
            assert(!(NULL==data&&size>0));
            const Tag   s = new string(dataName);
            const Input i = Kernel::MakeInput::FromData(data,size);
            return new Module(s,i,DataStream);
        }

        Module * Module:: OpenData(const char  *dataName,
                                   const void  *data,
                                   const size_t size)
        {
            const string _(dataName);
            return OpenData(_,data,size);
        }
        
        Module * Module:: OpenData(const char   *data,
                                   const size_t  size)
        {
            const string dataName(data,size);
            return OpenData(dataName,data,size);
        }
        
        Module * Module:: OpenData(const string &data)
        {
            return OpenData(data,*data,data.size());
        }
        
        Module * Module:: OpenData(const char *data)
        {
            const string _(data);
            return OpenData(_);
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
                Char * ch = new Char(*this,code);
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

