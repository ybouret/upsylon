R CMD SHLIB --preclean --clean rmodule.cpp 
R --no-save --slave < rmodule.R 

