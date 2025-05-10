program ScopeTest;
var
  x: integer;  { 全局变量 }

procedure Modify;
var
  x: integer;  { 局部变量 }
begin
  x := 10;
  writeln(x);
end;

begin
  x := 5;
  Modify;
  writeln(x);
end.