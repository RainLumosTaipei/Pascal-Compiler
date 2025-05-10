program MixedFeatures;
var
  arr: array[1..3] of integer;
  i: integer;
begin
  { 初始化数组 }
  for i := 1 to 3 do
    arr[i] := i * 10;

  { 条件判断 }
  if arr[2] > 15 then
    writeln(arr[2])
  else
    writeln(0);
end.