
#include "y/graphic/image.hpp"
#include "y/string/tokenizer.hpp"
#include "y/sequence/vector.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Graphic {


        Image:: Options:: ~Options() throw()
        {
        }

        Image:: Options:: Options() throw()  : OptionDB()
        {
        }


        Image::Options:: Options(const char *txt) : OptionDB()
        {
            (void) parse(txt);
        }


        Image::Options:: Options(const string &str) : OptionDB()
        {
            (void) parse(str);
        }


        Image::Options & Image::Options:: parse(const string &str)
        {
            static const char fn[] = "Image::Options::parse: ";
            vector<string,memory::pooled> option(2,as_capacity);
            tokenizer<char>               tkn(str);
            while( tkn.next_with(':') )
            {
                const string opt = tkn.to_string();
                option.free();
                tokenizer<char>::split_with(option,opt,'=');
                if(2 != option.size()) throw exception("%s: invalu=id option '%s'", fn, *opt);
                string key   = option.front(); key.clean_with(" \t");
                string value = option.back();  value.clean_with(" \t");

                string *pValue = this->search(key);
                if(pValue)
                {
                    *pValue = value;
                }
                else
                {
                    if( !this->insert(key,value) ) throw exception("%s: unexpected failure to insert <%s=%s>", fn, *key, *value);
                }

            }

            return *this;
        }

        Image::Options & Image::Options:: parse(const char *txt)
        {
            const string _(txt);
            return parse(_);
        }

        Image::Options & Image::Options:: operator<<( const string &str )
        {
            return parse(str);
        }

        Image::Options & Image::Options:: operator<<( const char *txt )
        {
            return parse(txt);
        }



        Image:: Options:: Options(const Options &other) : collection(), OptionDB(other)
        {
        }


        bool Image:: Options:: flag( const string &name ) const
        {
            const string *pValue = this->search(name);
            if( !pValue )
            {
                return false;
            }
            else
            {
                string value = *pValue;
                string_convert::to_lower(value);
                if( value == "false" || value == "0" || value == "off" )
                {
                    return false;
                }
                else if( value == "true" || value == "1" || value == "on" )
                {
                    return true;
                }
                else
                {
                    throw exception("Image::Options::flag(invalid <%s=%s>)",*name,*value);
                }
            }
        }

        bool Image::Options:: Flag(const Options *options, const string &name)
        {
            if(options)
            {
                return options->flag(name);
            }
            else
            {
                return false;
            }
        }

        bool Image::Options:: Flag(const Options *options, const char *name)
        {
            const string _(name); return Flag(options,_);
        }



    }

}
