.hello;

@string : jstring;

%include 'lex.g';

@drop : "\t" ' ' "[:blank:]" '\t';
@endl : "[:endl:]";

