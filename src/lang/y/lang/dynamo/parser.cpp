#include "y/lang/dynamo/parser.hpp"
#include "y/lang/lexical/plugin/comment.hpp"
#include "y/lang/lexical/plugin/strings.hpp"

namespace upsylon
{
    namespace Lang
    {

        
        DynamoParser:: DynamoParser() : Syntax::Parser("Dynamo")
        {

            //__________________________________________________________________
            //
            //
            // setup dynamo
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
            dynamo << (agg("M") << term("M_ID","\\.{ID}") << END );


            //__________________________________________________________________
            //
            //
            // Grammatical Rule
            //
            //__________________________________________________________________
            AGG &G = agg("G");
            {
                G << ID << COLON;

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
                        ITEM << ( agg("OOM") << ATOM << sole('+'));
                        ITEM << ( agg("ZOM") << ATOM << sole('*'));
                        ITEM << ( agg("OPT") << ATOM << sole('?'));
                        ITEM << ATOM;
                    }

                    //______________________________________________________
                    //
                    // a SUB rule is one or more item
                    //______________________________________________________
                    AGG &SUB = design("SUB");
                    SUB <<  oneOrMore(ITEM);

                    //______________________________________________________
                    //
                    // an ALT rule is the choice of different SUB rule
                    //______________________________________________________
                    AGG &CHX = design("CHX");
                    CHX  << SUB  << zeroOrMore( acting("extras") << mark('|') << SUB );

                    //______________________________________________________
                    //
                    // the add the GROUP possibility for an ATOM
                    // GROUP is temporary only, a wrapper for ALT
                    //______________________________________________________
                    ATOM << ( acting("GROUP") << mark('(') << CHX << mark(')') );

                    //______________________________________________________
                    //
                    // done
                    //______________________________________________________
                    G << CHX;

                }


                G << END;
            }

            //__________________________________________________________________
            //
            //
            // Lexical Rule
            //
            //__________________________________________________________________
            RULE &L_ID = term("L_ID","@{ID}");
            AGG  &L    = agg("L");
            L << L_ID << COLON << zeroOrMore( choice(RX,RS) ) << END;

            //__________________________________________________________________
            //
            //
            // Lexical Plugin
            //
            //__________________________________________________________________
            AGG &P = agg("P");
            P << L_ID << COLON << ID << END;

            //__________________________________________________________________
            //
            //
            // Alias Recognition
            //
            //__________________________________________________________________
            RULE &A = agg("A") << ID << COLON << choice(RX,OS) << END;

            ALT  &Entries = alt("Entries");
            Entries << A << G << L << P;
            dynamo  << G << zeroOrMore( Entries );


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

