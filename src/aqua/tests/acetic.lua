
lib =
{
  { "H+",   1 },
  { "HO-", -1 },
  { "AH",   0 },
  { "Am",   -1 }
};

Kw = 1e-14;

eqs =
{
    { "water", Kw , "H+", "HO-" },
    { "acetic", 10^(-4.8),  "-AH", "Am", "H+" }
};
