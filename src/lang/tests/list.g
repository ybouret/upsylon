
.List;

list     : '[' elements ']';
elements : element (',' element)*;
element  : NAME | list;
NAME     : "[:alpha:]+";

%include 'lex.g'

