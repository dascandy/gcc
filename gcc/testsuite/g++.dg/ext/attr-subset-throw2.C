// { dg-do compile { target c++17 } }
// Without <exception>, subset throw_non_std_exception explains that only
// classes derived from std::exception can be thrown.

struct X { };

void
f ()
{
  [[subset::activate(throw_non_std_exception)]];
  throw 1;	// { dg-error "throwing 'int'" }
		// { dg-message "which is not declared; include '<exception>'" "" { target *-*-* } .-1 }
  throw X ();	// { dg-error "throwing 'X'" }
		// { dg-message "which is not declared; include '<exception>'" "" { target *-*-* } .-1 }
  throw;
}
