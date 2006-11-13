/*
$Id$
Copyright 1995, 2004, 2005, 2006 Eric L. Smith <eric@brouhaha.com>

Nonpareil is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.  Note that I am not
granting permission to redistribute or modify Nonpareil under the
terms of any later version of the General Public License.

Nonpareil is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program (in the file "COPYING"); if not, write to the
Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA 02111, USA.
*/


extern int arch;

int pass;
extern int lineno;
extern int errors;

extern int group;	/* current rom group */
extern int rom;		/* current rom */
extern int pc;		/* current pc */

extern int dsr;		/* delayed select rom */
extern int dsg;		/* delayed select group */

extern char flag_char;  /* used to mark jumps across rom banks */

extern int symtab_flag;

extern int legal_flag;	// used to suppress warnings for unconditional
			// branches after arithmetic instructions


#define OTHER_INST 0
#define ARITH_INST 1
#define TEST_INST 2
extern int last_instruction_type;


#define MAX_LINE 256
extern char linebuf [MAX_LINE];
extern char *lineptr;


#define MAXGROUP 2
#define MAXROM 16     /* max 8 for classic, 16 for woodstock */

extern symtab_t *global_symtab;
extern symtab_t *symtab [MAXGROUP] [MAXROM];  /* separate symbol tables for each ROM */

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
int error       (char *format, ...);
int asm_warning (char *format, ...);


/* lexers: */

int asm_lex  (void);  /* generic, used only to parse .arch directive */
int casm_lex (void);  /* classic */
int wasm_lex (void);  /* woodstock */


/* parsers: */
typedef int (parser_t)(void);

extern parser_t *parser [ARCH_MAX];


int asm_parse  (void);  /* generic, used only to parse .arch directive */
int casm_parse (void);  /* classic */
int wasm_parse (void);  /* woodstock */


typedef struct keyword
{
  char *name;
  int value;
} keyword_t;

int keyword (char *string, keyword_t *table);


