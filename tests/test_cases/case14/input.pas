program SetDemo;
type
  TNumbers = array[1..10] of boolean;  // 用布尔数组模拟集合
var
  nums: TNumbers;
begin
  nums[1] := true;
  nums[3] := true;
  nums[5] := true;
  if nums[3] then
    writeln('3 is in the set');
end.