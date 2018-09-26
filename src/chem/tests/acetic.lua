
species =
{
    { "H+",  1  },
    { "HO-", -1 },
    { "AH",  0  },
    { "A-", -1 }
};


function KA(t)
return 10^(-4.8)
end

eqs =
{
    { "water", 10^(-14), { "H+",  1 }, { "HO-", 1 }  },
    { "acetic", "KA" ,   { "AH", -1 }, { "A-",  1 }, { "H+",1}  }
};
