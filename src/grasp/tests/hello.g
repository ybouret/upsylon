.hello;


//add : mult ( (PLUS|MINUS) mult);
add : ;
mul!: ;

// Alias
PLUS : '*'^;


@string : jstring;

%include 'lex.g';

@drop : "\t" ' ' "[:blank:]" '\t';
@endl : "[:endl:]";

%hello;
