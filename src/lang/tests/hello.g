.hello;


//add : mult ( (PLUS|MINUS) mult);
r1 : "hello";
r1b : 'hello';
r2 : 'hello' r1;
r3 : r1+;
r4 : r1 r2 r3 PLUS;
r5 : r1 r2+;
r6 : ( r1 r2 ) (r3 r4 r5?);
r7 : r1|r2;
r8 : r1|r2|r3;
r9 : r1|r2|r3|r4+;
r10 : (r1) | (r2 r3) | r4?;
r11 : r1 ( r3 | r3 );
// Alias
PLUS : '+'^;
ADDOP : "[-+]"^;


@string : jstring;

%include 'lex.g';

@drop : "\t" ' ' "[:blank:]" '\t';
@endl : "[:endl:]";

%hello;
