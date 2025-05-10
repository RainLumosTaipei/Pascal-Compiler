program FunctionDemo;
function Add(a, b: integer): integer;
begin
  Add := a + b;
end;  // 闭合函数定义

begin
  writeln(Add(3, 5));
end.