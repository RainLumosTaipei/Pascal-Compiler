program main;
var
  ia: array [0..9, 0..5] of integer;
  fa: array [0..8, 0..6] of real;
begin
    ia[1, 1] := 10;
    fa[2, 2] := 1.1;

    ia[0, 0] := ia[1, 1];
    fa[0, 0] := fa[2, 2];
end.