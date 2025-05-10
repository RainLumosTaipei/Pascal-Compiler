program main;
var q : integer;

function MyFunc(p: integer): integer;
begin
  p := p - 1;
  MyFunc := p;
end;

begin
    q := 10;
    q := MyFunc(q);
end.