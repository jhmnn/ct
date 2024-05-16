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
      let a: Int <- 5 in true;
    }
    fi;
  };
};
