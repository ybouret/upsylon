R CMD SHLIB --preclean --clean vecsum.cpp 
R --no-save --slave < vecsum.R 

