// JSON grammar to be compiled by Dynamo
.JSON;

json : object | array;

array : empty_array | heavy_array;

empty_array : '[' ']';

