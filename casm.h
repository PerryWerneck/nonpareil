/*
casm.h
$Id$
Copyright 1995, 2004 Eric L. Smith <eric@brouhaha.com>

CASM is an assembler for the processor used in the HP "Classic" series
of calculators, which includes the HP-35, HP-45, HP-55, HP-65, HP-70,
and HP-80.

CASM is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License version 2 as published by the Free
Software Foundation.  Note that I am not granting permission to redistribute
or modify CASM under the terms of any later version of the General Public
License.

This program is distributed in the hope that it will be useful (or at least
amusing), but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
Public License for more details.

You should have received a copy of the GNU General Public License along with
this program (in the file "COPYING"); if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

extern int pass;
extern int lineno;
extern int errors;

extern int group;	/* current rom group */
extern int rom;		/* current rom */
extern int pc;		/* current pc */

extern int dsr;		/* delayed select rom */
extern int dsg;		/* delayed select group */

extern char flag_char;  /* used to mark jumps across rom banks */

extern int symtab_flag;


#define OTHER_INST 0
#define ARITH_INST 1
#define TEST_INST 2
extern int last_instruction_type;


#define MAX_LINE 256
extern char linebuf [MAX_LINE];
extern char *lineptr;

#define MAXGROUP 2
#define MAXROM 8
extern t_symtab symtab [MAXGROUP] [MAXROM];  /* separate symbol tables for each ROM */

void do_label (char *s);


void emit       (int op);  /* use for instructions that never set carry */
void emit_arith (int op);  /* use for arithmetic instructions that may set carry */
void emit_test  (int op);  /* use for test instructions */


void target (int g, int r, int p);


/*
 * Check that val is in the range [min, max].  If so, return val.
 * If not, issue an error and return min.
 */
int range (int val, int min, int max);


/*
 * print to both listing error buffer and standard error
 *
 * Use this for general messages.  Don't use this for warnings or errors
 * generated by a particular line of the source file.  Use error() or
 * warning() for that.
 */
int err_printf (char *format, ...);

/* generate error or warning messages and increment appropriate counter */
int error   (char *format, ...);
int warning (char *format, ...);


/* in casml.l: */
int keyword (char *string);
