program main_main;

var
    a, b, c: integer;
    
external function add(a, b: integer): integer;

begin
  a := 1;
  b := 2;
  c := add(a, b);
end.