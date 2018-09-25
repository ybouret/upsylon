// JSON grammar to be compiled by Dynamo

.JSON;

json : object | array;

/* Arrays */
array : empty_array | heavy_array;

empty_array : '[' ']';
heavy_array : '[' value ( ',' value)* ']';

/* Objects */
object : empty_object | heavy_object;

empty_object : '{' '}';
heavy_object : '{' pair ( ',' pair)* '}';
pair : string ':' value;

@string : jstring;

@drop : "[:blank:]";
@endl : "[:endl:]";

number : "[:digit:]+";

value  : string | number | 'true' | 'false' | 'null' | object | array;

