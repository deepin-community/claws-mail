/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "matcher_parser_parse.y"

/*
 * Claws Mail -- a GTK based, lightweight, and fast e-mail client
 * Copyright (c) 2001-2014 by Hiroyuki Yamamoto & The Claws Mail Team
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "defs.h"

#include <glib.h>
#include <glib/gi18n.h>

#include "utils.h"
#include "filtering.h"
#include "procheader.h"
#include "matcher.h"
#include "matcher_parser.h"
#include "matcher_parser_lex.h"
#include "colorlabel.h"
#include "folder_item_prefs.h"

static gint error = 0;
static gint bool_op = 0;
static gint match_type = 0;
static gchar *header = NULL;

static MatcherProp *prop;

static GSList *matchers_list = NULL;

static gboolean enabled = TRUE;
static gchar *name = NULL;
static gint account_id = 0;
static MatcherList *cond;
static GSList *action_list = NULL;
static FilteringAction *action = NULL;
static gboolean matcher_is_fast = TRUE;
static gboolean disable_warnings = FALSE;

static FilteringProp *filtering;
static gboolean filtering_ptr_externally_managed = FALSE;

static GSList **prefs_filtering = NULL;
static int enable_compatibility = 0;

enum {
        MATCHER_PARSE_FILE,
        MATCHER_PARSE_NO_EOL,
	MATCHER_PARSE_ENABLED,
	MATCHER_PARSE_NAME,
	MATCHER_PARSE_ACCOUNT,
        MATCHER_PARSE_CONDITION,
        MATCHER_PARSE_FILTERING_ACTION,
};

static int matcher_parse_op = MATCHER_PARSE_FILE;


/* ******************************************************************** */
/* redeclarations to avoid warnings */
void matcher_parserrestart(FILE *input_file);
void matcher_parser_init(void);
void matcher_parser_switch_to_buffer(void * new_buffer);
void matcher_parser_delete_buffer(void * b);
void matcher_parserpop_buffer_state(void);
int matcher_parserlex(void);

void matcher_parser_disable_warnings(const gboolean disable)
{
	disable_warnings = disable;
}

void matcher_parser_start_parsing(FILE *f)
{
	matcher_parserlineno = 1;
	matcher_parserrestart(f);
	account_id = 0;
	matcher_parserparse();
}

 
void * matcher_parser_scan_string(const char * str);
 
FilteringProp *matcher_parser_get_filtering(gchar *str)
{
	void *bufstate;
	void *tmp_str = NULL;
	
	/* little hack to allow passing rules with no names */
	if (!strncmp(str, "rulename ", 9))
		tmp_str = g_strdup(str);
	else 
		tmp_str = g_strconcat("rulename \"\" ", str, NULL);

	/* bad coding to enable the sub-grammar matching
	   in yacc */
	matcher_parserlineno = 1;
	matcher_parse_op = MATCHER_PARSE_NO_EOL;
	matcher_parserrestart(NULL);
	matcher_parserpop_buffer_state();
        matcher_parser_init();
	bufstate = matcher_parser_scan_string((const char *) tmp_str);
        matcher_parser_switch_to_buffer(bufstate);
	/* Indicate that we will be using the global "filtering" pointer,
	 * so that yyparse does not free it in "filtering_action_list"
	 * section. */
	filtering_ptr_externally_managed = TRUE;
	if (matcher_parserparse() != 0)
		filtering = NULL;
	matcher_parse_op = MATCHER_PARSE_FILE;
	matcher_parser_delete_buffer(bufstate);
	g_free(tmp_str);
	filtering_ptr_externally_managed = FALSE; /* Return to normal. */
	return filtering;
}

static gboolean check_quote_symetry(gchar *str)
{
	const gchar *walk;
	int ret = 0;
	
	if (str == NULL)
		return TRUE; /* heh, that's symetric */
	if (*str == '\0')
		return TRUE;
	for (walk = str; *walk; walk++) {
		if (*walk == '\"') {
			if (walk == str 	/* first char */
			|| *(walk - 1) != '\\') /* not escaped */
				ret ++;
		}
	}
	return !(ret % 2);
}

MatcherList *matcher_parser_get_name(gchar *str)
{
	void *bufstate;

	if (!check_quote_symetry(str)) {
		cond = NULL;
		return cond;
	}
	
	/* bad coding to enable the sub-grammar matching
	   in yacc */
	matcher_parserlineno = 1;
	matcher_parse_op = MATCHER_PARSE_NAME;
	matcher_parserrestart(NULL);
	matcher_parserpop_buffer_state();
        matcher_parser_init();
	bufstate = matcher_parser_scan_string(str);
	matcher_parserparse();
	matcher_parse_op = MATCHER_PARSE_FILE;
	matcher_parser_delete_buffer(bufstate);
	return cond;
}

MatcherList *matcher_parser_get_enabled(gchar *str)
{
	void *bufstate;

	if (!check_quote_symetry(str)) {
		cond = NULL;
		return cond;
	}
	
	/* bad coding to enable the sub-grammar matching
	   in yacc */
	matcher_parserlineno = 1;
	matcher_parse_op = MATCHER_PARSE_ENABLED;
	matcher_parserrestart(NULL);
	matcher_parserpop_buffer_state();
	matcher_parser_init();
	bufstate = matcher_parser_scan_string(str);
	matcher_parserparse();
	matcher_parse_op = MATCHER_PARSE_FILE;
	matcher_parser_delete_buffer(bufstate);
	return cond;
}

MatcherList *matcher_parser_get_account(gchar *str)
{
	void *bufstate;

	if (!check_quote_symetry(str)) {
		cond = NULL;
		return cond;
	}
	
	/* bad coding to enable the sub-grammar matching
	   in yacc */
	matcher_parserlineno = 1;
	matcher_parse_op = MATCHER_PARSE_ACCOUNT;
	matcher_parserrestart(NULL);
	matcher_parserpop_buffer_state();
	matcher_parser_init();
	bufstate = matcher_parser_scan_string(str);
	matcher_parserparse();
	matcher_parse_op = MATCHER_PARSE_FILE;
	matcher_parser_delete_buffer(bufstate);
	return cond;
}

MatcherList *matcher_parser_get_cond(gchar *str, gboolean *is_fast)
{
	void *bufstate;

	if (!check_quote_symetry(str)) {
		cond = NULL;
		return cond;
	}
	
	matcher_is_fast = TRUE;
	/* bad coding to enable the sub-grammar matching
	   in yacc */
	matcher_parserlineno = 1;
	matcher_parse_op = MATCHER_PARSE_CONDITION;
	matcher_parserrestart(NULL);
	matcher_parserpop_buffer_state();
        matcher_parser_init();
	bufstate = matcher_parser_scan_string(str);
	matcher_parserparse();
	matcher_parse_op = MATCHER_PARSE_FILE;
	matcher_parser_delete_buffer(bufstate);
	if (is_fast)
		*is_fast = matcher_is_fast;
	return cond;
}

GSList *matcher_parser_get_action_list(gchar *str)
{
	void *bufstate;

	if (!check_quote_symetry(str)) {
		action_list = NULL;
		return action_list;
	}
	
	/* bad coding to enable the sub-grammar matching
	   in yacc */
	matcher_parserlineno = 1;
	matcher_parse_op = MATCHER_PARSE_FILTERING_ACTION;
	matcher_parserrestart(NULL);
	matcher_parserpop_buffer_state();
        matcher_parser_init();
	bufstate = matcher_parser_scan_string(str);
	matcher_parserparse();
	matcher_parse_op = MATCHER_PARSE_FILE;
	matcher_parser_delete_buffer(bufstate);
	return action_list;
}

MatcherProp *matcher_parser_get_prop(gchar *str)
{
	MatcherList *list;
	MatcherProp *prop;

	matcher_parserlineno = 1;
	list = matcher_parser_get_cond(str, NULL);
	if (list == NULL)
		return NULL;

	if (list->matchers == NULL)
		return NULL;

	if (list->matchers->next != NULL)
		return NULL;

	prop = list->matchers->data;

	g_slist_free(list->matchers);
	g_free(list);

	return prop;
}

void matcher_parsererror(char *str)
{
	GSList *l;

	if (matchers_list) {
		for (l = matchers_list; l != NULL; l = g_slist_next(l)) {
			matcherprop_free((MatcherProp *)
					 l->data);
			l->data = NULL;
		}
		g_slist_free(matchers_list);
		matchers_list = NULL;
	}
	cond = NULL;
	if (!disable_warnings)
		g_warning("filtering parsing: %i: %s",
		  	matcher_parserlineno, str);
	error = 1;
}

int matcher_parserwrap(void)
{
	return 1;
}

#line 387 "matcher_parser_parse.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_MATCHER_PARSER_PARSE_H_INCLUDED
# define YY_YY_MATCHER_PARSER_PARSE_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    MATCHER_ALL = 258,             /* MATCHER_ALL  */
    MATCHER_UNREAD = 259,          /* MATCHER_UNREAD  */
    MATCHER_NOT_UNREAD = 260,      /* MATCHER_NOT_UNREAD  */
    MATCHER_NEW = 261,             /* MATCHER_NEW  */
    MATCHER_NOT_NEW = 262,         /* MATCHER_NOT_NEW  */
    MATCHER_MARKED = 263,          /* MATCHER_MARKED  */
    MATCHER_NOT_MARKED = 264,      /* MATCHER_NOT_MARKED  */
    MATCHER_DELETED = 265,         /* MATCHER_DELETED  */
    MATCHER_NOT_DELETED = 266,     /* MATCHER_NOT_DELETED  */
    MATCHER_REPLIED = 267,         /* MATCHER_REPLIED  */
    MATCHER_NOT_REPLIED = 268,     /* MATCHER_NOT_REPLIED  */
    MATCHER_FORWARDED = 269,       /* MATCHER_FORWARDED  */
    MATCHER_NOT_FORWARDED = 270,   /* MATCHER_NOT_FORWARDED  */
    MATCHER_SUBJECT = 271,         /* MATCHER_SUBJECT  */
    MATCHER_NOT_SUBJECT = 272,     /* MATCHER_NOT_SUBJECT  */
    MATCHER_FROM = 273,            /* MATCHER_FROM  */
    MATCHER_NOT_FROM = 274,        /* MATCHER_NOT_FROM  */
    MATCHER_TO = 275,              /* MATCHER_TO  */
    MATCHER_NOT_TO = 276,          /* MATCHER_NOT_TO  */
    MATCHER_CC = 277,              /* MATCHER_CC  */
    MATCHER_NOT_CC = 278,          /* MATCHER_NOT_CC  */
    MATCHER_TO_OR_CC = 279,        /* MATCHER_TO_OR_CC  */
    MATCHER_NOT_TO_AND_NOT_CC = 280, /* MATCHER_NOT_TO_AND_NOT_CC  */
    MATCHER_AGE_GREATER = 281,     /* MATCHER_AGE_GREATER  */
    MATCHER_AGE_LOWER = 282,       /* MATCHER_AGE_LOWER  */
    MATCHER_NEWSGROUPS = 283,      /* MATCHER_NEWSGROUPS  */
    MATCHER_AGE_GREATER_HOURS = 284, /* MATCHER_AGE_GREATER_HOURS  */
    MATCHER_AGE_LOWER_HOURS = 285, /* MATCHER_AGE_LOWER_HOURS  */
    MATCHER_DATE_AFTER = 286,      /* MATCHER_DATE_AFTER  */
    MATCHER_DATE_BEFORE = 287,     /* MATCHER_DATE_BEFORE  */
    MATCHER_NOT_NEWSGROUPS = 288,  /* MATCHER_NOT_NEWSGROUPS  */
    MATCHER_INREPLYTO = 289,       /* MATCHER_INREPLYTO  */
    MATCHER_NOT_INREPLYTO = 290,   /* MATCHER_NOT_INREPLYTO  */
    MATCHER_MESSAGEID = 291,       /* MATCHER_MESSAGEID  */
    MATCHER_NOT_MESSAGEID = 292,   /* MATCHER_NOT_MESSAGEID  */
    MATCHER_REFERENCES = 293,      /* MATCHER_REFERENCES  */
    MATCHER_NOT_REFERENCES = 294,  /* MATCHER_NOT_REFERENCES  */
    MATCHER_SCORE_GREATER = 295,   /* MATCHER_SCORE_GREATER  */
    MATCHER_SCORE_LOWER = 296,     /* MATCHER_SCORE_LOWER  */
    MATCHER_HEADER = 297,          /* MATCHER_HEADER  */
    MATCHER_NOT_HEADER = 298,      /* MATCHER_NOT_HEADER  */
    MATCHER_HEADERS_PART = 299,    /* MATCHER_HEADERS_PART  */
    MATCHER_NOT_HEADERS_PART = 300, /* MATCHER_NOT_HEADERS_PART  */
    MATCHER_MESSAGE = 301,         /* MATCHER_MESSAGE  */
    MATCHER_HEADERS_CONT = 302,    /* MATCHER_HEADERS_CONT  */
    MATCHER_NOT_HEADERS_CONT = 303, /* MATCHER_NOT_HEADERS_CONT  */
    MATCHER_NOT_MESSAGE = 304,     /* MATCHER_NOT_MESSAGE  */
    MATCHER_BODY_PART = 305,       /* MATCHER_BODY_PART  */
    MATCHER_NOT_BODY_PART = 306,   /* MATCHER_NOT_BODY_PART  */
    MATCHER_TEST = 307,            /* MATCHER_TEST  */
    MATCHER_NOT_TEST = 308,        /* MATCHER_NOT_TEST  */
    MATCHER_MATCHCASE = 309,       /* MATCHER_MATCHCASE  */
    MATCHER_MATCH = 310,           /* MATCHER_MATCH  */
    MATCHER_REGEXPCASE = 311,      /* MATCHER_REGEXPCASE  */
    MATCHER_REGEXP = 312,          /* MATCHER_REGEXP  */
    MATCHER_SCORE = 313,           /* MATCHER_SCORE  */
    MATCHER_MOVE = 314,            /* MATCHER_MOVE  */
    MATCHER_FOUND_IN_ADDRESSBOOK = 315, /* MATCHER_FOUND_IN_ADDRESSBOOK  */
    MATCHER_NOT_FOUND_IN_ADDRESSBOOK = 316, /* MATCHER_NOT_FOUND_IN_ADDRESSBOOK  */
    MATCHER_IN = 317,              /* MATCHER_IN  */
    MATCHER_COPY = 318,            /* MATCHER_COPY  */
    MATCHER_DELETE = 319,          /* MATCHER_DELETE  */
    MATCHER_MARK = 320,            /* MATCHER_MARK  */
    MATCHER_UNMARK = 321,          /* MATCHER_UNMARK  */
    MATCHER_LOCK = 322,            /* MATCHER_LOCK  */
    MATCHER_UNLOCK = 323,          /* MATCHER_UNLOCK  */
    MATCHER_EXECUTE = 324,         /* MATCHER_EXECUTE  */
    MATCHER_MARK_AS_READ = 325,    /* MATCHER_MARK_AS_READ  */
    MATCHER_MARK_AS_UNREAD = 326,  /* MATCHER_MARK_AS_UNREAD  */
    MATCHER_FORWARD = 327,         /* MATCHER_FORWARD  */
    MATCHER_MARK_AS_SPAM = 328,    /* MATCHER_MARK_AS_SPAM  */
    MATCHER_MARK_AS_HAM = 329,     /* MATCHER_MARK_AS_HAM  */
    MATCHER_FORWARD_AS_ATTACHMENT = 330, /* MATCHER_FORWARD_AS_ATTACHMENT  */
    MATCHER_EOL = 331,             /* MATCHER_EOL  */
    MATCHER_OR = 332,              /* MATCHER_OR  */
    MATCHER_AND = 333,             /* MATCHER_AND  */
    MATCHER_COLOR = 334,           /* MATCHER_COLOR  */
    MATCHER_SCORE_EQUAL = 335,     /* MATCHER_SCORE_EQUAL  */
    MATCHER_REDIRECT = 336,        /* MATCHER_REDIRECT  */
    MATCHER_SIZE_GREATER = 337,    /* MATCHER_SIZE_GREATER  */
    MATCHER_SIZE_SMALLER = 338,    /* MATCHER_SIZE_SMALLER  */
    MATCHER_SIZE_EQUAL = 339,      /* MATCHER_SIZE_EQUAL  */
    MATCHER_LOCKED = 340,          /* MATCHER_LOCKED  */
    MATCHER_NOT_LOCKED = 341,      /* MATCHER_NOT_LOCKED  */
    MATCHER_PARTIAL = 342,         /* MATCHER_PARTIAL  */
    MATCHER_NOT_PARTIAL = 343,     /* MATCHER_NOT_PARTIAL  */
    MATCHER_COLORLABEL = 344,      /* MATCHER_COLORLABEL  */
    MATCHER_NOT_COLORLABEL = 345,  /* MATCHER_NOT_COLORLABEL  */
    MATCHER_IGNORE_THREAD = 346,   /* MATCHER_IGNORE_THREAD  */
    MATCHER_NOT_IGNORE_THREAD = 347, /* MATCHER_NOT_IGNORE_THREAD  */
    MATCHER_WATCH_THREAD = 348,    /* MATCHER_WATCH_THREAD  */
    MATCHER_NOT_WATCH_THREAD = 349, /* MATCHER_NOT_WATCH_THREAD  */
    MATCHER_CHANGE_SCORE = 350,    /* MATCHER_CHANGE_SCORE  */
    MATCHER_SET_SCORE = 351,       /* MATCHER_SET_SCORE  */
    MATCHER_ADD_TO_ADDRESSBOOK = 352, /* MATCHER_ADD_TO_ADDRESSBOOK  */
    MATCHER_STOP = 353,            /* MATCHER_STOP  */
    MATCHER_HIDE = 354,            /* MATCHER_HIDE  */
    MATCHER_IGNORE = 355,          /* MATCHER_IGNORE  */
    MATCHER_WATCH = 356,           /* MATCHER_WATCH  */
    MATCHER_SPAM = 357,            /* MATCHER_SPAM  */
    MATCHER_NOT_SPAM = 358,        /* MATCHER_NOT_SPAM  */
    MATCHER_HAS_ATTACHMENT = 359,  /* MATCHER_HAS_ATTACHMENT  */
    MATCHER_HAS_NO_ATTACHMENT = 360, /* MATCHER_HAS_NO_ATTACHMENT  */
    MATCHER_SIGNED = 361,          /* MATCHER_SIGNED  */
    MATCHER_NOT_SIGNED = 362,      /* MATCHER_NOT_SIGNED  */
    MATCHER_TAG = 363,             /* MATCHER_TAG  */
    MATCHER_NOT_TAG = 364,         /* MATCHER_NOT_TAG  */
    MATCHER_SET_TAG = 365,         /* MATCHER_SET_TAG  */
    MATCHER_UNSET_TAG = 366,       /* MATCHER_UNSET_TAG  */
    MATCHER_TAGGED = 367,          /* MATCHER_TAGGED  */
    MATCHER_NOT_TAGGED = 368,      /* MATCHER_NOT_TAGGED  */
    MATCHER_CLEAR_TAGS = 369,      /* MATCHER_CLEAR_TAGS  */
    MATCHER_ENABLED = 370,         /* MATCHER_ENABLED  */
    MATCHER_DISABLED = 371,        /* MATCHER_DISABLED  */
    MATCHER_RULENAME = 372,        /* MATCHER_RULENAME  */
    MATCHER_ACCOUNT = 373,         /* MATCHER_ACCOUNT  */
    MATCHER_STRING = 374,          /* MATCHER_STRING  */
    MATCHER_SECTION = 375,         /* MATCHER_SECTION  */
    MATCHER_INTEGER = 376          /* MATCHER_INTEGER  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define MATCHER_ALL 258
#define MATCHER_UNREAD 259
#define MATCHER_NOT_UNREAD 260
#define MATCHER_NEW 261
#define MATCHER_NOT_NEW 262
#define MATCHER_MARKED 263
#define MATCHER_NOT_MARKED 264
#define MATCHER_DELETED 265
#define MATCHER_NOT_DELETED 266
#define MATCHER_REPLIED 267
#define MATCHER_NOT_REPLIED 268
#define MATCHER_FORWARDED 269
#define MATCHER_NOT_FORWARDED 270
#define MATCHER_SUBJECT 271
#define MATCHER_NOT_SUBJECT 272
#define MATCHER_FROM 273
#define MATCHER_NOT_FROM 274
#define MATCHER_TO 275
#define MATCHER_NOT_TO 276
#define MATCHER_CC 277
#define MATCHER_NOT_CC 278
#define MATCHER_TO_OR_CC 279
#define MATCHER_NOT_TO_AND_NOT_CC 280
#define MATCHER_AGE_GREATER 281
#define MATCHER_AGE_LOWER 282
#define MATCHER_NEWSGROUPS 283
#define MATCHER_AGE_GREATER_HOURS 284
#define MATCHER_AGE_LOWER_HOURS 285
#define MATCHER_DATE_AFTER 286
#define MATCHER_DATE_BEFORE 287
#define MATCHER_NOT_NEWSGROUPS 288
#define MATCHER_INREPLYTO 289
#define MATCHER_NOT_INREPLYTO 290
#define MATCHER_MESSAGEID 291
#define MATCHER_NOT_MESSAGEID 292
#define MATCHER_REFERENCES 293
#define MATCHER_NOT_REFERENCES 294
#define MATCHER_SCORE_GREATER 295
#define MATCHER_SCORE_LOWER 296
#define MATCHER_HEADER 297
#define MATCHER_NOT_HEADER 298
#define MATCHER_HEADERS_PART 299
#define MATCHER_NOT_HEADERS_PART 300
#define MATCHER_MESSAGE 301
#define MATCHER_HEADERS_CONT 302
#define MATCHER_NOT_HEADERS_CONT 303
#define MATCHER_NOT_MESSAGE 304
#define MATCHER_BODY_PART 305
#define MATCHER_NOT_BODY_PART 306
#define MATCHER_TEST 307
#define MATCHER_NOT_TEST 308
#define MATCHER_MATCHCASE 309
#define MATCHER_MATCH 310
#define MATCHER_REGEXPCASE 311
#define MATCHER_REGEXP 312
#define MATCHER_SCORE 313
#define MATCHER_MOVE 314
#define MATCHER_FOUND_IN_ADDRESSBOOK 315
#define MATCHER_NOT_FOUND_IN_ADDRESSBOOK 316
#define MATCHER_IN 317
#define MATCHER_COPY 318
#define MATCHER_DELETE 319
#define MATCHER_MARK 320
#define MATCHER_UNMARK 321
#define MATCHER_LOCK 322
#define MATCHER_UNLOCK 323
#define MATCHER_EXECUTE 324
#define MATCHER_MARK_AS_READ 325
#define MATCHER_MARK_AS_UNREAD 326
#define MATCHER_FORWARD 327
#define MATCHER_MARK_AS_SPAM 328
#define MATCHER_MARK_AS_HAM 329
#define MATCHER_FORWARD_AS_ATTACHMENT 330
#define MATCHER_EOL 331
#define MATCHER_OR 332
#define MATCHER_AND 333
#define MATCHER_COLOR 334
#define MATCHER_SCORE_EQUAL 335
#define MATCHER_REDIRECT 336
#define MATCHER_SIZE_GREATER 337
#define MATCHER_SIZE_SMALLER 338
#define MATCHER_SIZE_EQUAL 339
#define MATCHER_LOCKED 340
#define MATCHER_NOT_LOCKED 341
#define MATCHER_PARTIAL 342
#define MATCHER_NOT_PARTIAL 343
#define MATCHER_COLORLABEL 344
#define MATCHER_NOT_COLORLABEL 345
#define MATCHER_IGNORE_THREAD 346
#define MATCHER_NOT_IGNORE_THREAD 347
#define MATCHER_WATCH_THREAD 348
#define MATCHER_NOT_WATCH_THREAD 349
#define MATCHER_CHANGE_SCORE 350
#define MATCHER_SET_SCORE 351
#define MATCHER_ADD_TO_ADDRESSBOOK 352
#define MATCHER_STOP 353
#define MATCHER_HIDE 354
#define MATCHER_IGNORE 355
#define MATCHER_WATCH 356
#define MATCHER_SPAM 357
#define MATCHER_NOT_SPAM 358
#define MATCHER_HAS_ATTACHMENT 359
#define MATCHER_HAS_NO_ATTACHMENT 360
#define MATCHER_SIGNED 361
#define MATCHER_NOT_SIGNED 362
#define MATCHER_TAG 363
#define MATCHER_NOT_TAG 364
#define MATCHER_SET_TAG 365
#define MATCHER_UNSET_TAG 366
#define MATCHER_TAGGED 367
#define MATCHER_NOT_TAGGED 368
#define MATCHER_CLEAR_TAGS 369
#define MATCHER_ENABLED 370
#define MATCHER_DISABLED 371
#define MATCHER_RULENAME 372
#define MATCHER_ACCOUNT 373
#define MATCHER_STRING 374
#define MATCHER_SECTION 375
#define MATCHER_INTEGER 376

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 317 "matcher_parser_parse.y"

	char *str;
	int value;

#line 687 "matcher_parser_parse.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_MATCHER_PARSER_PARSE_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_MATCHER_ALL = 3,                /* MATCHER_ALL  */
  YYSYMBOL_MATCHER_UNREAD = 4,             /* MATCHER_UNREAD  */
  YYSYMBOL_MATCHER_NOT_UNREAD = 5,         /* MATCHER_NOT_UNREAD  */
  YYSYMBOL_MATCHER_NEW = 6,                /* MATCHER_NEW  */
  YYSYMBOL_MATCHER_NOT_NEW = 7,            /* MATCHER_NOT_NEW  */
  YYSYMBOL_MATCHER_MARKED = 8,             /* MATCHER_MARKED  */
  YYSYMBOL_MATCHER_NOT_MARKED = 9,         /* MATCHER_NOT_MARKED  */
  YYSYMBOL_MATCHER_DELETED = 10,           /* MATCHER_DELETED  */
  YYSYMBOL_MATCHER_NOT_DELETED = 11,       /* MATCHER_NOT_DELETED  */
  YYSYMBOL_MATCHER_REPLIED = 12,           /* MATCHER_REPLIED  */
  YYSYMBOL_MATCHER_NOT_REPLIED = 13,       /* MATCHER_NOT_REPLIED  */
  YYSYMBOL_MATCHER_FORWARDED = 14,         /* MATCHER_FORWARDED  */
  YYSYMBOL_MATCHER_NOT_FORWARDED = 15,     /* MATCHER_NOT_FORWARDED  */
  YYSYMBOL_MATCHER_SUBJECT = 16,           /* MATCHER_SUBJECT  */
  YYSYMBOL_MATCHER_NOT_SUBJECT = 17,       /* MATCHER_NOT_SUBJECT  */
  YYSYMBOL_MATCHER_FROM = 18,              /* MATCHER_FROM  */
  YYSYMBOL_MATCHER_NOT_FROM = 19,          /* MATCHER_NOT_FROM  */
  YYSYMBOL_MATCHER_TO = 20,                /* MATCHER_TO  */
  YYSYMBOL_MATCHER_NOT_TO = 21,            /* MATCHER_NOT_TO  */
  YYSYMBOL_MATCHER_CC = 22,                /* MATCHER_CC  */
  YYSYMBOL_MATCHER_NOT_CC = 23,            /* MATCHER_NOT_CC  */
  YYSYMBOL_MATCHER_TO_OR_CC = 24,          /* MATCHER_TO_OR_CC  */
  YYSYMBOL_MATCHER_NOT_TO_AND_NOT_CC = 25, /* MATCHER_NOT_TO_AND_NOT_CC  */
  YYSYMBOL_MATCHER_AGE_GREATER = 26,       /* MATCHER_AGE_GREATER  */
  YYSYMBOL_MATCHER_AGE_LOWER = 27,         /* MATCHER_AGE_LOWER  */
  YYSYMBOL_MATCHER_NEWSGROUPS = 28,        /* MATCHER_NEWSGROUPS  */
  YYSYMBOL_MATCHER_AGE_GREATER_HOURS = 29, /* MATCHER_AGE_GREATER_HOURS  */
  YYSYMBOL_MATCHER_AGE_LOWER_HOURS = 30,   /* MATCHER_AGE_LOWER_HOURS  */
  YYSYMBOL_MATCHER_DATE_AFTER = 31,        /* MATCHER_DATE_AFTER  */
  YYSYMBOL_MATCHER_DATE_BEFORE = 32,       /* MATCHER_DATE_BEFORE  */
  YYSYMBOL_MATCHER_NOT_NEWSGROUPS = 33,    /* MATCHER_NOT_NEWSGROUPS  */
  YYSYMBOL_MATCHER_INREPLYTO = 34,         /* MATCHER_INREPLYTO  */
  YYSYMBOL_MATCHER_NOT_INREPLYTO = 35,     /* MATCHER_NOT_INREPLYTO  */
  YYSYMBOL_MATCHER_MESSAGEID = 36,         /* MATCHER_MESSAGEID  */
  YYSYMBOL_MATCHER_NOT_MESSAGEID = 37,     /* MATCHER_NOT_MESSAGEID  */
  YYSYMBOL_MATCHER_REFERENCES = 38,        /* MATCHER_REFERENCES  */
  YYSYMBOL_MATCHER_NOT_REFERENCES = 39,    /* MATCHER_NOT_REFERENCES  */
  YYSYMBOL_MATCHER_SCORE_GREATER = 40,     /* MATCHER_SCORE_GREATER  */
  YYSYMBOL_MATCHER_SCORE_LOWER = 41,       /* MATCHER_SCORE_LOWER  */
  YYSYMBOL_MATCHER_HEADER = 42,            /* MATCHER_HEADER  */
  YYSYMBOL_MATCHER_NOT_HEADER = 43,        /* MATCHER_NOT_HEADER  */
  YYSYMBOL_MATCHER_HEADERS_PART = 44,      /* MATCHER_HEADERS_PART  */
  YYSYMBOL_MATCHER_NOT_HEADERS_PART = 45,  /* MATCHER_NOT_HEADERS_PART  */
  YYSYMBOL_MATCHER_MESSAGE = 46,           /* MATCHER_MESSAGE  */
  YYSYMBOL_MATCHER_HEADERS_CONT = 47,      /* MATCHER_HEADERS_CONT  */
  YYSYMBOL_MATCHER_NOT_HEADERS_CONT = 48,  /* MATCHER_NOT_HEADERS_CONT  */
  YYSYMBOL_MATCHER_NOT_MESSAGE = 49,       /* MATCHER_NOT_MESSAGE  */
  YYSYMBOL_MATCHER_BODY_PART = 50,         /* MATCHER_BODY_PART  */
  YYSYMBOL_MATCHER_NOT_BODY_PART = 51,     /* MATCHER_NOT_BODY_PART  */
  YYSYMBOL_MATCHER_TEST = 52,              /* MATCHER_TEST  */
  YYSYMBOL_MATCHER_NOT_TEST = 53,          /* MATCHER_NOT_TEST  */
  YYSYMBOL_MATCHER_MATCHCASE = 54,         /* MATCHER_MATCHCASE  */
  YYSYMBOL_MATCHER_MATCH = 55,             /* MATCHER_MATCH  */
  YYSYMBOL_MATCHER_REGEXPCASE = 56,        /* MATCHER_REGEXPCASE  */
  YYSYMBOL_MATCHER_REGEXP = 57,            /* MATCHER_REGEXP  */
  YYSYMBOL_MATCHER_SCORE = 58,             /* MATCHER_SCORE  */
  YYSYMBOL_MATCHER_MOVE = 59,              /* MATCHER_MOVE  */
  YYSYMBOL_MATCHER_FOUND_IN_ADDRESSBOOK = 60, /* MATCHER_FOUND_IN_ADDRESSBOOK  */
  YYSYMBOL_MATCHER_NOT_FOUND_IN_ADDRESSBOOK = 61, /* MATCHER_NOT_FOUND_IN_ADDRESSBOOK  */
  YYSYMBOL_MATCHER_IN = 62,                /* MATCHER_IN  */
  YYSYMBOL_MATCHER_COPY = 63,              /* MATCHER_COPY  */
  YYSYMBOL_MATCHER_DELETE = 64,            /* MATCHER_DELETE  */
  YYSYMBOL_MATCHER_MARK = 65,              /* MATCHER_MARK  */
  YYSYMBOL_MATCHER_UNMARK = 66,            /* MATCHER_UNMARK  */
  YYSYMBOL_MATCHER_LOCK = 67,              /* MATCHER_LOCK  */
  YYSYMBOL_MATCHER_UNLOCK = 68,            /* MATCHER_UNLOCK  */
  YYSYMBOL_MATCHER_EXECUTE = 69,           /* MATCHER_EXECUTE  */
  YYSYMBOL_MATCHER_MARK_AS_READ = 70,      /* MATCHER_MARK_AS_READ  */
  YYSYMBOL_MATCHER_MARK_AS_UNREAD = 71,    /* MATCHER_MARK_AS_UNREAD  */
  YYSYMBOL_MATCHER_FORWARD = 72,           /* MATCHER_FORWARD  */
  YYSYMBOL_MATCHER_MARK_AS_SPAM = 73,      /* MATCHER_MARK_AS_SPAM  */
  YYSYMBOL_MATCHER_MARK_AS_HAM = 74,       /* MATCHER_MARK_AS_HAM  */
  YYSYMBOL_MATCHER_FORWARD_AS_ATTACHMENT = 75, /* MATCHER_FORWARD_AS_ATTACHMENT  */
  YYSYMBOL_MATCHER_EOL = 76,               /* MATCHER_EOL  */
  YYSYMBOL_MATCHER_OR = 77,                /* MATCHER_OR  */
  YYSYMBOL_MATCHER_AND = 78,               /* MATCHER_AND  */
  YYSYMBOL_MATCHER_COLOR = 79,             /* MATCHER_COLOR  */
  YYSYMBOL_MATCHER_SCORE_EQUAL = 80,       /* MATCHER_SCORE_EQUAL  */
  YYSYMBOL_MATCHER_REDIRECT = 81,          /* MATCHER_REDIRECT  */
  YYSYMBOL_MATCHER_SIZE_GREATER = 82,      /* MATCHER_SIZE_GREATER  */
  YYSYMBOL_MATCHER_SIZE_SMALLER = 83,      /* MATCHER_SIZE_SMALLER  */
  YYSYMBOL_MATCHER_SIZE_EQUAL = 84,        /* MATCHER_SIZE_EQUAL  */
  YYSYMBOL_MATCHER_LOCKED = 85,            /* MATCHER_LOCKED  */
  YYSYMBOL_MATCHER_NOT_LOCKED = 86,        /* MATCHER_NOT_LOCKED  */
  YYSYMBOL_MATCHER_PARTIAL = 87,           /* MATCHER_PARTIAL  */
  YYSYMBOL_MATCHER_NOT_PARTIAL = 88,       /* MATCHER_NOT_PARTIAL  */
  YYSYMBOL_MATCHER_COLORLABEL = 89,        /* MATCHER_COLORLABEL  */
  YYSYMBOL_MATCHER_NOT_COLORLABEL = 90,    /* MATCHER_NOT_COLORLABEL  */
  YYSYMBOL_MATCHER_IGNORE_THREAD = 91,     /* MATCHER_IGNORE_THREAD  */
  YYSYMBOL_MATCHER_NOT_IGNORE_THREAD = 92, /* MATCHER_NOT_IGNORE_THREAD  */
  YYSYMBOL_MATCHER_WATCH_THREAD = 93,      /* MATCHER_WATCH_THREAD  */
  YYSYMBOL_MATCHER_NOT_WATCH_THREAD = 94,  /* MATCHER_NOT_WATCH_THREAD  */
  YYSYMBOL_MATCHER_CHANGE_SCORE = 95,      /* MATCHER_CHANGE_SCORE  */
  YYSYMBOL_MATCHER_SET_SCORE = 96,         /* MATCHER_SET_SCORE  */
  YYSYMBOL_MATCHER_ADD_TO_ADDRESSBOOK = 97, /* MATCHER_ADD_TO_ADDRESSBOOK  */
  YYSYMBOL_MATCHER_STOP = 98,              /* MATCHER_STOP  */
  YYSYMBOL_MATCHER_HIDE = 99,              /* MATCHER_HIDE  */
  YYSYMBOL_MATCHER_IGNORE = 100,           /* MATCHER_IGNORE  */
  YYSYMBOL_MATCHER_WATCH = 101,            /* MATCHER_WATCH  */
  YYSYMBOL_MATCHER_SPAM = 102,             /* MATCHER_SPAM  */
  YYSYMBOL_MATCHER_NOT_SPAM = 103,         /* MATCHER_NOT_SPAM  */
  YYSYMBOL_MATCHER_HAS_ATTACHMENT = 104,   /* MATCHER_HAS_ATTACHMENT  */
  YYSYMBOL_MATCHER_HAS_NO_ATTACHMENT = 105, /* MATCHER_HAS_NO_ATTACHMENT  */
  YYSYMBOL_MATCHER_SIGNED = 106,           /* MATCHER_SIGNED  */
  YYSYMBOL_MATCHER_NOT_SIGNED = 107,       /* MATCHER_NOT_SIGNED  */
  YYSYMBOL_MATCHER_TAG = 108,              /* MATCHER_TAG  */
  YYSYMBOL_MATCHER_NOT_TAG = 109,          /* MATCHER_NOT_TAG  */
  YYSYMBOL_MATCHER_SET_TAG = 110,          /* MATCHER_SET_TAG  */
  YYSYMBOL_MATCHER_UNSET_TAG = 111,        /* MATCHER_UNSET_TAG  */
  YYSYMBOL_MATCHER_TAGGED = 112,           /* MATCHER_TAGGED  */
  YYSYMBOL_MATCHER_NOT_TAGGED = 113,       /* MATCHER_NOT_TAGGED  */
  YYSYMBOL_MATCHER_CLEAR_TAGS = 114,       /* MATCHER_CLEAR_TAGS  */
  YYSYMBOL_MATCHER_ENABLED = 115,          /* MATCHER_ENABLED  */
  YYSYMBOL_MATCHER_DISABLED = 116,         /* MATCHER_DISABLED  */
  YYSYMBOL_MATCHER_RULENAME = 117,         /* MATCHER_RULENAME  */
  YYSYMBOL_MATCHER_ACCOUNT = 118,          /* MATCHER_ACCOUNT  */
  YYSYMBOL_MATCHER_STRING = 119,           /* MATCHER_STRING  */
  YYSYMBOL_MATCHER_SECTION = 120,          /* MATCHER_SECTION  */
  YYSYMBOL_MATCHER_INTEGER = 121,          /* MATCHER_INTEGER  */
  YYSYMBOL_YYACCEPT = 122,                 /* $accept  */
  YYSYMBOL_file = 123,                     /* file  */
  YYSYMBOL_124_1 = 124,                    /* $@1  */
  YYSYMBOL_file_line_list = 125,           /* file_line_list  */
  YYSYMBOL_file_line = 126,                /* file_line  */
  YYSYMBOL_127_2 = 127,                    /* $@2  */
  YYSYMBOL_section_notification = 128,     /* section_notification  */
  YYSYMBOL_instruction = 129,              /* instruction  */
  YYSYMBOL_enabled = 130,                  /* enabled  */
  YYSYMBOL_name = 131,                     /* name  */
  YYSYMBOL_account = 132,                  /* account  */
  YYSYMBOL_filtering = 133,                /* filtering  */
  YYSYMBOL_filtering_action_list = 134,    /* filtering_action_list  */
  YYSYMBOL_filtering_action_b = 135,       /* filtering_action_b  */
  YYSYMBOL_match_type = 136,               /* match_type  */
  YYSYMBOL_condition = 137,                /* condition  */
  YYSYMBOL_condition_list = 138,           /* condition_list  */
  YYSYMBOL_bool_op = 139,                  /* bool_op  */
  YYSYMBOL_one_condition = 140,            /* one_condition  */
  YYSYMBOL_141_3 = 141,                    /* $@3  */
  YYSYMBOL_142_4 = 142,                    /* $@4  */
  YYSYMBOL_143_5 = 143,                    /* $@5  */
  YYSYMBOL_144_6 = 144,                    /* $@6  */
  YYSYMBOL_filtering_action = 145,         /* filtering_action  */
  YYSYMBOL_146_7 = 146                     /* $@7  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   751

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  122
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  25
/* YYNRULES -- Number of rules.  */
#define YYNRULES  148
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  261

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   376


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   376,   376,   376,   384,   386,   390,   392,   392,   394,
     400,   433,   434,   435,   436,   437,   438,   447,   456,   465,
     474,   483,   492,   496,   500,   507,   514,   521,   562,   563,
     567,   575,   579,   583,   587,   594,   602,   606,   614,   618,
     625,   632,   639,   646,   653,   660,   667,   674,   681,   688,
     695,   702,   709,   716,   723,   730,   737,   744,   751,   758,
     765,   772,   779,   786,   797,   808,   815,   822,   829,   836,
     845,   854,   863,   872,   881,   890,   899,   908,   917,   926,
     935,   944,   951,   958,   967,   976,   985,   994,  1005,  1016,
    1025,  1034,  1043,  1052,  1061,  1070,  1079,  1088,  1097,  1106,
    1115,  1123,  1131,  1140,  1139,  1153,  1152,  1165,  1174,  1183,
    1192,  1202,  1201,  1215,  1214,  1227,  1236,  1245,  1254,  1263,
    1272,  1284,  1293,  1302,  1311,  1320,  1327,  1336,  1343,  1350,
    1357,  1364,  1371,  1378,  1385,  1392,  1399,  1411,  1423,  1435,
    1444,  1453,  1461,  1469,  1473,  1477,  1482,  1481,  1494
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "MATCHER_ALL",
  "MATCHER_UNREAD", "MATCHER_NOT_UNREAD", "MATCHER_NEW", "MATCHER_NOT_NEW",
  "MATCHER_MARKED", "MATCHER_NOT_MARKED", "MATCHER_DELETED",
  "MATCHER_NOT_DELETED", "MATCHER_REPLIED", "MATCHER_NOT_REPLIED",
  "MATCHER_FORWARDED", "MATCHER_NOT_FORWARDED", "MATCHER_SUBJECT",
  "MATCHER_NOT_SUBJECT", "MATCHER_FROM", "MATCHER_NOT_FROM", "MATCHER_TO",
  "MATCHER_NOT_TO", "MATCHER_CC", "MATCHER_NOT_CC", "MATCHER_TO_OR_CC",
  "MATCHER_NOT_TO_AND_NOT_CC", "MATCHER_AGE_GREATER", "MATCHER_AGE_LOWER",
  "MATCHER_NEWSGROUPS", "MATCHER_AGE_GREATER_HOURS",
  "MATCHER_AGE_LOWER_HOURS", "MATCHER_DATE_AFTER", "MATCHER_DATE_BEFORE",
  "MATCHER_NOT_NEWSGROUPS", "MATCHER_INREPLYTO", "MATCHER_NOT_INREPLYTO",
  "MATCHER_MESSAGEID", "MATCHER_NOT_MESSAGEID", "MATCHER_REFERENCES",
  "MATCHER_NOT_REFERENCES", "MATCHER_SCORE_GREATER", "MATCHER_SCORE_LOWER",
  "MATCHER_HEADER", "MATCHER_NOT_HEADER", "MATCHER_HEADERS_PART",
  "MATCHER_NOT_HEADERS_PART", "MATCHER_MESSAGE", "MATCHER_HEADERS_CONT",
  "MATCHER_NOT_HEADERS_CONT", "MATCHER_NOT_MESSAGE", "MATCHER_BODY_PART",
  "MATCHER_NOT_BODY_PART", "MATCHER_TEST", "MATCHER_NOT_TEST",
  "MATCHER_MATCHCASE", "MATCHER_MATCH", "MATCHER_REGEXPCASE",
  "MATCHER_REGEXP", "MATCHER_SCORE", "MATCHER_MOVE",
  "MATCHER_FOUND_IN_ADDRESSBOOK", "MATCHER_NOT_FOUND_IN_ADDRESSBOOK",
  "MATCHER_IN", "MATCHER_COPY", "MATCHER_DELETE", "MATCHER_MARK",
  "MATCHER_UNMARK", "MATCHER_LOCK", "MATCHER_UNLOCK", "MATCHER_EXECUTE",
  "MATCHER_MARK_AS_READ", "MATCHER_MARK_AS_UNREAD", "MATCHER_FORWARD",
  "MATCHER_MARK_AS_SPAM", "MATCHER_MARK_AS_HAM",
  "MATCHER_FORWARD_AS_ATTACHMENT", "MATCHER_EOL", "MATCHER_OR",
  "MATCHER_AND", "MATCHER_COLOR", "MATCHER_SCORE_EQUAL",
  "MATCHER_REDIRECT", "MATCHER_SIZE_GREATER", "MATCHER_SIZE_SMALLER",
  "MATCHER_SIZE_EQUAL", "MATCHER_LOCKED", "MATCHER_NOT_LOCKED",
  "MATCHER_PARTIAL", "MATCHER_NOT_PARTIAL", "MATCHER_COLORLABEL",
  "MATCHER_NOT_COLORLABEL", "MATCHER_IGNORE_THREAD",
  "MATCHER_NOT_IGNORE_THREAD", "MATCHER_WATCH_THREAD",
  "MATCHER_NOT_WATCH_THREAD", "MATCHER_CHANGE_SCORE", "MATCHER_SET_SCORE",
  "MATCHER_ADD_TO_ADDRESSBOOK", "MATCHER_STOP", "MATCHER_HIDE",
  "MATCHER_IGNORE", "MATCHER_WATCH", "MATCHER_SPAM", "MATCHER_NOT_SPAM",
  "MATCHER_HAS_ATTACHMENT", "MATCHER_HAS_NO_ATTACHMENT", "MATCHER_SIGNED",
  "MATCHER_NOT_SIGNED", "MATCHER_TAG", "MATCHER_NOT_TAG",
  "MATCHER_SET_TAG", "MATCHER_UNSET_TAG", "MATCHER_TAGGED",
  "MATCHER_NOT_TAGGED", "MATCHER_CLEAR_TAGS", "MATCHER_ENABLED",
  "MATCHER_DISABLED", "MATCHER_RULENAME", "MATCHER_ACCOUNT",
  "MATCHER_STRING", "MATCHER_SECTION", "MATCHER_INTEGER", "$accept",
  "file", "$@1", "file_line_list", "file_line", "$@2",
  "section_notification", "instruction", "enabled", "name", "account",
  "filtering", "filtering_action_list", "filtering_action_b", "match_type",
  "condition", "condition_list", "bool_op", "one_condition", "$@3", "$@4",
  "$@5", "$@6", "filtering_action", "$@7", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-229)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-8)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -229,    12,   237,  -229,   -63,   -61,  -229,   119,   406,  -229,
    -229,  -229,  -229,  -229,  -229,  -229,  -229,  -229,  -229,  -229,
    -229,  -229,  -229,  -229,  -229,  -229,     5,     5,     5,     5,
       5,     5,     5,     5,     5,     5,   -97,   -96,     5,   -95,
     -84,   -81,   -80,     5,     5,     5,     5,     5,     5,     5,
     -66,   -64,   -56,   -55,     5,     5,     5,     5,     5,     5,
       5,     5,   -54,   -53,   -52,   -51,   -49,   -48,   -40,  -229,
    -229,  -229,  -229,  -229,   -39,  -229,  -229,   -38,  -229,  -229,
     -35,  -229,   -32,   -31,   -29,   -28,   -26,   -25,  -229,  -229,
    -229,  -229,   -24,   -22,  -229,  -229,  -229,  -229,   -21,   -20,
     -37,  -229,  -229,  -229,  -229,  -229,  -229,  -229,  -229,  -229,
    -229,     5,     5,   -17,   -16,  -229,  -229,  -229,  -229,  -229,
     -15,   -14,  -229,   -50,   638,  -229,  -229,   -23,  -229,   -68,
    -229,  -229,  -229,  -229,  -229,  -229,   -13,   -11,   -10,    -9,
      -8,    -7,    -6,    -5,    -4,    -3,  -229,  -229,    -2,  -229,
    -229,  -229,  -229,    -1,     2,    55,    56,    57,    62,   173,
    -229,  -229,  -229,  -229,   174,   175,   180,   195,   196,   239,
     240,   241,  -229,  -229,  -229,  -229,  -229,  -229,  -229,  -229,
     242,   243,  -229,  -229,   244,  -229,  -229,  -229,  -229,  -229,
    -229,  -229,  -229,   245,   246,  -229,  -229,  -229,  -229,   522,
     -23,  -229,  -229,  -229,   638,  -229,  -229,  -229,  -229,  -229,
    -229,  -229,  -229,  -229,  -229,  -229,  -229,  -229,  -229,  -229,
    -229,  -229,  -229,     5,     5,  -229,  -229,  -229,  -229,  -229,
    -229,  -229,  -229,    19,    32,  -229,  -229,  -229,   248,  -229,
    -229,   638,   -23,    22,  -229,  -229,   249,   250,   251,   252,
    -229,   -23,    29,  -229,  -229,  -229,  -229,  -229,   280,  -229,
    -229
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,     1,     0,     0,     3,     0,     0,     6,
       9,    10,     4,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   127,
     128,   129,   130,   131,     0,   132,   133,     0,   134,   135,
       0,    22,     0,     0,     0,     0,     0,     0,    53,    54,
      61,    62,     0,     0,    65,    66,    67,    68,     0,     0,
       0,   148,   143,   144,   145,    55,    56,    57,    58,    59,
      60,     0,     0,     0,     0,    81,    82,   125,    23,    24,
       0,     0,     8,    17,    19,    18,    21,    29,    20,    35,
      37,    30,    31,    32,    33,    34,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    83,    84,     0,    85,
      86,    87,    88,     0,     0,     0,     0,     0,     0,     0,
      97,    98,   103,   105,     0,     0,     0,     0,     0,     0,
       0,     0,   119,   120,   141,   122,   111,   113,   126,   121,
       0,     0,   139,    99,     0,   100,   101,   102,    63,    64,
     140,   142,   146,     0,     0,   123,   124,    25,    26,     0,
       0,    28,    39,    38,     0,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    89,    90,    93,    94,    91,
      92,    95,    96,     0,     0,   107,   108,   115,   109,   110,
     116,   117,   118,     0,     0,   136,   137,   138,     0,    79,
      80,     0,     0,    16,    27,    36,     0,     0,     0,     0,
     147,     0,    14,    15,   104,   106,   112,   114,    12,    13,
      11
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -229,  -229,  -229,   365,  -229,  -229,  -229,  -229,  -229,   -70,
     176,  -228,    46,  -229,   -27,   167,  -229,  -229,   169,  -229,
    -229,  -229,  -229,  -229,  -229
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,     2,     6,     7,     8,     9,   122,   123,   124,
     125,   243,   244,   127,   136,   128,   129,   204,   130,   223,
     224,   233,   234,   131,   238
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     137,   138,   139,   140,   141,   142,   143,   144,   145,   202,
     203,   148,     3,    10,   252,    11,   153,   154,   155,   156,
     157,   158,   159,   258,   146,   147,   149,   164,   165,   166,
     167,   168,   169,   170,   171,    64,    65,   150,   151,   152,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,   199,   126,   160,    82,   161,    84,   132,
     133,   134,   135,   162,   163,   172,   173,   120,   175,   174,
     176,   177,    98,    99,   100,   101,   102,   103,   104,   178,
     179,   248,   192,   180,   193,   194,   181,   113,   114,   182,
     183,   117,   184,   185,   249,   186,   187,   188,   253,   189,
     190,   191,   195,   196,   197,   259,   205,   198,   206,   207,
     208,   209,   210,   211,   212,   213,   214,   215,   216,    -5,
       4,   217,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,   201,   218,   219,   220,    -7,    -7,    -7,
      -7,   221,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,   246,   247,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,     4,     5,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,   200,   222,   225,   226,    -7,    -7,    -7,    -7,   227,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,   228,   229,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,    -7,
      -7,    -7,    -7,    -7,    -7,    -7,   260,     5,   230,   231,
     232,   235,   236,   237,   239,   240,   242,   250,   254,   255,
     256,   257,    12,   245,     0,   241,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   251,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
       0,     0,     0,     0,    64,    65,    66,    67,     0,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,     0,     0,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,     0,     0,     0,     0,
       0,     0,    66,    67,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    83,     0,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,     0,     0,     0,
       0,     0,     0,     0,   105,   106,   107,   108,   109,   110,
     111,   112,     0,     0,   115,   116,     0,     0,     0,     0,
     121,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,     0,     0,     0,     0,     0,     0,    66,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    83,     0,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,     0,     0,     0,     0,     0,     0,     0,
     105,   106,   107,   108,   109,   110,   111,   112,     0,     0,
     115,   116
};

static const yytype_int16 yycheck[] =
{
      27,    28,    29,    30,    31,    32,    33,    34,    35,    77,
      78,    38,     0,    76,   242,    76,    43,    44,    45,    46,
      47,    48,    49,   251,   121,   121,   121,    54,    55,    56,
      57,    58,    59,    60,    61,    58,    59,   121,   119,   119,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,   123,     8,   121,    79,   121,    81,    54,
      55,    56,    57,   119,   119,   119,   119,   117,   119,   121,
     119,   119,    95,    96,    97,    98,    99,   100,   101,   119,
     119,    62,   119,   121,   111,   112,   121,   110,   111,   121,
     121,   114,   121,   121,    62,   121,   121,   121,    76,   121,
     121,   121,   119,   119,   119,    76,   119,   121,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,     0,
       1,   119,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,   127,   119,   119,   119,    58,    59,    60,
      61,   119,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,   223,   224,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,     1,   120,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,   124,   119,   119,   119,    58,    59,    60,    61,   119,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,   119,   119,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    76,   120,   119,   119,
     119,   119,   119,   119,   119,   119,   199,   119,   119,   119,
     119,   119,     7,   204,    -1,   199,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   241,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      -1,    -1,    -1,    -1,    58,    59,    60,    61,    -1,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    -1,    -1,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    60,    61,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    80,    -1,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   102,   103,   104,   105,   106,   107,
     108,   109,    -1,    -1,   112,   113,    -1,    -1,    -1,    -1,
     118,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    -1,    -1,    -1,    -1,    -1,    -1,    60,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,    -1,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     102,   103,   104,   105,   106,   107,   108,   109,    -1,    -1,
     112,   113
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   123,   124,     0,     1,   120,   125,   126,   127,   128,
      76,    76,   125,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    58,    59,    60,    61,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   129,   130,   131,   132,   134,   135,   137,   138,
     140,   145,    54,    55,    56,    57,   136,   136,   136,   136,
     136,   136,   136,   136,   136,   136,   121,   121,   136,   121,
     121,   119,   119,   136,   136,   136,   136,   136,   136,   136,
     121,   121,   119,   119,   136,   136,   136,   136,   136,   136,
     136,   136,   119,   119,   121,   119,   119,   119,   119,   119,
     121,   121,   121,   121,   121,   121,   121,   121,   121,   121,
     121,   121,   119,   136,   136,   119,   119,   119,   121,   131,
     137,   134,    77,    78,   139,   119,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   119,   119,   119,   119,
     119,   119,   119,   141,   142,   119,   119,   119,   119,   119,
     119,   119,   119,   143,   144,   119,   119,   119,   146,   119,
     119,   132,   137,   133,   134,   140,   136,   136,    62,    62,
     119,   137,   133,    76,   119,   119,   119,   119,   133,    76,
      76
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   122,   124,   123,   125,   125,   126,   127,   126,   126,
     128,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   130,   130,   131,   132,   133,   134,   134,
     135,   136,   136,   136,   136,   137,   138,   138,   139,   139,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   140,   140,   140,   140,   140,   140,   140,
     140,   140,   140,   141,   140,   142,   140,   140,   140,   140,
     140,   143,   140,   144,   140,   140,   140,   140,   140,   140,
     140,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   146,   145,   145
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     1,     1,     0,     2,     2,
       2,     6,     5,     5,     4,     4,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     2,     2,     2,     2,     2,     2,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     2,     2,
       2,     2,     2,     0,     5,     0,     5,     3,     3,     3,
       3,     0,     5,     0,     5,     3,     3,     3,     3,     2,
       2,     2,     2,     2,     2,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     2,
       2,     2,     2,     1,     1,     1,     0,     4,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* $@1: %empty  */
#line 376 "matcher_parser_parse.y"
{
	if (matcher_parse_op == MATCHER_PARSE_FILE) {
		prefs_filtering = &pre_global_processing;
	}
}
#line 2131 "matcher_parser_parse.c"
    break;

  case 7: /* $@2: %empty  */
#line 392 "matcher_parser_parse.y"
{ action_list = NULL; }
#line 2137 "matcher_parser_parse.c"
    break;

  case 9: /* file_line: error MATCHER_EOL  */
#line 395 "matcher_parser_parse.y"
{
	yyerrok;
}
#line 2145 "matcher_parser_parse.c"
    break;

  case 10: /* section_notification: MATCHER_SECTION MATCHER_EOL  */
#line 401 "matcher_parser_parse.y"
{
	gchar *folder = (yyvsp[-1].str);
	FolderItem *item = NULL;

	if (matcher_parse_op == MATCHER_PARSE_FILE) {
                enable_compatibility = 0;
		if (!strcmp(folder, "global")) {
                        /* backward compatibility */
                        enable_compatibility = 1;
                }
		else if (!strcmp(folder, "preglobal")) {
			prefs_filtering = &pre_global_processing;
                }
		else if (!strcmp(folder, "postglobal")) {
			prefs_filtering = &post_global_processing;
                }
		else if (!strcmp(folder, "filtering")) {
                        prefs_filtering = &filtering_rules;
		}
                else {
			item = folder_find_item_from_identifier(folder);
			if (item != NULL) {
				prefs_filtering = &item->prefs->processing;
			} else {
				prefs_filtering = NULL;
			}
		}
	}
}
#line 2179 "matcher_parser_parse.c"
    break;

  case 16: /* instruction: name condition filtering  */
#line 439 "matcher_parser_parse.y"
{
	if (matcher_parse_op == MATCHER_PARSE_NO_EOL)
		YYACCEPT;
	else {
		matcher_parsererror("parse error [no eol]");
		YYERROR;
	}
}
#line 2192 "matcher_parser_parse.c"
    break;

  case 17: /* instruction: enabled  */
#line 448 "matcher_parser_parse.y"
{
	if (matcher_parse_op == MATCHER_PARSE_ENABLED)
		YYACCEPT;
	else {
		matcher_parsererror("parse error [enabled]");
		YYERROR;
	}
}
#line 2205 "matcher_parser_parse.c"
    break;

  case 18: /* instruction: account  */
#line 457 "matcher_parser_parse.y"
{
	if (matcher_parse_op == MATCHER_PARSE_ACCOUNT)
		YYACCEPT;
	else {
		matcher_parsererror("parse error [account]");
		YYERROR;
	}
}
#line 2218 "matcher_parser_parse.c"
    break;

  case 19: /* instruction: name  */
#line 466 "matcher_parser_parse.y"
{
	if (matcher_parse_op == MATCHER_PARSE_NAME)
		YYACCEPT;
	else {
		matcher_parsererror("parse error [name]");
		YYERROR;
	}
}
#line 2231 "matcher_parser_parse.c"
    break;

  case 20: /* instruction: condition  */
#line 475 "matcher_parser_parse.y"
{
	if (matcher_parse_op == MATCHER_PARSE_CONDITION)
		YYACCEPT;
	else {
		matcher_parsererror("parse error [condition]");
		YYERROR;
	}
}
#line 2244 "matcher_parser_parse.c"
    break;

  case 21: /* instruction: filtering_action_list  */
#line 484 "matcher_parser_parse.y"
{
	if (matcher_parse_op == MATCHER_PARSE_FILTERING_ACTION)
		YYACCEPT;
	else {
		matcher_parsererror("parse error [filtering action]");
		YYERROR;
	}
}
#line 2257 "matcher_parser_parse.c"
    break;

  case 23: /* enabled: MATCHER_ENABLED  */
#line 497 "matcher_parser_parse.y"
{
	enabled = TRUE;
}
#line 2265 "matcher_parser_parse.c"
    break;

  case 24: /* enabled: MATCHER_DISABLED  */
#line 501 "matcher_parser_parse.y"
{
	enabled = FALSE;
}
#line 2273 "matcher_parser_parse.c"
    break;

  case 25: /* name: MATCHER_RULENAME MATCHER_STRING  */
#line 508 "matcher_parser_parse.y"
{
	name = g_strdup((yyvsp[0].str));
}
#line 2281 "matcher_parser_parse.c"
    break;

  case 26: /* account: MATCHER_ACCOUNT MATCHER_INTEGER  */
#line 515 "matcher_parser_parse.y"
{
	account_id = strtol((yyvsp[0].str), NULL, 10);
}
#line 2289 "matcher_parser_parse.c"
    break;

  case 27: /* filtering: filtering_action_list  */
#line 522 "matcher_parser_parse.y"
{
	filtering = filteringprop_new(enabled, name, account_id, cond, action_list);
	enabled = TRUE;
	account_id = 0;
	g_free(name);
	name = NULL;
        if (enable_compatibility) {
                prefs_filtering = &filtering_rules;
                if (action_list != NULL) {
                        FilteringAction * first_action;
                        
                        first_action = action_list->data;
                        
                        if (first_action->type == MATCHACTION_CHANGE_SCORE)
                                prefs_filtering = &pre_global_processing;
                }
        }
        
	cond = NULL;
	action_list = NULL;
        
	if ((matcher_parse_op == MATCHER_PARSE_FILE) &&
            (prefs_filtering != NULL)) {
		*prefs_filtering = g_slist_append(*prefs_filtering,
						  filtering);
		filtering = NULL;
	} else if (!filtering_ptr_externally_managed) {
		/* If filtering_ptr_externally_managed was TRUE, it
		 * would mean that some function higher in the stack is
		 * interested in the data "filtering" is pointing at, so
		 * we would not free it. That function has to free it itself.
		 * At the time of writing this, the only function that
		 * does this is matcher_parser_get_filtering(). */
		filteringprop_free(filtering);
		filtering = NULL;
	}
}
#line 2331 "matcher_parser_parse.c"
    break;

  case 30: /* filtering_action_b: filtering_action  */
#line 568 "matcher_parser_parse.y"
{
        action_list = g_slist_append(action_list, action);
        action = NULL;
}
#line 2340 "matcher_parser_parse.c"
    break;

  case 31: /* match_type: MATCHER_MATCHCASE  */
#line 576 "matcher_parser_parse.y"
{
	match_type = MATCHTYPE_MATCHCASE;
}
#line 2348 "matcher_parser_parse.c"
    break;

  case 32: /* match_type: MATCHER_MATCH  */
#line 580 "matcher_parser_parse.y"
{
	match_type = MATCHTYPE_MATCH;
}
#line 2356 "matcher_parser_parse.c"
    break;

  case 33: /* match_type: MATCHER_REGEXPCASE  */
#line 584 "matcher_parser_parse.y"
{
	match_type = MATCHTYPE_REGEXPCASE;
}
#line 2364 "matcher_parser_parse.c"
    break;

  case 34: /* match_type: MATCHER_REGEXP  */
#line 588 "matcher_parser_parse.y"
{
	match_type = MATCHTYPE_REGEXP;
}
#line 2372 "matcher_parser_parse.c"
    break;

  case 35: /* condition: condition_list  */
#line 595 "matcher_parser_parse.y"
{
	cond = matcherlist_new(matchers_list, (bool_op == MATCHERBOOL_AND));
	matchers_list = NULL;
}
#line 2381 "matcher_parser_parse.c"
    break;

  case 36: /* condition_list: condition_list bool_op one_condition  */
#line 603 "matcher_parser_parse.y"
{
	matchers_list = g_slist_append(matchers_list, prop);
}
#line 2389 "matcher_parser_parse.c"
    break;

  case 37: /* condition_list: one_condition  */
#line 607 "matcher_parser_parse.y"
{
	matchers_list = NULL;
	matchers_list = g_slist_append(matchers_list, prop);
}
#line 2398 "matcher_parser_parse.c"
    break;

  case 38: /* bool_op: MATCHER_AND  */
#line 615 "matcher_parser_parse.y"
{
	bool_op = MATCHERBOOL_AND;
}
#line 2406 "matcher_parser_parse.c"
    break;

  case 39: /* bool_op: MATCHER_OR  */
#line 619 "matcher_parser_parse.y"
{
	bool_op = MATCHERBOOL_OR;
}
#line 2414 "matcher_parser_parse.c"
    break;

  case 40: /* one_condition: MATCHER_ALL  */
#line 626 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_ALL;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2425 "matcher_parser_parse.c"
    break;

  case 41: /* one_condition: MATCHER_UNREAD  */
#line 633 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_UNREAD;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2436 "matcher_parser_parse.c"
    break;

  case 42: /* one_condition: MATCHER_NOT_UNREAD  */
#line 640 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_UNREAD;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2447 "matcher_parser_parse.c"
    break;

  case 43: /* one_condition: MATCHER_NEW  */
#line 647 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_NEW;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2458 "matcher_parser_parse.c"
    break;

  case 44: /* one_condition: MATCHER_NOT_NEW  */
#line 654 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_NEW;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2469 "matcher_parser_parse.c"
    break;

  case 45: /* one_condition: MATCHER_MARKED  */
#line 661 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_MARKED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2480 "matcher_parser_parse.c"
    break;

  case 46: /* one_condition: MATCHER_NOT_MARKED  */
#line 668 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_MARKED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2491 "matcher_parser_parse.c"
    break;

  case 47: /* one_condition: MATCHER_DELETED  */
#line 675 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_DELETED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2502 "matcher_parser_parse.c"
    break;

  case 48: /* one_condition: MATCHER_NOT_DELETED  */
#line 682 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_DELETED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2513 "matcher_parser_parse.c"
    break;

  case 49: /* one_condition: MATCHER_REPLIED  */
#line 689 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_REPLIED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2524 "matcher_parser_parse.c"
    break;

  case 50: /* one_condition: MATCHER_NOT_REPLIED  */
#line 696 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_REPLIED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2535 "matcher_parser_parse.c"
    break;

  case 51: /* one_condition: MATCHER_FORWARDED  */
#line 703 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_FORWARDED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2546 "matcher_parser_parse.c"
    break;

  case 52: /* one_condition: MATCHER_NOT_FORWARDED  */
#line 710 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_FORWARDED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2557 "matcher_parser_parse.c"
    break;

  case 53: /* one_condition: MATCHER_LOCKED  */
#line 717 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_LOCKED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2568 "matcher_parser_parse.c"
    break;

  case 54: /* one_condition: MATCHER_NOT_LOCKED  */
#line 724 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_LOCKED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2579 "matcher_parser_parse.c"
    break;

  case 55: /* one_condition: MATCHER_SPAM  */
#line 731 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_SPAM;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2590 "matcher_parser_parse.c"
    break;

  case 56: /* one_condition: MATCHER_NOT_SPAM  */
#line 738 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_SPAM;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2601 "matcher_parser_parse.c"
    break;

  case 57: /* one_condition: MATCHER_HAS_ATTACHMENT  */
#line 745 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_HAS_ATTACHMENT;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2612 "matcher_parser_parse.c"
    break;

  case 58: /* one_condition: MATCHER_HAS_NO_ATTACHMENT  */
#line 752 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_HAS_NO_ATTACHMENT;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2623 "matcher_parser_parse.c"
    break;

  case 59: /* one_condition: MATCHER_SIGNED  */
#line 759 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_SIGNED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2634 "matcher_parser_parse.c"
    break;

  case 60: /* one_condition: MATCHER_NOT_SIGNED  */
#line 766 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_SIGNED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2645 "matcher_parser_parse.c"
    break;

  case 61: /* one_condition: MATCHER_PARTIAL  */
#line 773 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_PARTIAL;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2656 "matcher_parser_parse.c"
    break;

  case 62: /* one_condition: MATCHER_NOT_PARTIAL  */
#line 780 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_PARTIAL;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2667 "matcher_parser_parse.c"
    break;

  case 63: /* one_condition: MATCHER_COLORLABEL MATCHER_INTEGER  */
#line 787 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gint value = 0;

	criteria = MATCHCRITERIA_COLORLABEL;
	value = strtol((yyvsp[0].str), NULL, 10);
	if (value < 0) value = 0;
	else if (value > COLORLABELS) value = COLORLABELS;
	prop = matcherprop_new(criteria, NULL, 0, NULL, value);
}
#line 2682 "matcher_parser_parse.c"
    break;

  case 64: /* one_condition: MATCHER_NOT_COLORLABEL MATCHER_INTEGER  */
#line 798 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gint value = 0;

	criteria = MATCHCRITERIA_NOT_COLORLABEL;
	value = strtol((yyvsp[0].str), NULL, 0);
	if (value < 0) value = 0;
	else if (value > COLORLABELS) value = COLORLABELS;
	prop = matcherprop_new(criteria, NULL, 0, NULL, value);
}
#line 2697 "matcher_parser_parse.c"
    break;

  case 65: /* one_condition: MATCHER_IGNORE_THREAD  */
#line 809 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_IGNORE_THREAD;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2708 "matcher_parser_parse.c"
    break;

  case 66: /* one_condition: MATCHER_NOT_IGNORE_THREAD  */
#line 816 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_IGNORE_THREAD;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2719 "matcher_parser_parse.c"
    break;

  case 67: /* one_condition: MATCHER_WATCH_THREAD  */
#line 823 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_WATCH_THREAD;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2730 "matcher_parser_parse.c"
    break;

  case 68: /* one_condition: MATCHER_NOT_WATCH_THREAD  */
#line 830 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_WATCH_THREAD;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2741 "matcher_parser_parse.c"
    break;

  case 69: /* one_condition: MATCHER_SUBJECT match_type MATCHER_STRING  */
#line 837 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_SUBJECT;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 2754 "matcher_parser_parse.c"
    break;

  case 70: /* one_condition: MATCHER_NOT_SUBJECT match_type MATCHER_STRING  */
#line 846 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_NOT_SUBJECT;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 2767 "matcher_parser_parse.c"
    break;

  case 71: /* one_condition: MATCHER_FROM match_type MATCHER_STRING  */
#line 855 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_FROM;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 2780 "matcher_parser_parse.c"
    break;

  case 72: /* one_condition: MATCHER_NOT_FROM match_type MATCHER_STRING  */
#line 864 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_NOT_FROM;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 2793 "matcher_parser_parse.c"
    break;

  case 73: /* one_condition: MATCHER_TO match_type MATCHER_STRING  */
#line 873 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_TO;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 2806 "matcher_parser_parse.c"
    break;

  case 74: /* one_condition: MATCHER_NOT_TO match_type MATCHER_STRING  */
#line 882 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_NOT_TO;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 2819 "matcher_parser_parse.c"
    break;

  case 75: /* one_condition: MATCHER_CC match_type MATCHER_STRING  */
#line 891 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_CC;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 2832 "matcher_parser_parse.c"
    break;

  case 76: /* one_condition: MATCHER_NOT_CC match_type MATCHER_STRING  */
#line 900 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_NOT_CC;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 2845 "matcher_parser_parse.c"
    break;

  case 77: /* one_condition: MATCHER_TO_OR_CC match_type MATCHER_STRING  */
#line 909 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_TO_OR_CC;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 2858 "matcher_parser_parse.c"
    break;

  case 78: /* one_condition: MATCHER_NOT_TO_AND_NOT_CC match_type MATCHER_STRING  */
#line 918 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_NOT_TO_AND_NOT_CC;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 2871 "matcher_parser_parse.c"
    break;

  case 79: /* one_condition: MATCHER_TAG match_type MATCHER_STRING  */
#line 927 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_TAG;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 2884 "matcher_parser_parse.c"
    break;

  case 80: /* one_condition: MATCHER_NOT_TAG match_type MATCHER_STRING  */
#line 936 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_NOT_TAG;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 2897 "matcher_parser_parse.c"
    break;

  case 81: /* one_condition: MATCHER_TAGGED  */
#line 945 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_TAGGED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2908 "matcher_parser_parse.c"
    break;

  case 82: /* one_condition: MATCHER_NOT_TAGGED  */
#line 952 "matcher_parser_parse.y"
{
	gint criteria = 0;

	criteria = MATCHCRITERIA_NOT_TAGGED;
	prop = matcherprop_new(criteria, NULL, 0, NULL, 0);
}
#line 2919 "matcher_parser_parse.c"
    break;

  case 83: /* one_condition: MATCHER_AGE_GREATER MATCHER_INTEGER  */
#line 959 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gint value = 0;

	criteria = MATCHCRITERIA_AGE_GREATER;
	value = strtol((yyvsp[0].str), NULL, 0);
	prop = matcherprop_new(criteria, NULL, 0, NULL, value);
}
#line 2932 "matcher_parser_parse.c"
    break;

  case 84: /* one_condition: MATCHER_AGE_LOWER MATCHER_INTEGER  */
#line 968 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gint value = 0;

	criteria = MATCHCRITERIA_AGE_LOWER;
	value = strtol((yyvsp[0].str), NULL, 0);
	prop = matcherprop_new(criteria, NULL, 0, NULL, value);
}
#line 2945 "matcher_parser_parse.c"
    break;

  case 85: /* one_condition: MATCHER_AGE_GREATER_HOURS MATCHER_INTEGER  */
#line 977 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gint value = 0;

	criteria = MATCHCRITERIA_AGE_GREATER_HOURS;
	value = strtol((yyvsp[0].str), NULL, 0);
	prop = matcherprop_new(criteria, NULL, 0, NULL, value);
}
#line 2958 "matcher_parser_parse.c"
    break;

  case 86: /* one_condition: MATCHER_AGE_LOWER_HOURS MATCHER_INTEGER  */
#line 986 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gint value = 0;

	criteria = MATCHCRITERIA_AGE_LOWER_HOURS;
	value = strtol((yyvsp[0].str), NULL, 0);
	prop = matcherprop_new(criteria, NULL, 0, NULL, value);
}
#line 2971 "matcher_parser_parse.c"
    break;

  case 87: /* one_condition: MATCHER_DATE_AFTER MATCHER_STRING  */
#line 995 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;
	time_t value;

	criteria = MATCHCRITERIA_DATE_AFTER;
	expr = (yyvsp[0].str);
	value = procheader_date_parse(NULL, expr, 0);
	prop = matcherprop_new(criteria, NULL, 0, expr, value);
}
#line 2986 "matcher_parser_parse.c"
    break;

  case 88: /* one_condition: MATCHER_DATE_BEFORE MATCHER_STRING  */
#line 1006 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;
	time_t value;

	criteria = MATCHCRITERIA_DATE_BEFORE;
	expr = (yyvsp[0].str);
	value = procheader_date_parse(NULL, expr, 0);
	prop = matcherprop_new(criteria, NULL, 0, expr, value);
}
#line 3001 "matcher_parser_parse.c"
    break;

  case 89: /* one_condition: MATCHER_NEWSGROUPS match_type MATCHER_STRING  */
#line 1017 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_NEWSGROUPS;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 3014 "matcher_parser_parse.c"
    break;

  case 90: /* one_condition: MATCHER_NOT_NEWSGROUPS match_type MATCHER_STRING  */
#line 1026 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_NOT_NEWSGROUPS;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 3027 "matcher_parser_parse.c"
    break;

  case 91: /* one_condition: MATCHER_MESSAGEID match_type MATCHER_STRING  */
#line 1035 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_MESSAGEID;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 3040 "matcher_parser_parse.c"
    break;

  case 92: /* one_condition: MATCHER_NOT_MESSAGEID match_type MATCHER_STRING  */
#line 1044 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_NOT_MESSAGEID;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 3053 "matcher_parser_parse.c"
    break;

  case 93: /* one_condition: MATCHER_INREPLYTO match_type MATCHER_STRING  */
#line 1053 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_INREPLYTO;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 3066 "matcher_parser_parse.c"
    break;

  case 94: /* one_condition: MATCHER_NOT_INREPLYTO match_type MATCHER_STRING  */
#line 1062 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_NOT_INREPLYTO;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 3079 "matcher_parser_parse.c"
    break;

  case 95: /* one_condition: MATCHER_REFERENCES match_type MATCHER_STRING  */
#line 1071 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_REFERENCES;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 3092 "matcher_parser_parse.c"
    break;

  case 96: /* one_condition: MATCHER_NOT_REFERENCES match_type MATCHER_STRING  */
#line 1080 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_NOT_REFERENCES;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 3105 "matcher_parser_parse.c"
    break;

  case 97: /* one_condition: MATCHER_SCORE_GREATER MATCHER_INTEGER  */
#line 1089 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gint value = 0;

	criteria = MATCHCRITERIA_SCORE_GREATER;
	value = strtol((yyvsp[0].str), NULL, 0);
	prop = matcherprop_new(criteria, NULL, 0, NULL, value);
}
#line 3118 "matcher_parser_parse.c"
    break;

  case 98: /* one_condition: MATCHER_SCORE_LOWER MATCHER_INTEGER  */
#line 1098 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gint value = 0;

	criteria = MATCHCRITERIA_SCORE_LOWER;
	value = strtol((yyvsp[0].str), NULL, 0);
	prop = matcherprop_new(criteria, NULL, 0, NULL, value);
}
#line 3131 "matcher_parser_parse.c"
    break;

  case 99: /* one_condition: MATCHER_SCORE_EQUAL MATCHER_INTEGER  */
#line 1107 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gint value = 0;

	criteria = MATCHCRITERIA_SCORE_EQUAL;
	value = strtol((yyvsp[0].str), NULL, 0);
	prop = matcherprop_new(criteria, NULL, 0, NULL, value);
}
#line 3144 "matcher_parser_parse.c"
    break;

  case 100: /* one_condition: MATCHER_SIZE_GREATER MATCHER_INTEGER  */
#line 1116 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gint value    = 0;
	criteria = MATCHCRITERIA_SIZE_GREATER;
	value = strtol((yyvsp[0].str), NULL, 0);
	prop = matcherprop_new(criteria, NULL, 0, NULL, value);
}
#line 3156 "matcher_parser_parse.c"
    break;

  case 101: /* one_condition: MATCHER_SIZE_SMALLER MATCHER_INTEGER  */
#line 1124 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gint value    = 0;
	criteria = MATCHCRITERIA_SIZE_SMALLER;
	value = strtol((yyvsp[0].str), NULL, 0);
	prop = matcherprop_new(criteria, NULL, 0, NULL, value);
}
#line 3168 "matcher_parser_parse.c"
    break;

  case 102: /* one_condition: MATCHER_SIZE_EQUAL MATCHER_INTEGER  */
#line 1132 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gint value    = 0;
	criteria = MATCHCRITERIA_SIZE_EQUAL;
	value = strtol((yyvsp[0].str), NULL, 0);
	prop = matcherprop_new(criteria, NULL, 0, NULL, value);
}
#line 3180 "matcher_parser_parse.c"
    break;

  case 103: /* $@3: %empty  */
#line 1140 "matcher_parser_parse.y"
{
	header = g_strdup((yyvsp[0].str));
}
#line 3188 "matcher_parser_parse.c"
    break;

  case 104: /* one_condition: MATCHER_HEADER MATCHER_STRING $@3 match_type MATCHER_STRING  */
#line 1143 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;
	matcher_is_fast = FALSE;
	criteria = MATCHCRITERIA_HEADER;
	expr = (yyvsp[-3].str);
	prop = matcherprop_new(criteria, header, match_type, expr, 0);
	g_free(header);
}
#line 3202 "matcher_parser_parse.c"
    break;

  case 105: /* $@4: %empty  */
#line 1153 "matcher_parser_parse.y"
{
	header = g_strdup((yyvsp[0].str));
}
#line 3210 "matcher_parser_parse.c"
    break;

  case 106: /* one_condition: MATCHER_NOT_HEADER MATCHER_STRING $@4 match_type MATCHER_STRING  */
#line 1156 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;
	matcher_is_fast = FALSE;
	criteria = MATCHCRITERIA_NOT_HEADER;
	expr = (yyvsp[-3].str);
	prop = matcherprop_new(criteria, header, match_type, expr, 0);
	g_free(header);
}
#line 3224 "matcher_parser_parse.c"
    break;

  case 107: /* one_condition: MATCHER_HEADERS_PART match_type MATCHER_STRING  */
#line 1166 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;
	matcher_is_fast = FALSE;
	criteria = MATCHCRITERIA_HEADERS_PART;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 3237 "matcher_parser_parse.c"
    break;

  case 108: /* one_condition: MATCHER_NOT_HEADERS_PART match_type MATCHER_STRING  */
#line 1175 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;
	matcher_is_fast = FALSE;
	criteria = MATCHCRITERIA_NOT_HEADERS_PART;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 3250 "matcher_parser_parse.c"
    break;

  case 109: /* one_condition: MATCHER_HEADERS_CONT match_type MATCHER_STRING  */
#line 1184 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;
	matcher_is_fast = FALSE;
	criteria = MATCHCRITERIA_HEADERS_CONT;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 3263 "matcher_parser_parse.c"
    break;

  case 110: /* one_condition: MATCHER_NOT_HEADERS_CONT match_type MATCHER_STRING  */
#line 1193 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;
	matcher_is_fast = FALSE;
	criteria = MATCHCRITERIA_NOT_HEADERS_CONT;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 3276 "matcher_parser_parse.c"
    break;

  case 111: /* $@5: %empty  */
#line 1202 "matcher_parser_parse.y"
{
	header = g_strdup((yyvsp[0].str));
}
#line 3284 "matcher_parser_parse.c"
    break;

  case 112: /* one_condition: MATCHER_FOUND_IN_ADDRESSBOOK MATCHER_STRING $@5 MATCHER_IN MATCHER_STRING  */
#line 1205 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_FOUND_IN_ADDRESSBOOK;
	expr = (yyvsp[-3].str);
	prop = matcherprop_new(criteria, header, match_type, expr, 0);
	g_free(header);
}
#line 3298 "matcher_parser_parse.c"
    break;

  case 113: /* $@6: %empty  */
#line 1215 "matcher_parser_parse.y"
{
	header = g_strdup((yyvsp[0].str));
}
#line 3306 "matcher_parser_parse.c"
    break;

  case 114: /* one_condition: MATCHER_NOT_FOUND_IN_ADDRESSBOOK MATCHER_STRING $@6 MATCHER_IN MATCHER_STRING  */
#line 1218 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;

	criteria = MATCHCRITERIA_NOT_FOUND_IN_ADDRESSBOOK;
	expr = (yyvsp[-3].str);
	prop = matcherprop_new(criteria, header, match_type, expr, 0);
	g_free(header);
}
#line 3320 "matcher_parser_parse.c"
    break;

  case 115: /* one_condition: MATCHER_MESSAGE match_type MATCHER_STRING  */
#line 1228 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;
	matcher_is_fast = FALSE;
	criteria = MATCHCRITERIA_MESSAGE;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 3333 "matcher_parser_parse.c"
    break;

  case 116: /* one_condition: MATCHER_NOT_MESSAGE match_type MATCHER_STRING  */
#line 1237 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;
	matcher_is_fast = FALSE;
	criteria = MATCHCRITERIA_NOT_MESSAGE;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 3346 "matcher_parser_parse.c"
    break;

  case 117: /* one_condition: MATCHER_BODY_PART match_type MATCHER_STRING  */
#line 1246 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;
	matcher_is_fast = FALSE;
	criteria = MATCHCRITERIA_BODY_PART;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 3359 "matcher_parser_parse.c"
    break;

  case 118: /* one_condition: MATCHER_NOT_BODY_PART match_type MATCHER_STRING  */
#line 1255 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;
	matcher_is_fast = FALSE;
	criteria = MATCHCRITERIA_NOT_BODY_PART;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, match_type, expr, 0);
}
#line 3372 "matcher_parser_parse.c"
    break;

  case 119: /* one_condition: MATCHER_TEST MATCHER_STRING  */
#line 1264 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;
	matcher_is_fast = FALSE;
	criteria = MATCHCRITERIA_TEST;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, MATCHTYPE_MATCH, expr, 0);
}
#line 3385 "matcher_parser_parse.c"
    break;

  case 120: /* one_condition: MATCHER_NOT_TEST MATCHER_STRING  */
#line 1273 "matcher_parser_parse.y"
{
	gint criteria = 0;
	gchar *expr = NULL;
	matcher_is_fast = FALSE;
	criteria = MATCHCRITERIA_NOT_TEST;
	expr = (yyvsp[0].str);
	prop = matcherprop_new(criteria, NULL, MATCHTYPE_MATCH, expr, 0);
}
#line 3398 "matcher_parser_parse.c"
    break;

  case 121: /* filtering_action: MATCHER_EXECUTE MATCHER_STRING  */
#line 1285 "matcher_parser_parse.y"
{
	gchar *cmd = NULL;
	gint action_type = 0;

	action_type = MATCHACTION_EXECUTE;
	cmd = (yyvsp[0].str);
	action = filteringaction_new(action_type, 0, cmd, 0, 0, NULL);
}
#line 3411 "matcher_parser_parse.c"
    break;

  case 122: /* filtering_action: MATCHER_MOVE MATCHER_STRING  */
#line 1294 "matcher_parser_parse.y"
{
	gchar *destination = NULL;
	gint action_type = 0;

	action_type = MATCHACTION_MOVE;
	destination = (yyvsp[0].str);
	action = filteringaction_new(action_type, 0, destination, 0, 0, NULL);
}
#line 3424 "matcher_parser_parse.c"
    break;

  case 123: /* filtering_action: MATCHER_SET_TAG MATCHER_STRING  */
#line 1303 "matcher_parser_parse.y"
{
	gchar *destination = NULL;
	gint action_type = 0;

	action_type = MATCHACTION_SET_TAG;
	destination = (yyvsp[0].str);
	action = filteringaction_new(action_type, 0, destination, 0, 0, NULL);
}
#line 3437 "matcher_parser_parse.c"
    break;

  case 124: /* filtering_action: MATCHER_UNSET_TAG MATCHER_STRING  */
#line 1312 "matcher_parser_parse.y"
{
	gchar *destination = NULL;
	gint action_type = 0;

	action_type = MATCHACTION_UNSET_TAG;
	destination = (yyvsp[0].str);
	action = filteringaction_new(action_type, 0, destination, 0, 0, NULL);
}
#line 3450 "matcher_parser_parse.c"
    break;

  case 125: /* filtering_action: MATCHER_CLEAR_TAGS  */
#line 1321 "matcher_parser_parse.y"
{
	gint action_type = 0;

	action_type = MATCHACTION_CLEAR_TAGS;
	action = filteringaction_new(action_type, 0, NULL, 0, 0, NULL);
}
#line 3461 "matcher_parser_parse.c"
    break;

  case 126: /* filtering_action: MATCHER_COPY MATCHER_STRING  */
#line 1328 "matcher_parser_parse.y"
{
	gchar *destination = NULL;
	gint action_type = 0;

	action_type = MATCHACTION_COPY;
	destination = (yyvsp[0].str);
	action = filteringaction_new(action_type, 0, destination, 0, 0, NULL);
}
#line 3474 "matcher_parser_parse.c"
    break;

  case 127: /* filtering_action: MATCHER_DELETE  */
#line 1337 "matcher_parser_parse.y"
{
	gint action_type = 0;

	action_type = MATCHACTION_DELETE;
	action = filteringaction_new(action_type, 0, NULL, 0, 0, NULL);
}
#line 3485 "matcher_parser_parse.c"
    break;

  case 128: /* filtering_action: MATCHER_MARK  */
#line 1344 "matcher_parser_parse.y"
{
	gint action_type = 0;

	action_type = MATCHACTION_MARK;
	action = filteringaction_new(action_type, 0, NULL, 0, 0, NULL);
}
#line 3496 "matcher_parser_parse.c"
    break;

  case 129: /* filtering_action: MATCHER_UNMARK  */
#line 1351 "matcher_parser_parse.y"
{
	gint action_type = 0;

	action_type = MATCHACTION_UNMARK;
	action = filteringaction_new(action_type, 0, NULL, 0, 0, NULL);
}
#line 3507 "matcher_parser_parse.c"
    break;

  case 130: /* filtering_action: MATCHER_LOCK  */
#line 1358 "matcher_parser_parse.y"
{
	gint action_type = 0;

	action_type = MATCHACTION_LOCK;
	action = filteringaction_new(action_type, 0, NULL, 0, 0, NULL);
}
#line 3518 "matcher_parser_parse.c"
    break;

  case 131: /* filtering_action: MATCHER_UNLOCK  */
#line 1365 "matcher_parser_parse.y"
{
	gint action_type = 0;

	action_type = MATCHACTION_UNLOCK;
	action = filteringaction_new(action_type, 0, NULL, 0, 0, NULL);
}
#line 3529 "matcher_parser_parse.c"
    break;

  case 132: /* filtering_action: MATCHER_MARK_AS_READ  */
#line 1372 "matcher_parser_parse.y"
{
	gint action_type = 0;

	action_type = MATCHACTION_MARK_AS_READ;
	action = filteringaction_new(action_type, 0, NULL, 0, 0, NULL);
}
#line 3540 "matcher_parser_parse.c"
    break;

  case 133: /* filtering_action: MATCHER_MARK_AS_UNREAD  */
#line 1379 "matcher_parser_parse.y"
{
	gint action_type = 0;

	action_type = MATCHACTION_MARK_AS_UNREAD;
	action = filteringaction_new(action_type, 0, NULL, 0, 0, NULL);
}
#line 3551 "matcher_parser_parse.c"
    break;

  case 134: /* filtering_action: MATCHER_MARK_AS_SPAM  */
#line 1386 "matcher_parser_parse.y"
{
	gint action_type = 0;

	action_type = MATCHACTION_MARK_AS_SPAM;
	action = filteringaction_new(action_type, 0, NULL, 0, 0, NULL);
}
#line 3562 "matcher_parser_parse.c"
    break;

  case 135: /* filtering_action: MATCHER_MARK_AS_HAM  */
#line 1393 "matcher_parser_parse.y"
{
	gint action_type = 0;

	action_type = MATCHACTION_MARK_AS_HAM;
	action = filteringaction_new(action_type, 0, NULL, 0, 0, NULL);
}
#line 3573 "matcher_parser_parse.c"
    break;

  case 136: /* filtering_action: MATCHER_FORWARD MATCHER_INTEGER MATCHER_STRING  */
#line 1400 "matcher_parser_parse.y"
{
	gchar *destination = NULL;
	gint action_type = 0;
	gint account_id = 0;

	action_type = MATCHACTION_FORWARD;
	account_id = strtol((yyvsp[-1].str), NULL, 10);
	destination = (yyvsp[0].str);
	action = filteringaction_new(action_type,
            account_id, destination, 0, 0, NULL);
}
#line 3589 "matcher_parser_parse.c"
    break;

  case 137: /* filtering_action: MATCHER_FORWARD_AS_ATTACHMENT MATCHER_INTEGER MATCHER_STRING  */
#line 1412 "matcher_parser_parse.y"
{
	gchar *destination = NULL;
	gint action_type = 0;
	gint account_id = 0;

	action_type = MATCHACTION_FORWARD_AS_ATTACHMENT;
	account_id = strtol((yyvsp[-1].str), NULL, 10);
	destination = (yyvsp[0].str);
	action = filteringaction_new(action_type,
            account_id, destination, 0, 0, NULL);
}
#line 3605 "matcher_parser_parse.c"
    break;

  case 138: /* filtering_action: MATCHER_REDIRECT MATCHER_INTEGER MATCHER_STRING  */
#line 1424 "matcher_parser_parse.y"
{
	gchar *destination = NULL;
	gint action_type = 0;
	gint account_id = 0;

	action_type = MATCHACTION_REDIRECT;
	account_id = strtol((yyvsp[-1].str), NULL, 10);
	destination = (yyvsp[0].str);
	action = filteringaction_new(action_type,
            account_id, destination, 0, 0, NULL);
}
#line 3621 "matcher_parser_parse.c"
    break;

  case 139: /* filtering_action: MATCHER_COLOR MATCHER_INTEGER  */
#line 1436 "matcher_parser_parse.y"
{
	gint action_type = 0;
	gint color = 0;

	action_type = MATCHACTION_COLOR;
	color = strtol((yyvsp[0].str), NULL, 10);
	action = filteringaction_new(action_type, 0, NULL, color, 0, NULL);
}
#line 3634 "matcher_parser_parse.c"
    break;

  case 140: /* filtering_action: MATCHER_CHANGE_SCORE MATCHER_INTEGER  */
#line 1445 "matcher_parser_parse.y"
{
        gint score = 0;
        
        score = strtol((yyvsp[0].str), NULL, 10);
	action = filteringaction_new(MATCHACTION_CHANGE_SCORE, 0,
				     NULL, 0, score, NULL);
}
#line 3646 "matcher_parser_parse.c"
    break;

  case 141: /* filtering_action: MATCHER_SCORE MATCHER_INTEGER  */
#line 1454 "matcher_parser_parse.y"
{
        gint score = 0;
        
        score = strtol((yyvsp[0].str), NULL, 10);
	action = filteringaction_new(MATCHACTION_CHANGE_SCORE, 0,
				     NULL, 0, score, NULL);
}
#line 3658 "matcher_parser_parse.c"
    break;

  case 142: /* filtering_action: MATCHER_SET_SCORE MATCHER_INTEGER  */
#line 1462 "matcher_parser_parse.y"
{
        gint score = 0;
        
        score = strtol((yyvsp[0].str), NULL, 10);
	action = filteringaction_new(MATCHACTION_SET_SCORE, 0,
				     NULL, 0, score, NULL);
}
#line 3670 "matcher_parser_parse.c"
    break;

  case 143: /* filtering_action: MATCHER_HIDE  */
#line 1470 "matcher_parser_parse.y"
{
	action = filteringaction_new(MATCHACTION_HIDE, 0, NULL, 0, 0, NULL);
}
#line 3678 "matcher_parser_parse.c"
    break;

  case 144: /* filtering_action: MATCHER_IGNORE  */
#line 1474 "matcher_parser_parse.y"
{
	action = filteringaction_new(MATCHACTION_IGNORE, 0, NULL, 0, 0, NULL);
}
#line 3686 "matcher_parser_parse.c"
    break;

  case 145: /* filtering_action: MATCHER_WATCH  */
#line 1478 "matcher_parser_parse.y"
{
	action = filteringaction_new(MATCHACTION_WATCH, 0, NULL, 0, 0, NULL);
}
#line 3694 "matcher_parser_parse.c"
    break;

  case 146: /* $@7: %empty  */
#line 1482 "matcher_parser_parse.y"
{
	header = g_strdup((yyvsp[0].str));
}
#line 3702 "matcher_parser_parse.c"
    break;

  case 147: /* filtering_action: MATCHER_ADD_TO_ADDRESSBOOK MATCHER_STRING $@7 MATCHER_STRING  */
#line 1485 "matcher_parser_parse.y"
{
	gchar *addressbook = NULL;
	gint action_type = 0;

	action_type = MATCHACTION_ADD_TO_ADDRESSBOOK;
	addressbook = (yyvsp[-2].str);
	action = filteringaction_new(action_type, 0, addressbook, 0, 0, header);
	g_free(header);
}
#line 3716 "matcher_parser_parse.c"
    break;

  case 148: /* filtering_action: MATCHER_STOP  */
#line 1495 "matcher_parser_parse.y"
{
	action = filteringaction_new(MATCHACTION_STOP, 0, NULL, 0, 0, NULL);
}
#line 3724 "matcher_parser_parse.c"
    break;


#line 3728 "matcher_parser_parse.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

