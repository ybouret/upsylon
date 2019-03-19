.sample;


IDs : ID*;

ID   : name (number|string|"0x[:xdigit:]+")* ';';

name   : "[:alpha:]+";
number : "[:digit:]+";


/*
r1 : r2;
r2 : r1+;
*/
@string : jstring;

%include 'lex.g';
