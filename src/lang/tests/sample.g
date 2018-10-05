.SAMPLE;

main : (a b) | c;

a : "[:alpha:]+";
b : "[:punct:]+";
c : "[:digit:]+";

%include 'lex.g'
