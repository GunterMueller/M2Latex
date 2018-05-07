/*
 *  Modula2LaTeX 1.0:
 *  Produce pretty printed LaTeX files from Modula-2 and Pascal sources.
 *
 *  Copyright (C) 1991 Joerg Heitkoetter
 *  Systems Analysis Group, University of Dortmund (UNIDO)
 *  (heitk...@gorbi.informatik.uni-dortmund.de).
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 1, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

void
substitute (input)
char   *input;
{
       while (*input) {
               switch (*input) {
                       case '_':
                       case '&':
                       case '#':
                       case '$':
                       case '%':
                       case '{':
                       case '}':
                               printf ("\\%c", *input);
                               break;
                       case '+':
                       case '=':
                       case '<':
                       case '>':
                               printf ("$%c$", *input);
                               break;
                       case '*':
                               printf ("$\\ast$");
                               break;
                       case '|':
                               printf ("$\\mid$");
                               break;
                       case '\\':
                               printf ("$\\backslash$");
                               break;
                       case '^':
                               printf ("$\\wedge$");
                               break;
                       case '~':
                               printf ("$\\sim$");
                               break;
                       default:
                               printf ("%c", *input);
                               break;
               }
               input++;
       }
}


void
indent (blanks)
char   *blanks;
{
       int     i;

       i = 0;
       while (*blanks) {
               if (*blanks == ' ') {
                       i++;
               } else {               /* *blanks == '\t' */
                       while (++i % tabtotab);
               }
               blanks++;
       }
       printf ("\\hspace*{%d\\indentation}", i);
}


#include "getopt.h"
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <time.h>

extern char *version_string;

static struct option opts[] =
{
       {"complete-file", 0, 0, 'c'},
       {"font-size", 1, 0, 's'},
       {"indentation", 1, 0, 'i'},
       {"header", 0, 0, 'h'},
       {"piped", 0, 0, 'p'},
       {"alignment", 0, 0, 'n'},      /* turn off comment alignment  -joke */
       {"output", 1, 0, 'o'},
       {"tabstop", 1, 0, 'T'},
       {"comment-font", 1, 0, 'C'},
       {"function-font", 1, 0, 'F'},
       {"string-font", 1, 0, 'S'},
       {"keyword-font", 1, 0, 'K'},
       {"header-font", 1, 0, 'H'},
       {"pascal", 0, 0, 'P'},
       {"version", 0, 0, 'V'},
       {0, 0, 0, 0}
};



main (argc, argv)
int     argc;
char  **argv;
{
       int     c;
       int     index;
       int     i;
       int     has_filename;
       char   *input_name;
       char   *output_name;
       char   *program_name;
       long    now;
       char   *today;
       char   *malloc ();

       input_name = "Standard Input";
       output_name = 0;

       now = time (0);
       today = ctime (&now);

       program_name = strrchr (argv[0], '/');
       if (program_name == NULL) {    /* no pathname */
               program_name = argv[0];
       } else {
               program_name++;
       }

       /* simple heuristic: 'm' in name means Modula-2 */
       modula_mode = (strchr (program_name, 'm') != 0);

       if (argc == 1)
               usage (program_name);  /* added exit with usage  -joke */

       while ((c = getopt_long (argc, argv,
                                "cpno:s:i:b:hT:C:F:H:S:K:P:V", opts, &index))
                       != EOF) {
               if (c == 0) {          /* Long option */
                       c = opts[index].val;
               }
               switch (c) {
                       case 'c':
                               complete_file = 1;
                               break;
                       case 'o':
                               if (piped) {
                                       fprintf (stderr,
                                                "%s: Can't use {-p,+pipe} and {-o,+output} together\n",
                                                program_name);
                                       exit (5);
                               }
                               output_name = optarg;
                               break;
                       case 'n':
                               aligntoright = 0;
                               break;
                       case 's':
                               font_size = optarg;
                               break;
                       case 'i':
                               indentation = optarg;
                               break;
                       case 'T':
                               tabtotab = atoi (optarg);
                               break;
                       case 'p':
                               if (output_name != 0) {
                                       fprintf (stderr,
                                                "%s: Can't use {-p,+pipe} and {-o,+output} together\n",
                                                program_name);
                                       exit (5);
                               }
                               piped = 1;
                               break;
                       case 'h':
                               header = 1;
                               complete_file = 1;      /* header implies
                                                        * complete-file */
                               break;
                       case 'C':
                               comment_font = optarg;
                               break;
                       case 'F':
                               stdfun_font = optarg;
                               break;
                       case 'H':
                               header_font = optarg;
                               break;
                       case 'S':
                               string_font = optarg;
                               break;
                       case 'K':
                               keyword_font = optarg;
                               break;
                       case 'P':
                               modula_mode = 0;
                               break;
                       case 'V':
                               fprintf (stderr, "%s\n", version_string);
                               break;
                       default:
                               usage (program_name);
               }
       }
       has_filename = (argc - optind == 1);
       if (has_filename) {            /* last argument is input file name */
               input_name = argv[optind];
               if (freopen (input_name, "r", stdin) == NULL) {
                       fprintf (stderr, "%s: Can't open `%s' for reading\n",
                                program_name, input_name);
                       exit (2);
               }
       }
       if ((output_name == 0) && !piped) {
               char   *tmp;
               if (has_filename) {
                       tmp = strrchr (input_name, '/');
                       if (tmp == 0) { /* plain filename */
                               tmp = input_name;
                       } else {
                               tmp++;
                       }
               } else {
                       tmp = program_name;
               }
               output_name = malloc (strlen (tmp) + 4);
               if (output_name == 0) {
                       fprintf (stderr, "%s: Virtual memory exhausted\n", program_name);
                       exit (3);
               }
               strcpy (output_name, tmp);
               strcat (output_name, ".tex");
       }
       if (!piped) {
               if (freopen (output_name, "w", stdout) == NULL) {
                       fprintf (stderr, "%s: Can't open `%s' for writing\n",
                                program_name, output_name);
                       exit (3);
               }
       }
       printf ("\
%%\n\
%% This file was automatically produced at %.24s by\n\
%% %s", today, program_name);
       for (i = 1; i < argc; i++) {
               printf (" %s", argv[i]);
       }
       if (!has_filename) {
               printf (" (from Standard Input)");
       }
       printf ("\n%%\n");
       if (complete_file) {
               if (header) {
                       if (strcmp (font_size, "10") == 0) {
                               printf ("\\documentstyle[fancyheadings]{article}\n");
                       } else {
                               printf ("\\documentstyle[%spt,fancyheadings]{article}\n",
                                       font_size);
                       }
               } else {
                       if (strcmp (font_size, "10") == 0) {
                               printf ("\\documentstyle{article}\n");
                       } else {
                               printf ("\\documentstyle[%spt]{article}\n", font_size);
                       }
               }
               printf ("\\setlength{\\textwidth}{16cm}\n");
               printf ("\\setlength{\\textheight}{23cm}\n");
               printf ("\\setlength{\\hoffset}{-2cm}\n");
               printf ("\\setlength{\\voffset}{-2cm}\n");
               if (header) {
                       printf ("\\lhead{\\%s ", header_font);
                       substitute (input_name);
                       printf ("}");
                       printf ("\\rhead{\\rm\\thepage}\n");
                       printf ("\\cfoot{}\n");
                       printf ("\\addtolength{\\headheight}{14pt}\n");
                       printf ("\\pagestyle{fancy}\n");
               }
               printf ("\\begin{document}\n");
       }
       printf ("\\expandafter\\ifx\\csname indentation\\endcsname\\relax%\n");
       printf ("\\newlength{\\indentation}\\fi\n");
       printf ("\\setlength{\\indentation}{%s}\n", indentation);
       printf ("\\begin{flushleft}\n");
       yylex ();
       printf ("\\end{flushleft}\n");
       if (complete_file) {
               printf ("\\end{document}\n");
       }
       exit (0);
}


void
usage (name)
char   *name;
{
       fprintf (stderr, "%s\n", version_string);
       fprintf (stderr, "\
Usage: %s [options] file\n\n\
Options:\n\
       [-c]                    [-h]\n\
       [-i length]             [-n]\n\
       [-o file]               [-p]\n\
       [-s fontsize]           [-C font]\n\
       [-F font                [-H font]\n\
       [-K font]               [-P]\n\
       [-S font]               [-T tabulatorwidth]\n\
       [-V]\n\
       \n\
       [+complete-file]        [+header]\n\
       [+indentation length]   [+no-alignment]\n\
       [+output file]          [+pipe]\n\
       [+font-size size]       [+comment-font font]\n\
       {+function-font font    [+keyword-font font]\n\
       [+pascal]               [+header-font font]\n\
       [+string-font font]     [+tabstop width]\n\
       [+version]\n", name);
       exit (1);
}

