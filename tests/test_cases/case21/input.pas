program NestedCall;
function DoubleValue(n: integer): integer;  { 重命名函数 }
begin
  DoubleValue := n * 2;
end;

function Add(a, b: integer): integer;
begin
  Add := a + DoubleValue(b);  { 同步修改调用 }
end;

begin
  writeln(Add(2, 3));
end.