
#include "y/gfx/image/format/options.hpp"
#include "y/string/tokenizer.hpp"
#include "y/sequence/list.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace image
        {
            static const char fn[] = "image::options: ";

            options:: options() : options_()
            {
            }

            options:: ~options() throw()
            {
            }

            void options:: parse(const string &opts)
            {
                tokenizer<char> tkn(opts);
                while( tkn.next_with(':'))
                {
                    const string field( tkn.token(), tkn.units() );
                    add(field);
                }
            }


            void options:: parse(const char *opts)
            {
                const string _(opts); parse(_);
            }


            void options:: add(const string &field)
            {
                static const char blanks[]=" \t";

                list<string> words; tokenizer<char>::split_with(words,field,'=');
                if(words.size()!=2) throw exception("%s invalid field '%s'", fn, *field);

                string &name  = words.front(); name.clean_with(blanks);
                string &value = words.back();  value.clean_with(blanks);


                const option::pointer opt = new option(name,value);
                if(!insert(opt)) throw exception("%s multiple entry '%s'",fn,*name);
            }
        }

    }

}
