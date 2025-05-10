program Factorial;
function Fact(n: integer): integer;
begin
  if n <= 1 then
    Fact := 1
  else
    Fact := n * Fact(n-1);
end;
begin
  writeln(Fact(5));  { 计算5的阶乘 }
end.