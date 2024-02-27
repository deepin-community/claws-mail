/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

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

#line 244 "quote_fmt_parse.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_QUOTE_FMT_PARSE_H_INCLUDED  */
