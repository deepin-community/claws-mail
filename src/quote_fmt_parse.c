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
#line 20 "quote_fmt_parse.y"


#include "defs.h"

#include <glib.h>
#include <glib/gi18n.h>

#include <ctype.h>

#include "procmsg.h"
#include "procmime.h"
#include "utils.h"
#include "codeconv.h"
#include "procheader.h"
#include "addr_compl.h"
#include "gtk/inputdialog.h"

#include "quote_fmt.h"
#include "quote_fmt_lex.h"
#include "account.h"
#include "file-utils.h"

/* decl */
/*
flex quote_fmt.l
bison -p quote_fmt quote_fmt.y
*/

int yylex(void);

static MsgInfo *msginfo = NULL;
static PrefsAccount *account = NULL;
#ifdef USE_ENCHANT
static gchar default_dictionary[BUFFSIZE];
#endif
static gboolean *visible = NULL;
static gboolean dry_run = FALSE;
static gint maxsize = 0;
static gint stacksize = 0;
static GHashTable *var_table = NULL;
static GList *attachments = NULL;

typedef struct st_buffer
{
	gchar *buffer;
	gint bufsize;
	gint bufmax;
} st_buffer;

static struct st_buffer main_expr = { NULL, 0, 0 };
static struct st_buffer sub_expr = { NULL, 0, 0 };
static struct st_buffer* current = NULL;

static const gchar *quote_str = NULL;
static const gchar *body = NULL;
static gint error = 0;

static gint cursor_pos = -1;

extern int quote_fmt_firsttime;
extern int line;
extern int escaped_string;

static void add_visibility(gboolean val)
{
	stacksize++;
	if (maxsize < stacksize) {
		maxsize += 128;
		visible = g_realloc(visible, maxsize * sizeof(gboolean));
		if (visible == NULL)
			maxsize = 0;
	}
	if (visible != NULL)
		visible[stacksize - 1] = val;
}

static void remove_visibility(void)
{
	stacksize--;
	if (stacksize < 0) {
		g_warning("error: visibility stack underflow");
		stacksize = 0;
	}
}

static void add_buffer(const gchar *s)
{
	gint len;

	if (s == NULL)
		return;

	len = strlen(s);
	if (current->bufsize + len + 1 > current->bufmax) {
		if (current->bufmax == 0)
			current->bufmax = 128;
		while (current->bufsize + len + 1 > current->bufmax)
			current->bufmax *= 2;
		current->buffer = g_realloc(current->buffer, current->bufmax);
	}
	strcpy(current->buffer + current->bufsize, s);
	current->bufsize += len;
}

static void clear_buffer(void)
{
	if (current->buffer)
		*current->buffer = '\0';
	else
		/* force to an empty string, as buffer should not be left unallocated */
		add_buffer("");
	current->bufsize = 0;
}

gchar *quote_fmt_get_buffer(void)
{
	if (current != &main_expr)
		g_warning("error: parser still in sub-expr mode");

	if (error != 0)
		return NULL;
	else
		return current->buffer;
}

GList *quote_fmt_get_attachments_list(void)
{
	return attachments;
}

gint quote_fmt_get_line(void)
{
	return line;
}

gint quote_fmt_get_cursor_pos(void)
{
	return cursor_pos;	
}

#define INSERT(buf) \
	if (stacksize != 0 && visible[stacksize - 1])\
		add_buffer(buf); \

#define INSERT_CHARACTER(chr) \
	if (stacksize != 0 && visible[stacksize - 1]) { \
		gchar tmp[2]; \
		tmp[0] = (chr); \
		tmp[1] = '\0'; \
		add_buffer(tmp); \
	}

void quote_fmt_reset_vartable(void)
{
	if (var_table) {
		g_hash_table_destroy(var_table);
		var_table = NULL;
	}
	if (attachments) {
		GList *cur = attachments;
		while (cur) {
			g_free(cur->data);
			cur = g_list_next(cur);
		}
		g_list_free(attachments);
		attachments = NULL;
	}
}

#ifdef USE_ENCHANT
void quote_fmt_init(MsgInfo *info, const gchar *my_quote_str,
		    const gchar *my_body, gboolean my_dry_run,
			PrefsAccount *compose_account,
			gboolean string_is_escaped,
			GtkAspell *compose_gtkaspell)
#else
void quote_fmt_init(MsgInfo *info, const gchar *my_quote_str,
		    const gchar *my_body, gboolean my_dry_run,
			PrefsAccount *compose_account,
			gboolean string_is_escaped)
#endif
{
	quote_str = my_quote_str;
	body = my_body;
	msginfo = info;
	account = compose_account;
#ifdef USE_ENCHANT
	gchar *dict = gtkaspell_get_default_dictionary(compose_gtkaspell);
	if (dict)
		strncpy2(default_dictionary, dict, sizeof(default_dictionary));
	else
		*default_dictionary = '\0';
#endif
	dry_run = my_dry_run;
	stacksize = 0;
	add_visibility(TRUE);
	main_expr.bufmax = 0;
	sub_expr.bufmax = 0;
	current = &main_expr;
	clear_buffer();
	error = 0;
	line = 1;
	escaped_string = string_is_escaped;

	if (!var_table)
		var_table = g_hash_table_new_full(g_str_hash, g_str_equal, 
				g_free, g_free);

        /*
         * force LEX initialization
         */
	quote_fmt_firsttime = 1;
	cursor_pos = -1;
}

void quote_fmterror(char *str)
{
	g_warning("error: %s at line %d", str, line);
	error = 1;
}

int quote_fmtwrap(void)
{
	return 1;
}

static int isseparator(int ch)
{
	return g_ascii_isspace(ch) || ch == '.' || ch == '-';
}

/*
 * Search for glibc extended strftime timezone specs within haystack.
 * If not found NULL is returned and the integer pointed by tzspeclen is
 * not changed.
 * If found a pointer to the start of the specification within haystack
 * is returned and the integer pointed by tzspeclen is set to the lenght
 * of specification.
 */
static const char* strtzspec(const char *haystack, int *tzspeclen)
{
	const char *p = NULL;
	const char *q = NULL;
	const char *r = NULL;

	p = strstr(haystack, "%");
	while (p != NULL) {
		q = p + 1;
		if (!*q) return NULL;
		r = strchr("_-0^#", *q); /* skip flags */
		if (r != NULL) {
			++q;
			if (!*q) return NULL;
		}
		while (*q >= '0' && *q <= '9') ++q; /* skip width */
		if (!*q) return NULL;
		if (*q == 'z' || *q == 'Z') { /* numeric or name */
			*tzspeclen = 1 + (q - p);
			return p;
		}
		p = strstr(q, "%");
	}
	return NULL;
}

static void quote_fmt_show_date(const MsgInfo *msginfo, const gchar *format)
{
	char  result[100];
	char *rptr;
	char  zone[6];
	struct tm lt;
	const char *fptr;
	const char *zptr;

	if (!msginfo->date)
		return;
	
	/* 
	 * ALF - GNU C's strftime() has a nice format specifier 
	 * for time zone offset (%z). Non-standard however, so 
	 * emulate it.
	 */

#define RLEFT (sizeof result) - (rptr - result)	

	zone[0] = 0;

	if (procheader_date_parse_to_tm(msginfo->date, &lt, zone)) {
		/*
		 * break up format string in tiny bits delimited by valid %z's and 
		 * feed it to strftime(). don't forget that '%%z' mean literal '%z'.
		 */
		for (rptr = result, fptr = format; fptr && *fptr && rptr < &result[sizeof result - 1];) {
			int	    perc, zlen;
			const char *p;
			char	   *tmp;
			
			if (NULL != (zptr = strtzspec(fptr, &zlen))) {
				/*
				 * count nr. of prepended percent chars
				 */
				for (perc = 0, p = zptr; p && p >= format && *p == '%'; p--, perc++)
					;
				/*
				 * feed to strftime()
				 */
				tmp = g_strndup(fptr, zptr - fptr + (perc % 2 ? 0 : zlen));
				if (tmp) {
					rptr += strftime(rptr, RLEFT, tmp, &lt);
					g_free(tmp);
				}
				/*
				 * append time zone offset
				 */
				if (zone[0] && perc % 2) 
					rptr += g_snprintf(rptr, RLEFT, "%s", zone);
				fptr = zptr + zlen;
			} else {
				rptr += strftime(rptr, RLEFT, fptr, &lt);
				fptr  = NULL;
			}
		}
		
		if (g_utf8_validate(result, -1, NULL)) {
			INSERT(result);
		} else {
			gchar *utf = conv_codeset_strdup(result, 
				conv_get_locale_charset_str_no_utf8(),
				CS_INTERNAL);
			if (utf == NULL || 
			    !g_utf8_validate(utf, -1, NULL)) {
				g_free(utf);
				utf = g_malloc(strlen(result)*2+1);
				conv_localetodisp(utf, 
					strlen(result)*2+1, result);
			}
			if (g_utf8_validate(utf, -1, NULL)) {
				INSERT(utf);
			}
			g_free(utf);
		}
	}
#undef RLEFT			
}		

static void quote_fmt_show_first_name(const MsgInfo *msginfo)
{
	guchar *p;
	gchar *str;

	if (!msginfo->fromname)
		return;	
	
	p = (guchar*)strchr(msginfo->fromname, ',');
	if (p != NULL) {
		/* fromname is like "Duck, Donald" */
		p++;
		while (*p && isspace(*p)) p++;
		str = alloca(strlen((char *)p) + 1);
		if (str != NULL) {
			strcpy(str, (char *)p);
			INSERT(str);
		}
	} else {
		/* fromname is like "Donald Duck" */
		str = alloca(strlen(msginfo->fromname) + 1);
		if (str != NULL) {
			strcpy(str, msginfo->fromname);
			p = (guchar *)str;
			while (*p && !isspace(*p)) p++;
			*p = '\0';
			INSERT(str);
		}
	}
}

static void quote_fmt_show_last_name(const MsgInfo *msginfo)
{
	gchar *p;
	gchar *str;

	/* This probably won't work together very well with Middle
           names and the like - thth */
	if (!msginfo->fromname) 
		return;

	str = alloca(strlen(msginfo->fromname) + 1);
	if (str != NULL) {
		strcpy(str, msginfo->fromname);
		p = strchr(str, ',');
		if (p != NULL) {
			/* fromname is like "Duck, Donald" */
			*p = '\0';
			INSERT(str);
		} else {
			/* fromname is like "Donald Duck" */
			p = str;
			while (*p && !isspace(*p)) p++;
			if (*p) {
			    /* We found a space. Get first 
			     none-space char and insert
			     rest of string from there. */
			    while (*p && isspace(*p)) p++;
			    if (*p) {
				INSERT(p);
			    } else {
				/* If there is no none-space 
				 char, just insert whole 
				 fromname. */
				INSERT(str);
			    }
			} else {
			    /* If there is no space, just 
			     insert whole fromname. */
			    INSERT(str);
			}
		}
	}
}

static void quote_fmt_show_sender_initial(const MsgInfo *msginfo)
{
#define MAX_SENDER_INITIAL 20
	gchar tmp[MAX_SENDER_INITIAL];
	guchar *p;
	gchar *cur;
	gint len = 0;

	if (!msginfo->fromname) 
		return;

	p = (guchar *)msginfo->fromname;
	cur = tmp;
	while (*p) {
		if (*p && g_utf8_validate((gchar *)p, 1, NULL)) {
			*cur = toupper(*p);
				cur++;
			len++;
			if (len >= MAX_SENDER_INITIAL - 1)
				break;
		} else
			break;
		while (*p && !isseparator(*p)) p++;
		while (*p && isseparator(*p)) p++;
	}
	*cur = '\0';
	INSERT(tmp);
}

static void quote_fmt_show_msg(MsgInfo *msginfo, const gchar *body,
			       gboolean quoted, gboolean signature,
			       const gchar *quote_str)
{
	gchar buf[BUFFSIZE];
	FILE *fp;

	if (!(msginfo->folder || body))
		return;

	if (body)
		fp = str_open_as_stream(body);
	else {
		if (MSG_IS_ENCRYPTED(msginfo->flags))
			fp = procmime_get_first_encrypted_text_content(msginfo);
		else
			fp = procmime_get_first_text_content(msginfo);
	}

	if (fp == NULL)
		g_warning("can't get text part");
	else {
		account_sigsep_matchlist_create();
		while (fgets(buf, sizeof(buf), fp) != NULL) {
			strcrchomp(buf);

			if (!signature && account_sigsep_matchlist_nchar_found(buf, "%s\n"))
				break;
		
			if (quoted && quote_str)
				INSERT(quote_str);
			
			INSERT(buf);
		}
		account_sigsep_matchlist_delete();
		fclose(fp);
	}
}

static void quote_fmt_insert_file(const gchar *filename)
{
	FILE *file;
	char buffer[PATH_MAX];
	
	if ((file = g_fopen(filename, "rb")) != NULL) {
		while (fgets(buffer, sizeof(buffer), file)) {
			INSERT(buffer);
		}
		fclose(file);
	}

}

static void quote_fmt_insert_program_output(const gchar *progname)
{
	FILE *file;
	char buffer[BUFFSIZE];

	if ((file = get_command_output_stream(progname)) != NULL) {
		while (fgets(buffer, sizeof(buffer), file)) {
			INSERT(buffer);
		}
		fclose(file);
	}
}

static void quote_fmt_insert_user_input(const gchar *varname)
{
    gchar *buf = NULL;
    gchar *text = NULL;

    if (dry_run) 
	    return;

    if ((text = g_hash_table_lookup(var_table, varname)) == NULL) {
	    buf = g_strdup_printf(_("Enter text to replace '%s'"), varname);
	    text = input_dialog(_("Enter variable"), buf, "");
	    g_free(buf);
	    if (!text)
		    return;
	    g_hash_table_insert(var_table, g_strdup(varname), g_strdup(text));
    } else {
	    /* don't free the one in hashtable at the end */
	    text = g_strdup(text);
    }

    if (!text)
	    return;
    INSERT(text);
    g_free(text);
}

static void quote_fmt_attach_file(const gchar *filename)
{
	attachments = g_list_append(attachments, g_strdup(filename));
}

static void quote_fmt_attach_file_program_output(const gchar *progname)
{
	FILE *file;
	char buffer[BUFFSIZE];

	if ((file = get_command_output_stream(progname)) != NULL) {
		/* get first line only */
		if (fgets(buffer, sizeof(buffer), file)) {
			/* trim trailing CR/LF */
			strretchomp(buffer);
			attachments = g_list_append(attachments, g_strdup(buffer));
		}
		fclose(file);
	}
}

static gchar *quote_fmt_complete_address(const gchar *addr)
{
	gint count;
	gchar *res, *tmp, *email_addr;
	gchar **split;

	debug_print("quote_fmt_complete_address: %s\n", addr);
	if (addr == NULL)
		return NULL;

	/* if addr is a list of message, try the 1st element only */
	split = g_strsplit(addr, ",", -1);
	if (!split || !split[0] || *split[0] == '\0') {
		g_strfreev(split);
		return NULL;
	}

	Xstrdup_a(email_addr, split[0], {
                g_strfreev(split);
                return NULL;
        });
	extract_address(email_addr);
	if (!*email_addr) {
		g_strfreev(split);
		return NULL;
	}

	res = NULL;
	start_address_completion(NULL);
	if (1 < (count = complete_address(email_addr))) {
		tmp = get_complete_address(1);
		res = procheader_get_fromname(tmp);
		g_free(tmp);
	}
	end_address_completion();
	g_strfreev(split);

	debug_print("quote_fmt_complete_address: matched %s\n", res);
	return res;
}


#line 676 "quote_fmt_parse.c"

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
#ifndef YY_YY_QUOTE_FMT_PARSE_H_INCLUDED
# define YY_YY_QUOTE_FMT_PARSE_H_INCLUDED
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
    SHOW_NEWSGROUPS = 258,         /* SHOW_NEWSGROUPS  */
    SHOW_DATE = 259,               /* SHOW_DATE  */
    SHOW_FROM = 260,               /* SHOW_FROM  */
    SHOW_FULLNAME = 261,           /* SHOW_FULLNAME  */
    SHOW_FIRST_NAME = 262,         /* SHOW_FIRST_NAME  */
    SHOW_LAST_NAME = 263,          /* SHOW_LAST_NAME  */
    SHOW_SENDER_INITIAL = 264,     /* SHOW_SENDER_INITIAL  */
    SHOW_SUBJECT = 265,            /* SHOW_SUBJECT  */
    SHOW_TO = 266,                 /* SHOW_TO  */
    SHOW_MESSAGEID = 267,          /* SHOW_MESSAGEID  */
    SHOW_PERCENT = 268,            /* SHOW_PERCENT  */
    SHOW_CC = 269,                 /* SHOW_CC  */
    SHOW_REFERENCES = 270,         /* SHOW_REFERENCES  */
    SHOW_MESSAGE = 271,            /* SHOW_MESSAGE  */
    SHOW_QUOTED_MESSAGE = 272,     /* SHOW_QUOTED_MESSAGE  */
    SHOW_BACKSLASH = 273,          /* SHOW_BACKSLASH  */
    SHOW_TAB = 274,                /* SHOW_TAB  */
    SHOW_MAIL_ADDRESS = 275,       /* SHOW_MAIL_ADDRESS  */
    SHOW_QUOTED_MESSAGE_NO_SIGNATURE = 276, /* SHOW_QUOTED_MESSAGE_NO_SIGNATURE  */
    SHOW_MESSAGE_NO_SIGNATURE = 277, /* SHOW_MESSAGE_NO_SIGNATURE  */
    SHOW_EOL = 278,                /* SHOW_EOL  */
    SHOW_QUESTION_MARK = 279,      /* SHOW_QUESTION_MARK  */
    SHOW_EXCLAMATION_MARK = 280,   /* SHOW_EXCLAMATION_MARK  */
    SHOW_PIPE = 281,               /* SHOW_PIPE  */
    SHOW_OPARENT = 282,            /* SHOW_OPARENT  */
    SHOW_CPARENT = 283,            /* SHOW_CPARENT  */
    SHOW_ACCOUNT_FULL_NAME = 284,  /* SHOW_ACCOUNT_FULL_NAME  */
    SHOW_ACCOUNT_MAIL_ADDRESS = 285, /* SHOW_ACCOUNT_MAIL_ADDRESS  */
    SHOW_ACCOUNT_NAME = 286,       /* SHOW_ACCOUNT_NAME  */
    SHOW_ACCOUNT_ORGANIZATION = 287, /* SHOW_ACCOUNT_ORGANIZATION  */
    SHOW_ACCOUNT_DICT = 288,       /* SHOW_ACCOUNT_DICT  */
    SHOW_ACCOUNT_SIG = 289,        /* SHOW_ACCOUNT_SIG  */
    SHOW_ACCOUNT_SIGPATH = 290,    /* SHOW_ACCOUNT_SIGPATH  */
    SHOW_DICT = 291,               /* SHOW_DICT  */
    SHOW_TAGS = 292,               /* SHOW_TAGS  */
    SHOW_ADDRESSBOOK_COMPLETION_FOR_CC = 293, /* SHOW_ADDRESSBOOK_COMPLETION_FOR_CC  */
    SHOW_ADDRESSBOOK_COMPLETION_FOR_FROM = 294, /* SHOW_ADDRESSBOOK_COMPLETION_FOR_FROM  */
    SHOW_ADDRESSBOOK_COMPLETION_FOR_TO = 295, /* SHOW_ADDRESSBOOK_COMPLETION_FOR_TO  */
    QUERY_DATE = 296,              /* QUERY_DATE  */
    QUERY_FROM = 297,              /* QUERY_FROM  */
    QUERY_FULLNAME = 298,          /* QUERY_FULLNAME  */
    QUERY_SUBJECT = 299,           /* QUERY_SUBJECT  */
    QUERY_TO = 300,                /* QUERY_TO  */
    QUERY_NEWSGROUPS = 301,        /* QUERY_NEWSGROUPS  */
    QUERY_MESSAGEID = 302,         /* QUERY_MESSAGEID  */
    QUERY_CC = 303,                /* QUERY_CC  */
    QUERY_REFERENCES = 304,        /* QUERY_REFERENCES  */
    QUERY_ACCOUNT_FULL_NAME = 305, /* QUERY_ACCOUNT_FULL_NAME  */
    QUERY_ACCOUNT_ORGANIZATION = 306, /* QUERY_ACCOUNT_ORGANIZATION  */
    QUERY_ACCOUNT_DICT = 307,      /* QUERY_ACCOUNT_DICT  */
    QUERY_ACCOUNT_SIG = 308,       /* QUERY_ACCOUNT_SIG  */
    QUERY_ACCOUNT_SIGPATH = 309,   /* QUERY_ACCOUNT_SIGPATH  */
    QUERY_DICT = 310,              /* QUERY_DICT  */
    QUERY_CC_FOUND_IN_ADDRESSBOOK = 311, /* QUERY_CC_FOUND_IN_ADDRESSBOOK  */
    QUERY_FROM_FOUND_IN_ADDRESSBOOK = 312, /* QUERY_FROM_FOUND_IN_ADDRESSBOOK  */
    QUERY_TO_FOUND_IN_ADDRESSBOOK = 313, /* QUERY_TO_FOUND_IN_ADDRESSBOOK  */
    QUERY_NOT_DATE = 314,          /* QUERY_NOT_DATE  */
    QUERY_NOT_FROM = 315,          /* QUERY_NOT_FROM  */
    QUERY_NOT_FULLNAME = 316,      /* QUERY_NOT_FULLNAME  */
    QUERY_NOT_SUBJECT = 317,       /* QUERY_NOT_SUBJECT  */
    QUERY_NOT_TO = 318,            /* QUERY_NOT_TO  */
    QUERY_NOT_NEWSGROUPS = 319,    /* QUERY_NOT_NEWSGROUPS  */
    QUERY_NOT_MESSAGEID = 320,     /* QUERY_NOT_MESSAGEID  */
    QUERY_NOT_CC = 321,            /* QUERY_NOT_CC  */
    QUERY_NOT_REFERENCES = 322,    /* QUERY_NOT_REFERENCES  */
    QUERY_NOT_ACCOUNT_FULL_NAME = 323, /* QUERY_NOT_ACCOUNT_FULL_NAME  */
    QUERY_NOT_ACCOUNT_ORGANIZATION = 324, /* QUERY_NOT_ACCOUNT_ORGANIZATION  */
    QUERY_NOT_ACCOUNT_DICT = 325,  /* QUERY_NOT_ACCOUNT_DICT  */
    QUERY_NOT_ACCOUNT_SIG = 326,   /* QUERY_NOT_ACCOUNT_SIG  */
    QUERY_NOT_ACCOUNT_SIGPATH = 327, /* QUERY_NOT_ACCOUNT_SIGPATH  */
    QUERY_NOT_DICT = 328,          /* QUERY_NOT_DICT  */
    QUERY_NOT_CC_FOUND_IN_ADDRESSBOOK = 329, /* QUERY_NOT_CC_FOUND_IN_ADDRESSBOOK  */
    QUERY_NOT_FROM_FOUND_IN_ADDRESSBOOK = 330, /* QUERY_NOT_FROM_FOUND_IN_ADDRESSBOOK  */
    QUERY_NOT_TO_FOUND_IN_ADDRESSBOOK = 331, /* QUERY_NOT_TO_FOUND_IN_ADDRESSBOOK  */
    INSERT_FILE = 332,             /* INSERT_FILE  */
    INSERT_PROGRAMOUTPUT = 333,    /* INSERT_PROGRAMOUTPUT  */
    INSERT_USERINPUT = 334,        /* INSERT_USERINPUT  */
    ATTACH_FILE = 335,             /* ATTACH_FILE  */
    ATTACH_PROGRAMOUTPUT = 336,    /* ATTACH_PROGRAMOUTPUT  */
    OPARENT = 337,                 /* OPARENT  */
    CPARENT = 338,                 /* CPARENT  */
    CHARACTER = 339,               /* CHARACTER  */
    SHOW_DATE_EXPR = 340,          /* SHOW_DATE_EXPR  */
    SET_CURSOR_POS = 341           /* SET_CURSOR_POS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define SHOW_NEWSGROUPS 258
#define SHOW_DATE 259
#define SHOW_FROM 260
#define SHOW_FULLNAME 261
#define SHOW_FIRST_NAME 262
#define SHOW_LAST_NAME 263
#define SHOW_SENDER_INITIAL 264
#define SHOW_SUBJECT 265
#define SHOW_TO 266
#define SHOW_MESSAGEID 267
#define SHOW_PERCENT 268
#define SHOW_CC 269
#define SHOW_REFERENCES 270
#define SHOW_MESSAGE 271
#define SHOW_QUOTED_MESSAGE 272
#define SHOW_BACKSLASH 273
#define SHOW_TAB 274
#define SHOW_MAIL_ADDRESS 275
#define SHOW_QUOTED_MESSAGE_NO_SIGNATURE 276
#define SHOW_MESSAGE_NO_SIGNATURE 277
#define SHOW_EOL 278
#define SHOW_QUESTION_MARK 279
#define SHOW_EXCLAMATION_MARK 280
#define SHOW_PIPE 281
#define SHOW_OPARENT 282
#define SHOW_CPARENT 283
#define SHOW_ACCOUNT_FULL_NAME 284
#define SHOW_ACCOUNT_MAIL_ADDRESS 285
#define SHOW_ACCOUNT_NAME 286
#define SHOW_ACCOUNT_ORGANIZATION 287
#define SHOW_ACCOUNT_DICT 288
#define SHOW_ACCOUNT_SIG 289
#define SHOW_ACCOUNT_SIGPATH 290
#define SHOW_DICT 291
#define SHOW_TAGS 292
#define SHOW_ADDRESSBOOK_COMPLETION_FOR_CC 293
#define SHOW_ADDRESSBOOK_COMPLETION_FOR_FROM 294
#define SHOW_ADDRESSBOOK_COMPLETION_FOR_TO 295
#define QUERY_DATE 296
#define QUERY_FROM 297
#define QUERY_FULLNAME 298
#define QUERY_SUBJECT 299
#define QUERY_TO 300
#define QUERY_NEWSGROUPS 301
#define QUERY_MESSAGEID 302
#define QUERY_CC 303
#define QUERY_REFERENCES 304
#define QUERY_ACCOUNT_FULL_NAME 305
#define QUERY_ACCOUNT_ORGANIZATION 306
#define QUERY_ACCOUNT_DICT 307
#define QUERY_ACCOUNT_SIG 308
#define QUERY_ACCOUNT_SIGPATH 309
#define QUERY_DICT 310
#define QUERY_CC_FOUND_IN_ADDRESSBOOK 311
#define QUERY_FROM_FOUND_IN_ADDRESSBOOK 312
#define QUERY_TO_FOUND_IN_ADDRESSBOOK 313
#define QUERY_NOT_DATE 314
#define QUERY_NOT_FROM 315
#define QUERY_NOT_FULLNAME 316
#define QUERY_NOT_SUBJECT 317
#define QUERY_NOT_TO 318
#define QUERY_NOT_NEWSGROUPS 319
#define QUERY_NOT_MESSAGEID 320
#define QUERY_NOT_CC 321
#define QUERY_NOT_REFERENCES 322
#define QUERY_NOT_ACCOUNT_FULL_NAME 323
#define QUERY_NOT_ACCOUNT_ORGANIZATION 324
#define QUERY_NOT_ACCOUNT_DICT 325
#define QUERY_NOT_ACCOUNT_SIG 326
#define QUERY_NOT_ACCOUNT_SIGPATH 327
#define QUERY_NOT_DICT 328
#define QUERY_NOT_CC_FOUND_IN_ADDRESSBOOK 329
#define QUERY_NOT_FROM_FOUND_IN_ADDRESSBOOK 330
#define QUERY_NOT_TO_FOUND_IN_ADDRESSBOOK 331
#define INSERT_FILE 332
#define INSERT_PROGRAMOUTPUT 333
#define INSERT_USERINPUT 334
#define ATTACH_FILE 335
#define ATTACH_PROGRAMOUTPUT 336
#define OPARENT 337
#define CPARENT 338
#define CHARACTER 339
#define SHOW_DATE_EXPR 340
#define SET_CURSOR_POS 341

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 625 "quote_fmt_parse.y"

	char chr;
	char str[256];

#line 906 "quote_fmt_parse.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_QUOTE_FMT_PARSE_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_SHOW_NEWSGROUPS = 3,            /* SHOW_NEWSGROUPS  */
  YYSYMBOL_SHOW_DATE = 4,                  /* SHOW_DATE  */
  YYSYMBOL_SHOW_FROM = 5,                  /* SHOW_FROM  */
  YYSYMBOL_SHOW_FULLNAME = 6,              /* SHOW_FULLNAME  */
  YYSYMBOL_SHOW_FIRST_NAME = 7,            /* SHOW_FIRST_NAME  */
  YYSYMBOL_SHOW_LAST_NAME = 8,             /* SHOW_LAST_NAME  */
  YYSYMBOL_SHOW_SENDER_INITIAL = 9,        /* SHOW_SENDER_INITIAL  */
  YYSYMBOL_SHOW_SUBJECT = 10,              /* SHOW_SUBJECT  */
  YYSYMBOL_SHOW_TO = 11,                   /* SHOW_TO  */
  YYSYMBOL_SHOW_MESSAGEID = 12,            /* SHOW_MESSAGEID  */
  YYSYMBOL_SHOW_PERCENT = 13,              /* SHOW_PERCENT  */
  YYSYMBOL_SHOW_CC = 14,                   /* SHOW_CC  */
  YYSYMBOL_SHOW_REFERENCES = 15,           /* SHOW_REFERENCES  */
  YYSYMBOL_SHOW_MESSAGE = 16,              /* SHOW_MESSAGE  */
  YYSYMBOL_SHOW_QUOTED_MESSAGE = 17,       /* SHOW_QUOTED_MESSAGE  */
  YYSYMBOL_SHOW_BACKSLASH = 18,            /* SHOW_BACKSLASH  */
  YYSYMBOL_SHOW_TAB = 19,                  /* SHOW_TAB  */
  YYSYMBOL_SHOW_MAIL_ADDRESS = 20,         /* SHOW_MAIL_ADDRESS  */
  YYSYMBOL_SHOW_QUOTED_MESSAGE_NO_SIGNATURE = 21, /* SHOW_QUOTED_MESSAGE_NO_SIGNATURE  */
  YYSYMBOL_SHOW_MESSAGE_NO_SIGNATURE = 22, /* SHOW_MESSAGE_NO_SIGNATURE  */
  YYSYMBOL_SHOW_EOL = 23,                  /* SHOW_EOL  */
  YYSYMBOL_SHOW_QUESTION_MARK = 24,        /* SHOW_QUESTION_MARK  */
  YYSYMBOL_SHOW_EXCLAMATION_MARK = 25,     /* SHOW_EXCLAMATION_MARK  */
  YYSYMBOL_SHOW_PIPE = 26,                 /* SHOW_PIPE  */
  YYSYMBOL_SHOW_OPARENT = 27,              /* SHOW_OPARENT  */
  YYSYMBOL_SHOW_CPARENT = 28,              /* SHOW_CPARENT  */
  YYSYMBOL_SHOW_ACCOUNT_FULL_NAME = 29,    /* SHOW_ACCOUNT_FULL_NAME  */
  YYSYMBOL_SHOW_ACCOUNT_MAIL_ADDRESS = 30, /* SHOW_ACCOUNT_MAIL_ADDRESS  */
  YYSYMBOL_SHOW_ACCOUNT_NAME = 31,         /* SHOW_ACCOUNT_NAME  */
  YYSYMBOL_SHOW_ACCOUNT_ORGANIZATION = 32, /* SHOW_ACCOUNT_ORGANIZATION  */
  YYSYMBOL_SHOW_ACCOUNT_DICT = 33,         /* SHOW_ACCOUNT_DICT  */
  YYSYMBOL_SHOW_ACCOUNT_SIG = 34,          /* SHOW_ACCOUNT_SIG  */
  YYSYMBOL_SHOW_ACCOUNT_SIGPATH = 35,      /* SHOW_ACCOUNT_SIGPATH  */
  YYSYMBOL_SHOW_DICT = 36,                 /* SHOW_DICT  */
  YYSYMBOL_SHOW_TAGS = 37,                 /* SHOW_TAGS  */
  YYSYMBOL_SHOW_ADDRESSBOOK_COMPLETION_FOR_CC = 38, /* SHOW_ADDRESSBOOK_COMPLETION_FOR_CC  */
  YYSYMBOL_SHOW_ADDRESSBOOK_COMPLETION_FOR_FROM = 39, /* SHOW_ADDRESSBOOK_COMPLETION_FOR_FROM  */
  YYSYMBOL_SHOW_ADDRESSBOOK_COMPLETION_FOR_TO = 40, /* SHOW_ADDRESSBOOK_COMPLETION_FOR_TO  */
  YYSYMBOL_QUERY_DATE = 41,                /* QUERY_DATE  */
  YYSYMBOL_QUERY_FROM = 42,                /* QUERY_FROM  */
  YYSYMBOL_QUERY_FULLNAME = 43,            /* QUERY_FULLNAME  */
  YYSYMBOL_QUERY_SUBJECT = 44,             /* QUERY_SUBJECT  */
  YYSYMBOL_QUERY_TO = 45,                  /* QUERY_TO  */
  YYSYMBOL_QUERY_NEWSGROUPS = 46,          /* QUERY_NEWSGROUPS  */
  YYSYMBOL_QUERY_MESSAGEID = 47,           /* QUERY_MESSAGEID  */
  YYSYMBOL_QUERY_CC = 48,                  /* QUERY_CC  */
  YYSYMBOL_QUERY_REFERENCES = 49,          /* QUERY_REFERENCES  */
  YYSYMBOL_QUERY_ACCOUNT_FULL_NAME = 50,   /* QUERY_ACCOUNT_FULL_NAME  */
  YYSYMBOL_QUERY_ACCOUNT_ORGANIZATION = 51, /* QUERY_ACCOUNT_ORGANIZATION  */
  YYSYMBOL_QUERY_ACCOUNT_DICT = 52,        /* QUERY_ACCOUNT_DICT  */
  YYSYMBOL_QUERY_ACCOUNT_SIG = 53,         /* QUERY_ACCOUNT_SIG  */
  YYSYMBOL_QUERY_ACCOUNT_SIGPATH = 54,     /* QUERY_ACCOUNT_SIGPATH  */
  YYSYMBOL_QUERY_DICT = 55,                /* QUERY_DICT  */
  YYSYMBOL_QUERY_CC_FOUND_IN_ADDRESSBOOK = 56, /* QUERY_CC_FOUND_IN_ADDRESSBOOK  */
  YYSYMBOL_QUERY_FROM_FOUND_IN_ADDRESSBOOK = 57, /* QUERY_FROM_FOUND_IN_ADDRESSBOOK  */
  YYSYMBOL_QUERY_TO_FOUND_IN_ADDRESSBOOK = 58, /* QUERY_TO_FOUND_IN_ADDRESSBOOK  */
  YYSYMBOL_QUERY_NOT_DATE = 59,            /* QUERY_NOT_DATE  */
  YYSYMBOL_QUERY_NOT_FROM = 60,            /* QUERY_NOT_FROM  */
  YYSYMBOL_QUERY_NOT_FULLNAME = 61,        /* QUERY_NOT_FULLNAME  */
  YYSYMBOL_QUERY_NOT_SUBJECT = 62,         /* QUERY_NOT_SUBJECT  */
  YYSYMBOL_QUERY_NOT_TO = 63,              /* QUERY_NOT_TO  */
  YYSYMBOL_QUERY_NOT_NEWSGROUPS = 64,      /* QUERY_NOT_NEWSGROUPS  */
  YYSYMBOL_QUERY_NOT_MESSAGEID = 65,       /* QUERY_NOT_MESSAGEID  */
  YYSYMBOL_QUERY_NOT_CC = 66,              /* QUERY_NOT_CC  */
  YYSYMBOL_QUERY_NOT_REFERENCES = 67,      /* QUERY_NOT_REFERENCES  */
  YYSYMBOL_QUERY_NOT_ACCOUNT_FULL_NAME = 68, /* QUERY_NOT_ACCOUNT_FULL_NAME  */
  YYSYMBOL_QUERY_NOT_ACCOUNT_ORGANIZATION = 69, /* QUERY_NOT_ACCOUNT_ORGANIZATION  */
  YYSYMBOL_QUERY_NOT_ACCOUNT_DICT = 70,    /* QUERY_NOT_ACCOUNT_DICT  */
  YYSYMBOL_QUERY_NOT_ACCOUNT_SIG = 71,     /* QUERY_NOT_ACCOUNT_SIG  */
  YYSYMBOL_QUERY_NOT_ACCOUNT_SIGPATH = 72, /* QUERY_NOT_ACCOUNT_SIGPATH  */
  YYSYMBOL_QUERY_NOT_DICT = 73,            /* QUERY_NOT_DICT  */
  YYSYMBOL_QUERY_NOT_CC_FOUND_IN_ADDRESSBOOK = 74, /* QUERY_NOT_CC_FOUND_IN_ADDRESSBOOK  */
  YYSYMBOL_QUERY_NOT_FROM_FOUND_IN_ADDRESSBOOK = 75, /* QUERY_NOT_FROM_FOUND_IN_ADDRESSBOOK  */
  YYSYMBOL_QUERY_NOT_TO_FOUND_IN_ADDRESSBOOK = 76, /* QUERY_NOT_TO_FOUND_IN_ADDRESSBOOK  */
  YYSYMBOL_INSERT_FILE = 77,               /* INSERT_FILE  */
  YYSYMBOL_INSERT_PROGRAMOUTPUT = 78,      /* INSERT_PROGRAMOUTPUT  */
  YYSYMBOL_INSERT_USERINPUT = 79,          /* INSERT_USERINPUT  */
  YYSYMBOL_ATTACH_FILE = 80,               /* ATTACH_FILE  */
  YYSYMBOL_ATTACH_PROGRAMOUTPUT = 81,      /* ATTACH_PROGRAMOUTPUT  */
  YYSYMBOL_OPARENT = 82,                   /* OPARENT  */
  YYSYMBOL_CPARENT = 83,                   /* CPARENT  */
  YYSYMBOL_CHARACTER = 84,                 /* CHARACTER  */
  YYSYMBOL_SHOW_DATE_EXPR = 85,            /* SHOW_DATE_EXPR  */
  YYSYMBOL_SET_CURSOR_POS = 86,            /* SET_CURSOR_POS  */
  YYSYMBOL_YYACCEPT = 87,                  /* $accept  */
  YYSYMBOL_quote_fmt = 88,                 /* quote_fmt  */
  YYSYMBOL_sub_expr = 89,                  /* sub_expr  */
  YYSYMBOL_character_or_special_or_insert_or_query_list = 90, /* character_or_special_or_insert_or_query_list  */
  YYSYMBOL_character_or_special_list = 91, /* character_or_special_list  */
  YYSYMBOL_character_or_special_or_insert_or_query = 92, /* character_or_special_or_insert_or_query  */
  YYSYMBOL_character_or_special = 93,      /* character_or_special  */
  YYSYMBOL_character = 94,                 /* character  */
  YYSYMBOL_string = 95,                    /* string  */
  YYSYMBOL_special = 96,                   /* special  */
  YYSYMBOL_query = 97,                     /* query  */
  YYSYMBOL_98_1 = 98,                      /* $@1  */
  YYSYMBOL_99_2 = 99,                      /* $@2  */
  YYSYMBOL_100_3 = 100,                    /* $@3  */
  YYSYMBOL_101_4 = 101,                    /* $@4  */
  YYSYMBOL_102_5 = 102,                    /* $@5  */
  YYSYMBOL_103_6 = 103,                    /* $@6  */
  YYSYMBOL_104_7 = 104,                    /* $@7  */
  YYSYMBOL_105_8 = 105,                    /* $@8  */
  YYSYMBOL_106_9 = 106,                    /* $@9  */
  YYSYMBOL_107_10 = 107,                   /* $@10  */
  YYSYMBOL_108_11 = 108,                   /* $@11  */
  YYSYMBOL_109_12 = 109,                   /* $@12  */
  YYSYMBOL_110_13 = 110,                   /* $@13  */
  YYSYMBOL_111_14 = 111,                   /* $@14  */
  YYSYMBOL_112_15 = 112,                   /* $@15  */
  YYSYMBOL_113_16 = 113,                   /* $@16  */
  YYSYMBOL_114_17 = 114,                   /* $@17  */
  YYSYMBOL_115_18 = 115,                   /* $@18  */
  YYSYMBOL_query_not = 116,                /* query_not  */
  YYSYMBOL_117_19 = 117,                   /* $@19  */
  YYSYMBOL_118_20 = 118,                   /* $@20  */
  YYSYMBOL_119_21 = 119,                   /* $@21  */
  YYSYMBOL_120_22 = 120,                   /* $@22  */
  YYSYMBOL_121_23 = 121,                   /* $@23  */
  YYSYMBOL_122_24 = 122,                   /* $@24  */
  YYSYMBOL_123_25 = 123,                   /* $@25  */
  YYSYMBOL_124_26 = 124,                   /* $@26  */
  YYSYMBOL_125_27 = 125,                   /* $@27  */
  YYSYMBOL_126_28 = 126,                   /* $@28  */
  YYSYMBOL_127_29 = 127,                   /* $@29  */
  YYSYMBOL_128_30 = 128,                   /* $@30  */
  YYSYMBOL_129_31 = 129,                   /* $@31  */
  YYSYMBOL_130_32 = 130,                   /* $@32  */
  YYSYMBOL_131_33 = 131,                   /* $@33  */
  YYSYMBOL_132_34 = 132,                   /* $@34  */
  YYSYMBOL_133_35 = 133,                   /* $@35  */
  YYSYMBOL_134_36 = 134,                   /* $@36  */
  YYSYMBOL_insert = 135,                   /* insert  */
  YYSYMBOL_136_37 = 136,                   /* $@37  */
  YYSYMBOL_137_38 = 137,                   /* $@38  */
  YYSYMBOL_138_39 = 138,                   /* $@39  */
  YYSYMBOL_attach = 139,                   /* attach  */
  YYSYMBOL_140_40 = 140,                   /* $@40  */
  YYSYMBOL_141_41 = 141                    /* $@41  */
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
#define YYFINAL  135
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   261

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  87
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  55
/* YYNRULES -- Number of rules.  */
#define YYNRULES  139
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  267

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   341


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
      85,    86
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   681,   681,   684,   687,   688,   691,   692,   695,   696,
     697,   698,   699,   702,   703,   709,   713,   718,   732,   737,
     741,   746,   751,   760,   765,   769,   773,   777,   782,   787,
     792,   796,   801,   810,   814,   818,   822,   826,   831,   836,
     841,   846,   852,   857,   867,   873,   881,   885,   889,   893,
     897,   901,   905,   909,   913,   920,   928,   936,   947,   946,
     955,   954,   963,   962,   971,   970,   979,   978,   987,   986,
     995,   994,  1003,  1002,  1011,  1010,  1026,  1025,  1034,  1033,
    1042,  1041,  1052,  1051,  1061,  1060,  1074,  1073,  1086,  1085,
    1096,  1095,  1106,  1105,  1118,  1117,  1126,  1125,  1134,  1133,
    1142,  1141,  1150,  1149,  1158,  1157,  1166,  1165,  1174,  1173,
    1182,  1181,  1197,  1196,  1205,  1204,  1213,  1212,  1223,  1222,
    1232,  1231,  1245,  1244,  1257,  1256,  1267,  1266,  1277,  1276,
    1289,  1288,  1301,  1300,  1313,  1312,  1327,  1326,  1339,  1338
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
  "\"end of file\"", "error", "\"invalid token\"", "SHOW_NEWSGROUPS",
  "SHOW_DATE", "SHOW_FROM", "SHOW_FULLNAME", "SHOW_FIRST_NAME",
  "SHOW_LAST_NAME", "SHOW_SENDER_INITIAL", "SHOW_SUBJECT", "SHOW_TO",
  "SHOW_MESSAGEID", "SHOW_PERCENT", "SHOW_CC", "SHOW_REFERENCES",
  "SHOW_MESSAGE", "SHOW_QUOTED_MESSAGE", "SHOW_BACKSLASH", "SHOW_TAB",
  "SHOW_MAIL_ADDRESS", "SHOW_QUOTED_MESSAGE_NO_SIGNATURE",
  "SHOW_MESSAGE_NO_SIGNATURE", "SHOW_EOL", "SHOW_QUESTION_MARK",
  "SHOW_EXCLAMATION_MARK", "SHOW_PIPE", "SHOW_OPARENT", "SHOW_CPARENT",
  "SHOW_ACCOUNT_FULL_NAME", "SHOW_ACCOUNT_MAIL_ADDRESS",
  "SHOW_ACCOUNT_NAME", "SHOW_ACCOUNT_ORGANIZATION", "SHOW_ACCOUNT_DICT",
  "SHOW_ACCOUNT_SIG", "SHOW_ACCOUNT_SIGPATH", "SHOW_DICT", "SHOW_TAGS",
  "SHOW_ADDRESSBOOK_COMPLETION_FOR_CC",
  "SHOW_ADDRESSBOOK_COMPLETION_FOR_FROM",
  "SHOW_ADDRESSBOOK_COMPLETION_FOR_TO", "QUERY_DATE", "QUERY_FROM",
  "QUERY_FULLNAME", "QUERY_SUBJECT", "QUERY_TO", "QUERY_NEWSGROUPS",
  "QUERY_MESSAGEID", "QUERY_CC", "QUERY_REFERENCES",
  "QUERY_ACCOUNT_FULL_NAME", "QUERY_ACCOUNT_ORGANIZATION",
  "QUERY_ACCOUNT_DICT", "QUERY_ACCOUNT_SIG", "QUERY_ACCOUNT_SIGPATH",
  "QUERY_DICT", "QUERY_CC_FOUND_IN_ADDRESSBOOK",
  "QUERY_FROM_FOUND_IN_ADDRESSBOOK", "QUERY_TO_FOUND_IN_ADDRESSBOOK",
  "QUERY_NOT_DATE", "QUERY_NOT_FROM", "QUERY_NOT_FULLNAME",
  "QUERY_NOT_SUBJECT", "QUERY_NOT_TO", "QUERY_NOT_NEWSGROUPS",
  "QUERY_NOT_MESSAGEID", "QUERY_NOT_CC", "QUERY_NOT_REFERENCES",
  "QUERY_NOT_ACCOUNT_FULL_NAME", "QUERY_NOT_ACCOUNT_ORGANIZATION",
  "QUERY_NOT_ACCOUNT_DICT", "QUERY_NOT_ACCOUNT_SIG",
  "QUERY_NOT_ACCOUNT_SIGPATH", "QUERY_NOT_DICT",
  "QUERY_NOT_CC_FOUND_IN_ADDRESSBOOK",
  "QUERY_NOT_FROM_FOUND_IN_ADDRESSBOOK",
  "QUERY_NOT_TO_FOUND_IN_ADDRESSBOOK", "INSERT_FILE",
  "INSERT_PROGRAMOUTPUT", "INSERT_USERINPUT", "ATTACH_FILE",
  "ATTACH_PROGRAMOUTPUT", "OPARENT", "CPARENT", "CHARACTER",
  "SHOW_DATE_EXPR", "SET_CURSOR_POS", "$accept", "quote_fmt", "sub_expr",
  "character_or_special_or_insert_or_query_list",
  "character_or_special_list", "character_or_special_or_insert_or_query",
  "character_or_special", "character", "string", "special", "query", "$@1",
  "$@2", "$@3", "$@4", "$@5", "$@6", "$@7", "$@8", "$@9", "$@10", "$@11",
  "$@12", "$@13", "$@14", "$@15", "$@16", "$@17", "$@18", "query_not",
  "$@19", "$@20", "$@21", "$@22", "$@23", "$@24", "$@25", "$@26", "$@27",
  "$@28", "$@29", "$@30", "$@31", "$@32", "$@33", "$@34", "$@35", "$@36",
  "insert", "$@37", "$@38", "$@39", "attach", "$@40", "$@41", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-51)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -3,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,
     -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,
     -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,
     -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,
     -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,
     -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,
     -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,
     -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,
     -51,    40,   -51,   164,   -51,    -3,   -51,   -51,   -51,   -51,
     -51,   -51,   -51,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   -51,   -51,    -3,    -3,    -3,
      -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,
      -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,
      -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,    -3,
      -3,    -3,    -3,    81,    81,    81,    81,    81,   -51,    -4,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     165,   166,   167,   168,   169,   170,   171,   -51,    81,   172,
     173,   174,   175,   -51,   -51,   -51,   -51,   -51,   -51,   -51,
     -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,
     -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,
     -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,
     -51,   -51,   -51,   -51,   -51,   -51,   -51
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    18,    20,    21,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    46,    47,    22,    36,
      35,    48,    49,    50,    51,    52,    53,    37,    38,    39,
      40,    43,    41,    42,    44,    45,    55,    56,    57,    58,
      60,    62,    64,    66,    68,    70,    72,    74,    76,    78,
      84,    80,    82,    86,    88,    90,    92,    94,    96,    98,
     100,   102,   104,   106,   108,   110,   112,   114,   120,   116,
     118,   122,   124,   126,   128,   130,   132,   134,   136,   138,
      15,     0,    54,     0,     2,     5,     8,    14,    13,     9,
      10,    11,    12,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     1,     4,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    16,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     3,     7,     0,
       0,     0,     0,    19,    17,    59,    61,    63,    65,    67,
      69,    71,    73,    75,    77,    79,    85,    81,    83,    87,
      89,    91,    93,    95,    97,    99,   101,   103,   105,   107,
     109,   111,   113,   115,   121,   117,   119,   123,   125,   127,
     129,   131,     6,   133,   135,   137,   139
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -51,    -9,    -5,   176,    29,   -51,   -50,   -51,   -51,   -51,
     -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,
     -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,
     -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,
     -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,   -51,
     -51,   -51,   -51,   -51,   -51
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    83,   216,    84,   217,    85,    86,    87,   179,    88,
      89,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   105,   106,   104,   107,   108,   109,   110,    90,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   123,   124,   122,   125,   126,   127,   128,    91,   129,
     130,   131,    92,   132,   133
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
       1,     2,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,   223,
     224,    80,    81,    82,     1,     2,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,   134,   218,   218,   218,   218,   218,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   135,    80,    81,    82,   218,   219,
     220,   221,   222,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,     0,     0,   178,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,   240,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   262,   255,   256,
     257,   258,   259,   260,   261,   263,   264,   265,   266,     0,
       0,   136
};

static const yytype_int16 yycheck[] =
{
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    83,
      84,    84,    85,    86,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    82,   173,   174,   175,   176,   177,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,     0,    84,    85,    86,   218,   174,
     175,   176,   177,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    -1,    -1,    84,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,   218,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    -1,
      -1,    85
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      84,    85,    86,    88,    90,    92,    93,    94,    96,    97,
     116,   135,   139,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   111,   109,   110,   112,   113,   114,
     115,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   130,   128,   129,   131,   132,   133,   134,   136,
     137,   138,   140,   141,    82,     0,    90,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82,    82,    84,    95,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    89,    91,    93,    89,
      89,    89,    89,    83,    84,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    91,    83,    83,    83,    83
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    87,    88,    89,    90,    90,    91,    91,    92,    92,
      92,    92,    92,    93,    93,    94,    95,    95,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    98,    97,
      99,    97,   100,    97,   101,    97,   102,    97,   103,    97,
     104,    97,   105,    97,   106,    97,   107,    97,   108,    97,
     109,    97,   110,    97,   111,    97,   112,    97,   113,    97,
     114,    97,   115,    97,   117,   116,   118,   116,   119,   116,
     120,   116,   121,   116,   122,   116,   123,   116,   124,   116,
     125,   116,   126,   116,   127,   116,   128,   116,   129,   116,
     130,   116,   131,   116,   132,   116,   133,   116,   134,   116,
     136,   135,   137,   135,   138,   135,   140,   139,   141,   139
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     5,
       0,     5,     0,     5,     0,     5,     0,     5,     0,     5,
       0,     5,     0,     5,     0,     5,     0,     5,     0,     5,
       0,     5,     0,     5,     0,     5,     0,     5,     0,     5,
       0,     5,     0,     5,     0,     5,     0,     5,     0,     5,
       0,     5,     0,     5,     0,     5,     0,     5,     0,     5,
       0,     5,     0,     5,     0,     5,     0,     5,     0,     5,
       0,     5,     0,     5,     0,     5,     0,     5,     0,     5,
       0,     5,     0,     5,     0,     5,     0,     5,     0,     5
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
  case 14: /* character_or_special: character  */
#line 704 "quote_fmt_parse.y"
        {
		INSERT_CHARACTER((yyvsp[0].chr));
	}
#line 2240 "quote_fmt_parse.c"
    break;

  case 16: /* string: CHARACTER  */
#line 714 "quote_fmt_parse.y"
        {
		(yyval.str)[0] = (yyvsp[0].chr);
		(yyval.str)[1] = '\0';
	}
#line 2249 "quote_fmt_parse.c"
    break;

  case 17: /* string: string CHARACTER  */
#line 719 "quote_fmt_parse.y"
        {
		size_t len;
		
		strncpy((yyval.str), (yyvsp[-1].str), sizeof((yyval.str)));
		(yyval.str)[sizeof((yyval.str)) - 1] = '\0';
		len = strlen((yyval.str));
		if (len + 1 < sizeof((yyval.str))) {
			(yyval.str)[len + 1] = '\0';
			(yyval.str)[len] = (yyvsp[0].chr);
		}
	}
#line 2265 "quote_fmt_parse.c"
    break;

  case 18: /* special: SHOW_NEWSGROUPS  */
#line 733 "quote_fmt_parse.y"
        {
		if (msginfo->newsgroups)
			INSERT(msginfo->newsgroups);
	}
#line 2274 "quote_fmt_parse.c"
    break;

  case 19: /* special: SHOW_DATE_EXPR OPARENT string CPARENT  */
#line 738 "quote_fmt_parse.y"
        {
		quote_fmt_show_date(msginfo, (yyvsp[-1].str));
	}
#line 2282 "quote_fmt_parse.c"
    break;

  case 20: /* special: SHOW_DATE  */
#line 742 "quote_fmt_parse.y"
        {
		if (msginfo->date)
			INSERT(msginfo->date);
	}
#line 2291 "quote_fmt_parse.c"
    break;

  case 21: /* special: SHOW_FROM  */
#line 747 "quote_fmt_parse.y"
        {
		if (msginfo->from)
			INSERT(msginfo->from);
	}
#line 2300 "quote_fmt_parse.c"
    break;

  case 22: /* special: SHOW_MAIL_ADDRESS  */
#line 752 "quote_fmt_parse.y"
        {
		if (msginfo->from) {
			gchar *stripped_address = g_strdup(msginfo->from);
			extract_address(stripped_address);
			INSERT(stripped_address);
			g_free(stripped_address);
		}
	}
#line 2313 "quote_fmt_parse.c"
    break;

  case 23: /* special: SHOW_FULLNAME  */
#line 761 "quote_fmt_parse.y"
        {
		if (msginfo->fromname)
			INSERT(msginfo->fromname);
	}
#line 2322 "quote_fmt_parse.c"
    break;

  case 24: /* special: SHOW_FIRST_NAME  */
#line 766 "quote_fmt_parse.y"
        {
		quote_fmt_show_first_name(msginfo);
	}
#line 2330 "quote_fmt_parse.c"
    break;

  case 25: /* special: SHOW_LAST_NAME  */
#line 770 "quote_fmt_parse.y"
    {
		quote_fmt_show_last_name(msginfo);
	}
#line 2338 "quote_fmt_parse.c"
    break;

  case 26: /* special: SHOW_SENDER_INITIAL  */
#line 774 "quote_fmt_parse.y"
        {
		quote_fmt_show_sender_initial(msginfo);
	}
#line 2346 "quote_fmt_parse.c"
    break;

  case 27: /* special: SHOW_SUBJECT  */
#line 778 "quote_fmt_parse.y"
        {
		if (msginfo->subject)
			INSERT(msginfo->subject);
	}
#line 2355 "quote_fmt_parse.c"
    break;

  case 28: /* special: SHOW_TO  */
#line 783 "quote_fmt_parse.y"
        {
		if (msginfo->to)
			INSERT(msginfo->to);
	}
#line 2364 "quote_fmt_parse.c"
    break;

  case 29: /* special: SHOW_MESSAGEID  */
#line 788 "quote_fmt_parse.y"
        {
		if (msginfo->msgid)
			INSERT(msginfo->msgid);
	}
#line 2373 "quote_fmt_parse.c"
    break;

  case 30: /* special: SHOW_PERCENT  */
#line 793 "quote_fmt_parse.y"
        {
		INSERT("%");
	}
#line 2381 "quote_fmt_parse.c"
    break;

  case 31: /* special: SHOW_CC  */
#line 797 "quote_fmt_parse.y"
        {
		if (msginfo->cc)
			INSERT(msginfo->cc);
	}
#line 2390 "quote_fmt_parse.c"
    break;

  case 32: /* special: SHOW_REFERENCES  */
#line 802 "quote_fmt_parse.y"
        {
		GSList *item;

		INSERT(msginfo->inreplyto);
		for (item = msginfo->references; item != NULL; item = g_slist_next(item))
			if (item->data)
				INSERT(item->data);
	}
#line 2403 "quote_fmt_parse.c"
    break;

  case 33: /* special: SHOW_MESSAGE  */
#line 811 "quote_fmt_parse.y"
        {
		quote_fmt_show_msg(msginfo, body, FALSE, TRUE, quote_str);
	}
#line 2411 "quote_fmt_parse.c"
    break;

  case 34: /* special: SHOW_QUOTED_MESSAGE  */
#line 815 "quote_fmt_parse.y"
        {
		quote_fmt_show_msg(msginfo, body, TRUE, TRUE, quote_str);
	}
#line 2419 "quote_fmt_parse.c"
    break;

  case 35: /* special: SHOW_MESSAGE_NO_SIGNATURE  */
#line 819 "quote_fmt_parse.y"
        {
		quote_fmt_show_msg(msginfo, body, FALSE, FALSE, quote_str);
	}
#line 2427 "quote_fmt_parse.c"
    break;

  case 36: /* special: SHOW_QUOTED_MESSAGE_NO_SIGNATURE  */
#line 823 "quote_fmt_parse.y"
        {
		quote_fmt_show_msg(msginfo, body, TRUE, FALSE, quote_str);
	}
#line 2435 "quote_fmt_parse.c"
    break;

  case 37: /* special: SHOW_ACCOUNT_FULL_NAME  */
#line 827 "quote_fmt_parse.y"
        {
		if (account && account->name)
			INSERT(account->name);
	}
#line 2444 "quote_fmt_parse.c"
    break;

  case 38: /* special: SHOW_ACCOUNT_MAIL_ADDRESS  */
#line 832 "quote_fmt_parse.y"
        {
		if (account && account->address)
			INSERT(account->address);
	}
#line 2453 "quote_fmt_parse.c"
    break;

  case 39: /* special: SHOW_ACCOUNT_NAME  */
#line 837 "quote_fmt_parse.y"
        {
		if (account && account->account_name)
			INSERT(account->account_name);
	}
#line 2462 "quote_fmt_parse.c"
    break;

  case 40: /* special: SHOW_ACCOUNT_ORGANIZATION  */
#line 842 "quote_fmt_parse.y"
        {
		if (account && account->organization)
			INSERT(account->organization);
	}
#line 2471 "quote_fmt_parse.c"
    break;

  case 41: /* special: SHOW_ACCOUNT_SIG  */
#line 847 "quote_fmt_parse.y"
        {
		gchar *str = account_get_signature_str(account);
		INSERT(str);
		g_free(str);
	}
#line 2481 "quote_fmt_parse.c"
    break;

  case 42: /* special: SHOW_ACCOUNT_SIGPATH  */
#line 853 "quote_fmt_parse.y"
        {
		if (account && account->sig_path)
			INSERT(account->sig_path);
	}
#line 2490 "quote_fmt_parse.c"
    break;

  case 43: /* special: SHOW_ACCOUNT_DICT  */
#line 858 "quote_fmt_parse.y"
        {
#ifdef USE_ENCHANT
		if (account && account->enable_default_dictionary) {
			gchar *dictname = g_path_get_basename(account->default_dictionary);
			INSERT(dictname);
			g_free(dictname);
		}
#endif
	}
#line 2504 "quote_fmt_parse.c"
    break;

  case 44: /* special: SHOW_DICT  */
#line 868 "quote_fmt_parse.y"
        {
#ifdef USE_ENCHANT
		INSERT(default_dictionary);
#endif
	}
#line 2514 "quote_fmt_parse.c"
    break;

  case 45: /* special: SHOW_TAGS  */
#line 874 "quote_fmt_parse.y"
        {
		gchar *tags = procmsg_msginfo_get_tags_str(msginfo);
		if (tags) {
			INSERT(tags);
		}
		g_free(tags);
	}
#line 2526 "quote_fmt_parse.c"
    break;

  case 46: /* special: SHOW_BACKSLASH  */
#line 882 "quote_fmt_parse.y"
        {
		INSERT("\\");
	}
#line 2534 "quote_fmt_parse.c"
    break;

  case 47: /* special: SHOW_TAB  */
#line 886 "quote_fmt_parse.y"
        {
		INSERT("\t");
	}
#line 2542 "quote_fmt_parse.c"
    break;

  case 48: /* special: SHOW_EOL  */
#line 890 "quote_fmt_parse.y"
        {
		INSERT("\n");
	}
#line 2550 "quote_fmt_parse.c"
    break;

  case 49: /* special: SHOW_QUESTION_MARK  */
#line 894 "quote_fmt_parse.y"
        {
		INSERT("?");
	}
#line 2558 "quote_fmt_parse.c"
    break;

  case 50: /* special: SHOW_EXCLAMATION_MARK  */
#line 898 "quote_fmt_parse.y"
        {
		INSERT("!");
	}
#line 2566 "quote_fmt_parse.c"
    break;

  case 51: /* special: SHOW_PIPE  */
#line 902 "quote_fmt_parse.y"
        {
		INSERT("|");
	}
#line 2574 "quote_fmt_parse.c"
    break;

  case 52: /* special: SHOW_OPARENT  */
#line 906 "quote_fmt_parse.y"
        {
		INSERT("{");
	}
#line 2582 "quote_fmt_parse.c"
    break;

  case 53: /* special: SHOW_CPARENT  */
#line 910 "quote_fmt_parse.y"
        {
		INSERT("}");
	}
#line 2590 "quote_fmt_parse.c"
    break;

  case 54: /* special: SET_CURSOR_POS  */
#line 914 "quote_fmt_parse.y"
        {
		if (current->buffer)
			cursor_pos = g_utf8_strlen(current->buffer, -1);
		else
			cursor_pos = 0;
	}
#line 2601 "quote_fmt_parse.c"
    break;

  case 55: /* special: SHOW_ADDRESSBOOK_COMPLETION_FOR_CC  */
#line 921 "quote_fmt_parse.y"
        {
		gchar *tmp = quote_fmt_complete_address(msginfo->cc);
		if (tmp) {
			INSERT(tmp);
			g_free(tmp);
		}
	}
#line 2613 "quote_fmt_parse.c"
    break;

  case 56: /* special: SHOW_ADDRESSBOOK_COMPLETION_FOR_FROM  */
#line 929 "quote_fmt_parse.y"
        {
		gchar *tmp = quote_fmt_complete_address(msginfo->from);
		if (tmp) {
			INSERT(tmp);
			g_free(tmp);
		}
	}
#line 2625 "quote_fmt_parse.c"
    break;

  case 57: /* special: SHOW_ADDRESSBOOK_COMPLETION_FOR_TO  */
#line 937 "quote_fmt_parse.y"
        {
		gchar *tmp = quote_fmt_complete_address(msginfo->to);
		if (tmp) {
			INSERT(tmp);
			g_free(tmp);
		}
	}
#line 2637 "quote_fmt_parse.c"
    break;

  case 58: /* $@1: %empty  */
#line 947 "quote_fmt_parse.y"
        {
		add_visibility(msginfo->date != NULL);
	}
#line 2645 "quote_fmt_parse.c"
    break;

  case 59: /* query: QUERY_DATE $@1 OPARENT quote_fmt CPARENT  */
#line 951 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 2653 "quote_fmt_parse.c"
    break;

  case 60: /* $@2: %empty  */
#line 955 "quote_fmt_parse.y"
        {
		add_visibility(msginfo->from != NULL);
	}
#line 2661 "quote_fmt_parse.c"
    break;

  case 61: /* query: QUERY_FROM $@2 OPARENT quote_fmt CPARENT  */
#line 959 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 2669 "quote_fmt_parse.c"
    break;

  case 62: /* $@3: %empty  */
#line 963 "quote_fmt_parse.y"
        {
		add_visibility(msginfo->fromname != NULL);
	}
#line 2677 "quote_fmt_parse.c"
    break;

  case 63: /* query: QUERY_FULLNAME $@3 OPARENT quote_fmt CPARENT  */
#line 967 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 2685 "quote_fmt_parse.c"
    break;

  case 64: /* $@4: %empty  */
#line 971 "quote_fmt_parse.y"
        {
		add_visibility(msginfo->subject != NULL);
	}
#line 2693 "quote_fmt_parse.c"
    break;

  case 65: /* query: QUERY_SUBJECT $@4 OPARENT quote_fmt CPARENT  */
#line 975 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 2701 "quote_fmt_parse.c"
    break;

  case 66: /* $@5: %empty  */
#line 979 "quote_fmt_parse.y"
        {
		add_visibility(msginfo->to != NULL);
	}
#line 2709 "quote_fmt_parse.c"
    break;

  case 67: /* query: QUERY_TO $@5 OPARENT quote_fmt CPARENT  */
#line 983 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 2717 "quote_fmt_parse.c"
    break;

  case 68: /* $@6: %empty  */
#line 987 "quote_fmt_parse.y"
        {
		add_visibility(msginfo->newsgroups != NULL);
	}
#line 2725 "quote_fmt_parse.c"
    break;

  case 69: /* query: QUERY_NEWSGROUPS $@6 OPARENT quote_fmt CPARENT  */
#line 991 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 2733 "quote_fmt_parse.c"
    break;

  case 70: /* $@7: %empty  */
#line 995 "quote_fmt_parse.y"
        {
		add_visibility(msginfo->msgid != NULL);
	}
#line 2741 "quote_fmt_parse.c"
    break;

  case 71: /* query: QUERY_MESSAGEID $@7 OPARENT quote_fmt CPARENT  */
#line 999 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 2749 "quote_fmt_parse.c"
    break;

  case 72: /* $@8: %empty  */
#line 1003 "quote_fmt_parse.y"
        {
		add_visibility(msginfo->cc != NULL);
	}
#line 2757 "quote_fmt_parse.c"
    break;

  case 73: /* query: QUERY_CC $@8 OPARENT quote_fmt CPARENT  */
#line 1007 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 2765 "quote_fmt_parse.c"
    break;

  case 74: /* $@9: %empty  */
#line 1011 "quote_fmt_parse.y"
        {
		gboolean found;
		GSList *item;

		found = (msginfo->inreplyto != NULL);
		for (item = msginfo->references; found == FALSE && item != NULL; item = g_slist_next(item))
			if (item->data)
				found = TRUE;
		add_visibility(found == TRUE);
	}
#line 2780 "quote_fmt_parse.c"
    break;

  case 75: /* query: QUERY_REFERENCES $@9 OPARENT quote_fmt CPARENT  */
#line 1022 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 2788 "quote_fmt_parse.c"
    break;

  case 76: /* $@10: %empty  */
#line 1026 "quote_fmt_parse.y"
        {
		add_visibility(account != NULL && account->name != NULL);
	}
#line 2796 "quote_fmt_parse.c"
    break;

  case 77: /* query: QUERY_ACCOUNT_FULL_NAME $@10 OPARENT quote_fmt CPARENT  */
#line 1030 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 2804 "quote_fmt_parse.c"
    break;

  case 78: /* $@11: %empty  */
#line 1034 "quote_fmt_parse.y"
        {
		add_visibility(account != NULL && account->organization != NULL);
	}
#line 2812 "quote_fmt_parse.c"
    break;

  case 79: /* query: QUERY_ACCOUNT_ORGANIZATION $@11 OPARENT quote_fmt CPARENT  */
#line 1038 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 2820 "quote_fmt_parse.c"
    break;

  case 80: /* $@12: %empty  */
#line 1042 "quote_fmt_parse.y"
        {
		gchar *str = account_get_signature_str(account);
		add_visibility(str != NULL && * str != '\0');
		g_free(str);
	}
#line 2830 "quote_fmt_parse.c"
    break;

  case 81: /* query: QUERY_ACCOUNT_SIG $@12 OPARENT quote_fmt CPARENT  */
#line 1048 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 2838 "quote_fmt_parse.c"
    break;

  case 82: /* $@13: %empty  */
#line 1052 "quote_fmt_parse.y"
        {
		add_visibility(account != NULL && account->sig_path != NULL
				&& *account->sig_path != '\0');
	}
#line 2847 "quote_fmt_parse.c"
    break;

  case 83: /* query: QUERY_ACCOUNT_SIGPATH $@13 OPARENT quote_fmt CPARENT  */
#line 1057 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 2855 "quote_fmt_parse.c"
    break;

  case 84: /* $@14: %empty  */
#line 1061 "quote_fmt_parse.y"
        {
#ifdef USE_ENCHANT
		add_visibility(account != NULL && account->enable_default_dictionary == TRUE &&
				account->default_dictionary != NULL && *account->default_dictionary != '\0');
#else
		add_visibility(FALSE);
#endif
	}
#line 2868 "quote_fmt_parse.c"
    break;

  case 85: /* query: QUERY_ACCOUNT_DICT $@14 OPARENT quote_fmt CPARENT  */
#line 1070 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 2876 "quote_fmt_parse.c"
    break;

  case 86: /* $@15: %empty  */
#line 1074 "quote_fmt_parse.y"
        {
#ifdef USE_ENCHANT
		add_visibility(*default_dictionary != '\0');
#else
		add_visibility(FALSE);
#endif
	}
#line 2888 "quote_fmt_parse.c"
    break;

  case 87: /* query: QUERY_DICT $@15 OPARENT quote_fmt CPARENT  */
#line 1082 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 2896 "quote_fmt_parse.c"
    break;

  case 88: /* $@16: %empty  */
#line 1086 "quote_fmt_parse.y"
        {
		gchar *tmp = quote_fmt_complete_address(msginfo->cc);
		add_visibility(tmp != NULL && *tmp != '\0');
		g_free(tmp);
	}
#line 2906 "quote_fmt_parse.c"
    break;

  case 89: /* query: QUERY_CC_FOUND_IN_ADDRESSBOOK $@16 OPARENT quote_fmt CPARENT  */
#line 1092 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 2914 "quote_fmt_parse.c"
    break;

  case 90: /* $@17: %empty  */
#line 1096 "quote_fmt_parse.y"
        {
		gchar *tmp = quote_fmt_complete_address(msginfo->from);
		add_visibility(tmp != NULL && *tmp != '\0');
		g_free(tmp);
	}
#line 2924 "quote_fmt_parse.c"
    break;

  case 91: /* query: QUERY_FROM_FOUND_IN_ADDRESSBOOK $@17 OPARENT quote_fmt CPARENT  */
#line 1102 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 2932 "quote_fmt_parse.c"
    break;

  case 92: /* $@18: %empty  */
#line 1106 "quote_fmt_parse.y"
        {
		gchar *tmp = quote_fmt_complete_address(msginfo->to);
		add_visibility(tmp != NULL && *tmp != '\0');
		g_free(tmp);
	}
#line 2942 "quote_fmt_parse.c"
    break;

  case 93: /* query: QUERY_TO_FOUND_IN_ADDRESSBOOK $@18 OPARENT quote_fmt CPARENT  */
#line 1112 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 2950 "quote_fmt_parse.c"
    break;

  case 94: /* $@19: %empty  */
#line 1118 "quote_fmt_parse.y"
        {
		add_visibility(msginfo->date == NULL);
	}
#line 2958 "quote_fmt_parse.c"
    break;

  case 95: /* query_not: QUERY_NOT_DATE $@19 OPARENT quote_fmt CPARENT  */
#line 1122 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 2966 "quote_fmt_parse.c"
    break;

  case 96: /* $@20: %empty  */
#line 1126 "quote_fmt_parse.y"
        {
		add_visibility(msginfo->from == NULL);
	}
#line 2974 "quote_fmt_parse.c"
    break;

  case 97: /* query_not: QUERY_NOT_FROM $@20 OPARENT quote_fmt CPARENT  */
#line 1130 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 2982 "quote_fmt_parse.c"
    break;

  case 98: /* $@21: %empty  */
#line 1134 "quote_fmt_parse.y"
        {
		add_visibility(msginfo->fromname == NULL);
	}
#line 2990 "quote_fmt_parse.c"
    break;

  case 99: /* query_not: QUERY_NOT_FULLNAME $@21 OPARENT quote_fmt CPARENT  */
#line 1138 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 2998 "quote_fmt_parse.c"
    break;

  case 100: /* $@22: %empty  */
#line 1142 "quote_fmt_parse.y"
        {
		add_visibility(msginfo->subject == NULL);
	}
#line 3006 "quote_fmt_parse.c"
    break;

  case 101: /* query_not: QUERY_NOT_SUBJECT $@22 OPARENT quote_fmt CPARENT  */
#line 1146 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 3014 "quote_fmt_parse.c"
    break;

  case 102: /* $@23: %empty  */
#line 1150 "quote_fmt_parse.y"
        {
		add_visibility(msginfo->to == NULL);
	}
#line 3022 "quote_fmt_parse.c"
    break;

  case 103: /* query_not: QUERY_NOT_TO $@23 OPARENT quote_fmt CPARENT  */
#line 1154 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 3030 "quote_fmt_parse.c"
    break;

  case 104: /* $@24: %empty  */
#line 1158 "quote_fmt_parse.y"
        {
		add_visibility(msginfo->newsgroups == NULL);
	}
#line 3038 "quote_fmt_parse.c"
    break;

  case 105: /* query_not: QUERY_NOT_NEWSGROUPS $@24 OPARENT quote_fmt CPARENT  */
#line 1162 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 3046 "quote_fmt_parse.c"
    break;

  case 106: /* $@25: %empty  */
#line 1166 "quote_fmt_parse.y"
        {
		add_visibility(msginfo->msgid == NULL);
	}
#line 3054 "quote_fmt_parse.c"
    break;

  case 107: /* query_not: QUERY_NOT_MESSAGEID $@25 OPARENT quote_fmt CPARENT  */
#line 1170 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 3062 "quote_fmt_parse.c"
    break;

  case 108: /* $@26: %empty  */
#line 1174 "quote_fmt_parse.y"
        {
		add_visibility(msginfo->cc == NULL);
	}
#line 3070 "quote_fmt_parse.c"
    break;

  case 109: /* query_not: QUERY_NOT_CC $@26 OPARENT quote_fmt CPARENT  */
#line 1178 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 3078 "quote_fmt_parse.c"
    break;

  case 110: /* $@27: %empty  */
#line 1182 "quote_fmt_parse.y"
        {
		gboolean found;
		GSList *item;

		found = (msginfo->inreplyto != NULL);
		for (item = msginfo->references; found == FALSE && item != NULL; item = g_slist_next(item))
			if (item->data)
				found = TRUE;
		add_visibility(found == FALSE);
	}
#line 3093 "quote_fmt_parse.c"
    break;

  case 111: /* query_not: QUERY_NOT_REFERENCES $@27 OPARENT quote_fmt CPARENT  */
#line 1193 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 3101 "quote_fmt_parse.c"
    break;

  case 112: /* $@28: %empty  */
#line 1197 "quote_fmt_parse.y"
        {
		add_visibility(account == NULL || account->name == NULL);
	}
#line 3109 "quote_fmt_parse.c"
    break;

  case 113: /* query_not: QUERY_NOT_ACCOUNT_FULL_NAME $@28 OPARENT quote_fmt CPARENT  */
#line 1201 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 3117 "quote_fmt_parse.c"
    break;

  case 114: /* $@29: %empty  */
#line 1205 "quote_fmt_parse.y"
        {
		add_visibility(account == NULL || account->organization == NULL);
	}
#line 3125 "quote_fmt_parse.c"
    break;

  case 115: /* query_not: QUERY_NOT_ACCOUNT_ORGANIZATION $@29 OPARENT quote_fmt CPARENT  */
#line 1209 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 3133 "quote_fmt_parse.c"
    break;

  case 116: /* $@30: %empty  */
#line 1213 "quote_fmt_parse.y"
        {
		gchar *str = account_get_signature_str(account);
		add_visibility(str == NULL || *str == '\0');
		g_free(str);
	}
#line 3143 "quote_fmt_parse.c"
    break;

  case 117: /* query_not: QUERY_NOT_ACCOUNT_SIG $@30 OPARENT quote_fmt CPARENT  */
#line 1219 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 3151 "quote_fmt_parse.c"
    break;

  case 118: /* $@31: %empty  */
#line 1223 "quote_fmt_parse.y"
        {
		add_visibility(account == NULL || account->sig_path == NULL
				|| *account->sig_path == '\0');
	}
#line 3160 "quote_fmt_parse.c"
    break;

  case 119: /* query_not: QUERY_NOT_ACCOUNT_SIGPATH $@31 OPARENT quote_fmt CPARENT  */
#line 1228 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 3168 "quote_fmt_parse.c"
    break;

  case 120: /* $@32: %empty  */
#line 1232 "quote_fmt_parse.y"
        {
#ifdef USE_ENCHANT
		add_visibility(account == NULL || account->enable_default_dictionary == FALSE
				|| *account->default_dictionary == '\0');
#else
		add_visibility(FALSE);
#endif
	}
#line 3181 "quote_fmt_parse.c"
    break;

  case 121: /* query_not: QUERY_NOT_ACCOUNT_DICT $@32 OPARENT quote_fmt CPARENT  */
#line 1241 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 3189 "quote_fmt_parse.c"
    break;

  case 122: /* $@33: %empty  */
#line 1245 "quote_fmt_parse.y"
        {
#ifdef USE_ENCHANT
		add_visibility(*default_dictionary == '\0');
#else
		add_visibility(FALSE);
#endif
	}
#line 3201 "quote_fmt_parse.c"
    break;

  case 123: /* query_not: QUERY_NOT_DICT $@33 OPARENT quote_fmt CPARENT  */
#line 1253 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 3209 "quote_fmt_parse.c"
    break;

  case 124: /* $@34: %empty  */
#line 1257 "quote_fmt_parse.y"
        {
		gchar *tmp = quote_fmt_complete_address(msginfo->cc);
		add_visibility(tmp == NULL || *tmp == '\0');
		g_free(tmp);
	}
#line 3219 "quote_fmt_parse.c"
    break;

  case 125: /* query_not: QUERY_NOT_CC_FOUND_IN_ADDRESSBOOK $@34 OPARENT quote_fmt CPARENT  */
#line 1263 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 3227 "quote_fmt_parse.c"
    break;

  case 126: /* $@35: %empty  */
#line 1267 "quote_fmt_parse.y"
        {
		gchar *tmp = quote_fmt_complete_address(msginfo->from);
		add_visibility(tmp == NULL || *tmp == '\0');
		g_free(tmp);
	}
#line 3237 "quote_fmt_parse.c"
    break;

  case 127: /* query_not: QUERY_NOT_FROM_FOUND_IN_ADDRESSBOOK $@35 OPARENT quote_fmt CPARENT  */
#line 1273 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 3245 "quote_fmt_parse.c"
    break;

  case 128: /* $@36: %empty  */
#line 1277 "quote_fmt_parse.y"
        {
		gchar *tmp = quote_fmt_complete_address(msginfo->to);
		add_visibility(tmp == NULL || *tmp == '\0');
		g_free(tmp);
	}
#line 3255 "quote_fmt_parse.c"
    break;

  case 129: /* query_not: QUERY_NOT_TO_FOUND_IN_ADDRESSBOOK $@36 OPARENT quote_fmt CPARENT  */
#line 1283 "quote_fmt_parse.y"
        {
		remove_visibility();
	}
#line 3263 "quote_fmt_parse.c"
    break;

  case 130: /* $@37: %empty  */
#line 1289 "quote_fmt_parse.y"
        {
		current = &sub_expr;
		clear_buffer();
	}
#line 3272 "quote_fmt_parse.c"
    break;

  case 131: /* insert: INSERT_FILE $@37 OPARENT sub_expr CPARENT  */
#line 1294 "quote_fmt_parse.y"
        {
		current = &main_expr;
		if (!dry_run) {
			quote_fmt_insert_file(sub_expr.buffer);
		}
	}
#line 3283 "quote_fmt_parse.c"
    break;

  case 132: /* $@38: %empty  */
#line 1301 "quote_fmt_parse.y"
        {
		current = &sub_expr;
		clear_buffer();
	}
#line 3292 "quote_fmt_parse.c"
    break;

  case 133: /* insert: INSERT_PROGRAMOUTPUT $@38 OPARENT sub_expr CPARENT  */
#line 1306 "quote_fmt_parse.y"
        {
		current = &main_expr;
		if (!dry_run) {
			quote_fmt_insert_program_output(sub_expr.buffer);
		}
	}
#line 3303 "quote_fmt_parse.c"
    break;

  case 134: /* $@39: %empty  */
#line 1313 "quote_fmt_parse.y"
        {
		current = &sub_expr;
		clear_buffer();
	}
#line 3312 "quote_fmt_parse.c"
    break;

  case 135: /* insert: INSERT_USERINPUT $@39 OPARENT sub_expr CPARENT  */
#line 1318 "quote_fmt_parse.y"
        {
		current = &main_expr;
		if (!dry_run) {
			quote_fmt_insert_user_input(sub_expr.buffer);
		}
	}
#line 3323 "quote_fmt_parse.c"
    break;

  case 136: /* $@40: %empty  */
#line 1327 "quote_fmt_parse.y"
        {
		current = &sub_expr;
		clear_buffer();
	}
#line 3332 "quote_fmt_parse.c"
    break;

  case 137: /* attach: ATTACH_FILE $@40 OPARENT sub_expr CPARENT  */
#line 1332 "quote_fmt_parse.y"
        {
		current = &main_expr;
		if (!dry_run) {
			quote_fmt_attach_file(sub_expr.buffer);
		}
	}
#line 3343 "quote_fmt_parse.c"
    break;

  case 138: /* $@41: %empty  */
#line 1339 "quote_fmt_parse.y"
        {
		current = &sub_expr;
		clear_buffer();
	}
#line 3352 "quote_fmt_parse.c"
    break;

  case 139: /* attach: ATTACH_PROGRAMOUTPUT $@41 OPARENT sub_expr CPARENT  */
#line 1344 "quote_fmt_parse.y"
        {
		current = &main_expr;
		if (!dry_run) {
			quote_fmt_attach_file_program_output(sub_expr.buffer);
		}
	}
#line 3363 "quote_fmt_parse.c"
    break;


#line 3367 "quote_fmt_parse.c"

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

