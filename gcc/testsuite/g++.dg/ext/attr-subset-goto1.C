// { dg-do compile { target c++17 } }
// { dg-options "" }
// Subset no_goto disallows goto statements, including computed gotos.
// Labels and other jump statements are allowed.

void
inactive (int i)
{
  goto l;
 l:
  (void) i;
}

[[subset::activate(no_goto)]];

void
active (int i)
{
  void *p = &&out;
  goto out;					// { dg-error "'goto' statement in subset 'no_goto'" }
  for (;;)
    {
      if (i)
	break;
      if (i > 1)
	continue;
      switch (i)
	{
	case 0:
	  return;
	default:
	  break;
	}
    }
  goto *p;					// { dg-error "'goto' statement in subset 'no_goto'" }
 out:
  (void) i;
}

// A template is checked where it is defined, even if never instantiated.
template <typename T>
void
tf (T t)
{
  goto done;					// { dg-error "'goto' statement" }
 done:
  (void) t;
}

auto lambda = [] { goto x; x:; };		// { dg-error "'goto' statement" }

[[subset::suppress(no_goto)]];

void
again ()
{
  goto e;
 e:;
}
