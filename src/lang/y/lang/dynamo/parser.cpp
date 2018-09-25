#include "y/lang/dynamo/parser.hpp"
#include "y/lang/lexical/plugin/comment.hpp"
#include "y/lang/lexical/plugin/strings.hpp"

namespace upsylon
{
    namespace Lang
    {
        static const char *commands_kw[] =
        {
            "include"
        };
        
        DynamoParser:: DynamoParser() :
        Syntax::Parser("Dynamo"),
        depth(0),
        commands( YOCTO_MPERF_FOR(commands_kw) )
        {

            //__________________________________________________________________
            //
            //
            // setup dynamo with terminals
            //
            //__________________________________________________________________
            AGG &dynamo = agg("dynamo");

            dict("HEAD","[_[:alpha:]]");
            dict("TAIL","[:word:]*");
            dict("ID",  "{HEAD}{TAIL}");

            RULE &END      = mark(';');
            RULE &COLON    = mark(':');
            RULE &RX       = plug<Lexical::jString>("RX");
            RULE &RS       = plug<Lexical::rString>("RS");
            RULE &OS       = (acting("OS") << RS << optional( op('^') ));
            RULE &ID       = term("ID","{ID}");
            //__________________________________________________________________
            //
            //
            // Module name declaration
            //
            //__________________________________________________________________
            dynamo << (agg("Module") << term("ModuleID","\\.{ID}") << END );


            //__________________________________________________________________
            //
            //
            // Grammatical Rule
            //
            //__________________________________________________________________
            AGG &__RULE = agg("RULE");
            {
                __RULE  << ID << COLON;

                //______________________________________________________________
                //
                // rule content
                //______________________________________________________________
                {

                    //______________________________________________________
                    //
                    // an ATOM is a basic content
                    //______________________________________________________
                    ALT &ATOM = alt("ATOM");
                    ATOM << ID << RX << OS;

                    //______________________________________________________
                    //
                    // an ITEM is a modified ATOM or an ATOM
                    //______________________________________________________
                    ALT &ITEM = alt("ITEM");
                    {
                        ITEM << ( agg("OOM") << ATOM << mark('+'));
                        ITEM << ( agg("ZOM") << ATOM << mark('*'));
                        ITEM << ( agg("OPT") << ATOM << mark('?'));
                        ITEM << ATOM;
                    }

                    //______________________________________________________
                    //
                    // an __AGG rule is one or more item
                    //______________________________________________________
                    AGG &__AGG = design("AGG");
                    __AGG  <<  oneOrMore(ITEM);

                    //______________________________________________________
                    //
                    // an __ALT rule is the choice of different __AGG rule
                    //______________________________________________________
                    AGG &__ALT = design("ALT");
                    __ALT  << __AGG   << zeroOrMore( acting("extras") << mark('|') << __AGG  );

                    //______________________________________________________
                    //
                    // the add the GROUP possibility for an ATOM
                    // GROUP is temporary only, a wrapper for ALT
                    //______________________________________________________
                    ATOM << ( acting("GROUP") << mark('(') << __ALT << mark(')') );

                    //______________________________________________________
                    //
                    // done
                    //______________________________________________________
                    __RULE  << __ALT;

                }


                __RULE  << END;
            }

            //__________________________________________________________________
            //
            //
            // Lexical Rule
            //
            //__________________________________________________________________
            RULE &L_ID = term("L_ID","@{ID}");
            AGG  &LXR  = agg("LXR");
            LXR << L_ID << COLON << zeroOrMore( choice(RX,RS) ) << END;

            //__________________________________________________________________
            //
            //
            // Lexical Plugin
            //
            //__________________________________________________________________
            AGG &PLUGIN = agg("PLUGIN");
            PLUGIN << L_ID << COLON << ID << END;

            //__________________________________________________________________
            //
            //
            // Alias Recognition
            //
            //__________________________________________________________________
            RULE &ALIAS = agg("ALIAS") << ID << COLON << choice(RX,OS) << END;

            //__________________________________________________________________
            //
            //
            // command
            //
            //__________________________________________________________________
            RULE &COMMAND = agg("COMMAND") << term("CMD","%{ID}") << zeroOrMore(choice(RX,RS));


            ALT  &entries = alt("entries");
            entries << ALIAS << __RULE  << LXR << PLUGIN << COMMAND;
            dynamo  << zeroOrMore( entries );


            //__________________________________________________________________
            //
            //
            // Extra Lexical Rules
            //
            //__________________________________________________________________
            hook<Lexical::CXX_Comment>("Single Line Comment");
            hook<Lexical::C_Comment>(  "Multi-Lines Comment");
            root.drop("[:blank:]");
            root.endl("[:endl:]");

            checkValidity();
            dict.release();
        }

        DynamoParser:: ~DynamoParser() throw()
        {
        }

        

    }
}

