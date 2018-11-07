.eval;


add  : mult ( (PLUS|MINUS) mult)* ; // left association
//mult : pow ('*'^ pow)* ; // left association
//pow  : atom ('^'^ pow)? ; // right association
mult!  : atom ( ('*'|'/'^) atom)*;
atom   : ID | INT | '(' add ')' ; // recursion

PLUS  : '+'^;
MINUS : '-';

ID  : "[:alpha:]+";
INT : "[:digit:]+";

@comment: '//';
@comment: '/*' '*/';
%include 'lex.g'
//%include "lex.g"
