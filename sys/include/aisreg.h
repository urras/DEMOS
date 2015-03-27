struct aisdevice {
	unsigned aisdat ;
	unsigned aiscsr ;
};

#define AIS_IENABLE   0000100
#define AIS_CLRERR    0000060
#define AIS_REOI      0000050
#define AIS_CLRCHAN   0000030
#define AIS_TRDY      0000004
#define AIS_RDONE     0000001

#define AIS_SR1       0000001
#define AIS_SR3       0000003
#define AIS_SR4       0000004
#define AIS_SR5       0000005

#define AIS_LR1       0000001

#define AISSR1_RIE    0000030
#define AISSR1_SMV    0000004
#define AISSR1_TIE    0000002

#define AISLR1_FRE    0000100
#define AISLR1_OVR    0000040
#define AISLR1_RDPE   0000020

#define AISSR3_R7BITS 0000100
#define AISSR3_RENBL  0000001

#define AISSR4_B2400  0000300
#define AISSR4_B4800  0000200
#define AISSR4_B9600  0000100
#define AISSR4_STOP1  0000004
#define AISSR4_PENBL  0000001

#define AISSR5_T7BITS 0000040
#define AISSR5_BRKHLT 0000020
#define AISSR5_TENBL  0000010

#define AISDELAY 4
