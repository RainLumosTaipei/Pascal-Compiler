program main;

function forFunc: integer;
var a, b: integer;
begin
    a := 0;
    b := 1;
    for a := 0 to 4 do
        b := b * 2;
    forFunc := b;
end;

begin
  forFunc();
end.