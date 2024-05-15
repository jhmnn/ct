-- Line Comment

(* Comment
      (*
        Nested Comment
      *)
*)

class Main inherits IO {
  main() : Object {
    if 1 < 2 then {
      out_string("\A\n \
                        string");
    } else {
      a: Int <- 5;
    }
    fi;
  };
};
