.hello;


//add : mult ( (PLUS|MINUS) mult);
r1  : "hello";
r1b : 'world';
r2 : 'world' r1 "example";
r2b! : 'world' '-'^ r1 "example";
r2c : r1|r2|r1;
r2d : r1|r2|r1;
PLUS : '+'^;
r3   : r2*;
r4   : (r1|r2)+;

r5 : r1 r2+;
r6 : ( r1 r2 ) (r3 r4 r5?);
r7! : r1 ( r3 | r4 );

// Alias
ADDOP : "[-+]"^;
$NL   : "[:endl:]";

@string : jstring;

//%include 'lex.g';


@comment : '#';
@comment : '--';
@comment : '/*' '*/';
%hello;
