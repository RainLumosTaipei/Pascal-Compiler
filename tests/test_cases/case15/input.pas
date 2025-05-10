program TypeConversion;
var
  y: real;
  x: integer;
begin
  y := 3.14;
  x := round(y);  { 浮点数转整数 }
  writeln(x);
end.