int fldpi = 0;
vdpbeg() {
    if( !fldpi ) {
        fldpi = 1;
        dpbeg();
    }
}

vdpend() {
    if( fldpi ) {
        fldpi = 0;
        dpend();
    }
}
