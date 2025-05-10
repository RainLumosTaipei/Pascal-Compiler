program main;
var 
    a, b, c, d, e: integer; 
    flag: boolean;
    
begin
  a := 1 + 3;
  b := 0 - 8;
  c := 1 * 1;
  d := 2 / 2;
  e := 4 mod 2;

  flag := (a * b / c = e + d) and (a * (a + b) + c <= d + e) or (a - (b * c) = d - a / c)

end.