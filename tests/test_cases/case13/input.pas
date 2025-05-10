program RecordDemo;
type
  TPoint = record
    x, y: integer;
  end;
var p: TPoint;
begin
  p.x := 3;
  p.y := 4;
  writeln('(', p.x, ',', p.y, ')');
end.