program RepeatDemo;
var i: integer;
begin
  i := 0;
  repeat
    i := i + 1;
  until i > 5;
  writeln(i);
end.