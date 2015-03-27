
typedef union 
{
	int				type_type;	/* OPERATOR TYPES ETC. */
	QTREE				*tree_type;
	int				rng_type;
	char				char_type;
	int				int_type;
	short				*I2_type;
	long				*I4_type;
	float				*F4_type;
	double				*F8_type;
	char				*string_type;
} YYSTYPE;
extern YYSTYPE yylval;
# define APPEND 257
# define COPY 258
# define CREATE 259
# define DELETE 260
# define DESTROY 261
# define HELP 262
# define INDEX 263
# define MODIFY 264
# define PRINT 265
# define RANGE 266
# define REPLACE 267
# define RETRIEVE 268
# define SAVE 269
# define DEFINE 270
# define PERMIT 271
# define VIEW 272
# define INTEGRITY 273
# define ALL 274
# define BY 275
# define FROM 276
# define IN 277
# define INTO 278
# define UNIQUE 279
# define AT 280
# define IS 281
# define OF 282
# define ON 283
# define ONTO 284
# define TO 285
# define UNTIL 286
# define WHERE 287
# define NAME 288
# define SCONST 289
# define I2CONST 290
# define I4CONST 291
# define F4CONST 292
# define F8CONST 293
# define DATECONST 294
# define TIMECONST 295
# define COMMA 296
# define LPAREN 297
# define PERIOD 298
# define RPAREN 299
# define COLON 300
# define BGNCMNT 301
# define ENDCMNT 302
# define UAOP 303
# define BAOP 304
# define BAOPH 305
# define BDOP 306
# define EOP 307
# define LBOP 308
# define LUOP 309
# define FOP 310
# define FBOP 311
# define AGOP 312
# define unaryop 313
