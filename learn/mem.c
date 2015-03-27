# include <stdio.h>
# include "lrnref"
# define SAME 0
/*  */
struct keys {
	char *k_wd;
	int k_val;
} 
keybuff[] = {
	{
		"ready",        READY          }
	,
	{
		"готов",       READY           }
	,
	{
		"ответ",       READY           }
	,
	{
		"answer",       READY          }
	,

	{
		"",       READY                }
	,

	{
		"#redefine",       REDEFINE   }
	,
	{
		"#redefoff",       REDEFOFF   }
	,
	{
		"#print",       PRINT         }
	,
	{
		"#copyin",      COPYIN        }
	,
	{
		"#uncopyin",    UNCOPIN       }
	,
	{
		"#copyout",     COPYOUT     }
	,
	{
		"#uncopyout",   UNCOPOUT    }
	,
	{
		"#pipe",        PIPE        }
	,
	{
		"#unpipe",      UNPIPE      }
	,
	{
		"#succeed",     SUCCEED     }
	,
	{
		"#fail",        FAIL        }
	,
	{
		"пока",        BYE          }
	,
	{
		"bye",          BYE      }
	,
	{
		"chdir",        CHDIR    }
	,
	{
		"cd",           CHDIR    }
	,
	{
		"learn",        LEARN    }
	,
	{
		"#log",        LOG       }
	,
	{
		"да",          YES       }
	,
	{
		"yes",          YES      }
	,
	{ 
		"нет",          NO       }
	,
	{
		"no",           NO       }
	,
	{
		"#mv",          MV       }
	,
	{
		"#user",        USER     }
	,
	{
		"#next",        NEXT     }
	,
	{
		"перейти",      SKIP     }
	,
	{
		"skip",         SKIP     }
	,
	{
		"#where",       WHERE    }
	,
	{
		"где",          WHERE    }
	,
	{
		"#match",       MATCH    }
	,
	{
		"#bad",         BAD      }
	,
	{
		"#create",      CREATE   }
	,
	{
		"#cmp",         CMP      }
	,
	{
		"#once",        ONCE     }
	,
	{
		"#",            NOP      }
	,
	{
		NULL,           0        }
};
/*    */
int *action(s)
register char *s;
{
	register struct keys *kp;
	if(reTURN) {
		for (kp=keybuff + 4; kp->k_wd; kp++)
			if (strcmp(kp->k_wd, s) == SAME)
				return(&(kp->k_val)); 
	}
	else {
		for (kp=keybuff; kp->k_wd; kp++)
			if (strcmp(kp->k_wd, s) == SAME)
				return(&(kp->k_val)); 
	}
	return(NULL);
}
/*  */
# define NW 100
# define NWCH 800
struct whichdid {
	char *w_less;
	int w_seq;
} 
which[NW];
int nwh = 0;
char whbuff[NWCH];
char *whcp = whbuff;
/*  */
setdid(lesson, sequence)
register char *lesson;
register sequence;
{
	register  struct whichdid *pw;
	for(pw=which; pw < which+nwh; pw++)
		if (strcmp(pw->w_less, lesson) == SAME)
		{
			pw->w_seq = sequence;
			return;
		}
	pw=which+nwh++;
	if (nwh >= NW) {
		fprintf(stderr, "nwh>=NW\n");
		wrapup(1);
	}
	pw->w_seq = sequence;
	pw->w_less = whcp;
	while (*whcp++ = *lesson++);
	if (whcp >= whbuff + NWCH) {  
		if (flag)fprintf(stderr,"Задано слишком длинное имя урока\n");
		else    fprintf(stderr, "lesson name too long\n");
		wrapup(1);
	}
}
/*  */
already(lesson, sequence)
register char *lesson;
register sequence;
{
	register  struct whichdid *pw;
	for (pw=which; pw < which+nwh; pw++)
		if (strcmp(pw->w_less, lesson) == SAME)
			return(1);
	return(0);
}
