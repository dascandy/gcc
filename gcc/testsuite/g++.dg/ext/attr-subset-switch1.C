// { dg-do compile { target c++17 } }
// { dg-options "-Wno-switch-unreachable" }
// Subset no_mixed_switch only allows case and default labels as direct
// children of the braced body of the switch they belong to, not nested in
// other statements of it, as in Duff's device, nor in a body without
// braces.

void
duff_inactive (char *to, const char *from, int count)
{
  int n = (count + 7) / 8;
  switch (count % 8)
    {
    case 0: do { *to = *from++;
    case 7:      *to = *from++;
		 } while (--n > 0);
    }
}

[[subset::activate(no_mixed_switch)]];

void
duff (char *to, const char *from, int count)
{
  int n = (count + 7) / 8;
  switch (count % 8)
    {
    case 0: do { *to = *from++;
    case 7:      *to = *from++;			// { dg-error "'case' label not directly in the braced body of its 'switch' in subset 'no_mixed_switch'" }
    case 6:      *to = *from++;			// { dg-error "'case' label not directly in the braced body" }
		 } while (--n > 0);
    }
}

void
nested (int i, int j)
{
  switch (i)
    {
    case 0:
      if (j)
	{
	case 1:					// { dg-error "'case' label not directly in the braced body" }
	  break;
	}
      break;
    case 2:
      {
      case 3:					// { dg-error "'case' label not directly in the braced body" }
	break;
      }
    case 4:
      for (;;)
	{
	default:				// { dg-error "'default' label not directly in the braced body" }
	  break;
	}
      break;
    case 5:
      while (j)
      case 6:					// { dg-error "'case' label not directly in the braced body" }
	--j;
      break;
    }

  // Bodies without braces.
  switch (i)
    if (j)
      case 9:					// { dg-error "'case' label not directly in the braced body" }
	break;
  switch (i)
    case 0:					// { dg-error "'case' label not directly in the braced body" }
      j = 0;
  switch (i)
    default:					// { dg-error "'default' label not directly in the braced body" }
    case 1:					// { dg-error "'case' label not directly in the braced body" }
      j = 1;
}

void
allowed (int i, int j)
{
  switch (i)
    {
    case 0:
    case 1:
      j = 1;
      break;
    case 2:
      {
	int k = j;
	j = k;
      }
      break;
    [[likely]] case 3:
      break;
    default:
      if (j)
	break;
      switch (j)
	{
	case 0:
	  break;
	default:
	  break;
	}
    }

  // A braced body with attributes.
  switch (i)
    [[]] {
    case 0:
      break;
    }

  // Labels in a lambda belong to a switch in the lambda.
  switch (i)
    {
    case 0:
      [] (int x) { switch (x) { case 1: break; } } (j);
      break;
    }
}

// A template is checked where it is defined, even if never instantiated.
template <typename T>
void
tf (T t)
{
  switch (t)
    {
    case 0:
      {
      case 1:					// { dg-error "'case' label not directly in the braced body" }
	break;
      }
    }
}

[[subset::suppress(no_mixed_switch)]];

void
again (int i)
{
  switch (i)
    {
    case 0:
      {
      case 1:
	break;
      }
    }
}
