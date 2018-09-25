.EXPR;

code: (assign | show)*;

show   : add;
assign : ID '=' add;

%include 'eval.g'


