
.list;

lists    : list*;

list       : empty_list | heavy_list;
empty_list : '[' ']';
heavy_list : '[' elements ']';
elements   : element (',' element)*;
element    : NAME | list;
NAME       : "[:alpha:]+";

%include 'lex.g';

