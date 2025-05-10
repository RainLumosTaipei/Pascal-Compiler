program MultiArray;
var
  matrix: array[1..2, 1..2] of integer;
  i, j: integer;
begin
  for i := 1 to 2 do
    for j := 1 to 2 do
      matrix[i][j] := i * j;  // 使用 Pascal 标准语法
  writeln(matrix[2][1]);
end.