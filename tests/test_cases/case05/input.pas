program ArrayDemo;
var
  arr: array[1..3] of integer;
  i: integer;
begin
  for i := 1 to 3 do
    arr[i] := i * 10;
  for i := 1 to 3 do
    writeln(arr[i]);
end.