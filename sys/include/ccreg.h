
#define SOP     0100000
#define DFLAG   040000
#define MA      0400
#define MAD     01000
#define UBL     01400
#define UBR     02000
#define UBC     02400
#define	XMA	03000


struct  device
{
        int     csr;
        int     dmr;
        int     dhr;
};

#define DENB    0100
#define DMR     0377

