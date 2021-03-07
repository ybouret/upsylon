#include "y/gfx/types.hpp"
#include "y/exceptions.hpp"

namespace upsylon
{
    namespace graphic
    {
        unit_t checking:: geqz(const unit_t value, const char *name)
        {
            assert(name);
            if(value<0) throw exception("%s<0",name);
            return value;
        }

        unit_t checking:: gtz(const unit_t value, const char *name)
        {
            assert(name);
            if(value<=0) throw exception("%s<=0",name);
            return value;
        }
        
        const char checking:: height[] = "height";
        const char checking:: width[]  = "width";
        const char checking:: depth[]  = "depth";

       
    }
}


#include "y/string/convert.hpp"
#include "y/string/tokenizer.hpp"
#include "y/sequence/list.hpp"

namespace upsylon
{
    namespace graphic
    {
        coord parsing:: wxh(const string &str)
        {
            static const char fn[] = "wxh: ";
            list<string>      words;
            tokenizer<char>::split_with(words,str,'x');
            if(2!=words.size())
                throw exception("%sinvalid '%s'",fn,*str);
            string &w = words.front(); w.clean_with(" \t");
            string &h = words.back();  w.clean_with(" \t");
            return coord(string_convert::to<size_t>(w, checking::width),
                         string_convert::to<size_t>(h, checking::height)
                         );
        }
        
        coord parsing:: wxh(const char *str)
        {
            const string _(str); return wxh(_);
        }
    }
    
}
