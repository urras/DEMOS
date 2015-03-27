# define TRIM 0177

/*
 *      int match (char *name, char *pattern)
 *
 *      Match name with pattern. Returns 1 if successful.
 *      Pattern may contain wild symbols:
 *
 *      ^       - at beginning of string - any string not matched
 *      *       - any string
 *      ?       - any symbol
 *      [a-z]   - symbol in range
 *      [^a-z]  - symbol out of range
 */

match (s, p)
register char *s, *p;
{
	register scc;
	int c, yes;

	yes = 1;
	if (*p == '^') {
		yes = 0;
		++p;
	}
	for (;;) {
		scc = *s++ & TRIM;
		switch (c = *p++) {
		case '[': {
			int ok, lc, good;

			lc = 077777;
			good = 1;
			if (*p == '^') {
				good = 0;
				++p;
			}
			ok = ! good;
			for (;;) {
				int cc;

				cc = *p++;
				if (cc == 0)
					return (! yes);         /* Missing ] */
				if (cc == ']')
					break;
				if (cc == '-') {
					if (lc <= scc && scc <= *p++)
						ok = good;
				} else
					if (scc == (lc = cc))
						ok = good;
			}
			if (! ok)
				return (! yes);
			continue;
		}
		case '*':
			if (! *p)
				return (yes);
			for (--s; *s; ++s)
				if (match (s, p))
					return (yes);
			return (! yes);
		case 0:
			return (scc==0 ? yes : !yes);
		default:
			if ((c & TRIM) != scc)
				return (! yes);
			continue;
		case '?':
			if (scc == 0)
				return (! yes);
			continue;
		}
	}
}
