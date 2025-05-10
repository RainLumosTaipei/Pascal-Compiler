program ConstantDemo;
const
  PI = 3.1415;
var
  r: real;
begin
  r := 2.0;
  writeln(PI * r * r);  { 移除格式化参数 }
end.