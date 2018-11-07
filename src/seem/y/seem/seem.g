//! SEEM grammar
.SEEM;

add    : mult ( (PLUS|MINUS) mult)* ; // left association
mult!  : pow ( (MUL|DIV) pow)* ;      // left association
pow!   : atom ('^'^ pow)? ;           // right association
atom   : ID | INT | '(' add ')' ;     // recursion

PLUS  : '+'^;
MINUS : '-'^;
MUL   : '*'^;
DIV   : '/'^;

ID  : "[:alpha:]+";
INT : "[:digit:]+";

@comment: '//';
@comment: '/*' '*/';


@drop : ' ' "\t";
@endl : "[:endl:]";
