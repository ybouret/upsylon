.hello;


//add : mult ( (PLUS|MINUS) mult);
add : ;

// Alias
PLUS : '*'^;


@string : jstring;

%include 'lex.g';

@drop : "\t" ' ' "[:blank:]" '\t';
@endl : "[:endl:]";

%hello;
