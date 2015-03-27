struct smenu {
	char *sname;                    /* name of submenu */
	int key;                        /* return key */
	int (*exec) ();                 /* function to execute */
	int active;                     /* is it active for now */
	int stag;                       /* tag of submenu */
	int row;                        /* row of submenu */
};

struct menu {
	char *mname;                    /* name of menu */
	int key;                        /* return key */
	struct smenu *submenu;          /* array of submenus */
	int col;                        /* base column of submenu window */
	int len;                        /* length of head name */
	int hgh;                        /* height of submenu window */
	int wid;                        /* width of submenu window */
	int nsm;                        /* current submenu */
	BOX *box;                       /* box under menu */
};

extern struct menu menu [];             /* main menu array */

extern nmenu;                           /* current menu */
