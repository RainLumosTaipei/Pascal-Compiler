program ExpressionPriority;
var
  x: integer;
begin
  x := 3 + 5 * 2 mod 4;  { 3 + (5*2 mod 4) = 3 + 2 = 5 }
  writeln(x);
end.