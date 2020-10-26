
lib =
{
  { "H+",   1 },
  { "HO-", -1 },
  { "AH",   0 },
  { "Am",   -1 },
  { "Na+",   1},
  { "Cl-",  -1},
  { "NH4+", 1 },
  { "NH3" , 0 }
};

Kw = 1e-14;

eqs =
{
    { "water",    Kw,        { 1, "H+"  }, { 1, "HO-" }  },
    { "acetic",   10^(-4.8), {-1, "AH"  }, { 1, "Am"  }, { 1,"H+"} },
    { "ammoniac", 10^(-9.2), {-1, "NH4+"}, { 1, "NH3" }, { 1,"H+"} }
};

C0 = 1e-4

ini =
{
    "E/N",
    {0,  { 1, "AH" }, {1,"Am"} },
    {0,  { 1, "Na+" } },
    {0,  { 1, "Cl-" } },
    {C0,  { 1, "NH4+"}, {1, "NH3" } }
}

Cb = 1e-4;
add =
{
    "E/N",
    {0,   { 1, "AH" }, {1,"Am"} },
    {0,   { 1, "Na+" } },
    {Cb,  { 1, "Cl-" } },
    {0,   { 1, "NH4+"}, {1, "NH3" } }
}
