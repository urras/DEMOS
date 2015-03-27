main()
{
    int b8[8], lgt, buf[256], textlgt, bd = 1024*8-1, oldtext, b2;

    read(0, b8, sizeof b8);
    b8[0] = 0407;
    oldtext = b8[1];
    b8[1] = textlgt = (oldtext + bd) & (~bd);
    write(1, b8, sizeof b8);

    textlgt -= oldtext;
    for( ; oldtext; oldtext -= 2 ) {
        read(  0, &b2, sizeof b2 );
        write( 1, &b2, sizeof b2 );
    }

    for( b2=0; textlgt; textlgt -= 2 ) {
        write( 1, &b2, sizeof b2 );
    }

    while( (lgt = read(0,buf,sizeof buf)) )  {
        write(1,buf,lgt);
    }
}
