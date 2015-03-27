char   *
        basename (name)
register char  *name;
{
    register char  *s;
    extern char *rindex ();

    s = rindex (name, '/');
    return (((s) ? ++s : name));
}
