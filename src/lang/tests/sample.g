.sample;


IDs : ID*;

ID   : name number* ';';

name   : "[:alpha:]+";
number : "[:digit:]+";


/*
r1 : r2;
r2 : r1+;
*/

%include 'lex.g';
