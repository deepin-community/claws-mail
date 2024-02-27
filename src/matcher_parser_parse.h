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

#line 314 "matcher_parser_parse.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_MATCHER_PARSER_PARSE_H_INCLUDED  */
