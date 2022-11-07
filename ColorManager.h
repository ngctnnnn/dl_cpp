#pragma once 

#define RST   "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGREEN  "\x1B[32m"
#define KYELLOW  "\x1B[33m"
#define KBLUE  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYAN  "\x1B[36m"
#define KWHITE  "\x1B[37m"

#define FRED(x) KRED x RST
#define FGREEN(x) KGREEN x RST
#define FYELLOW(x) KYELLOW x RST
#define FBLUE(x) KBLU x RST
#define FMAG(x) KMAG x RST
#define FCYAN(x) KCYAN x RST
#define FWHITE(x) KWHITE x RST

#define BOLD(x) "\x1B[1m" x RST
#define UNDERLINE(x) "\x1B[4m" x RST