/*
$Id$
Copyright 2005, 2006 Eric L. Smith <eric@brouhaha.com>

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

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib.h>

#include "util.h"
#include "platform.h"
#include "arch.h"
#include "model.h"
#include "display.h"  // proc.h needs segment_bitmap_t
#include "keyboard.h"
#include "proc.h"
#include "calcdef.h"
//#include "proc_int.h"

#include "sound.h"  // ugh! needed for stub sound functions


#ifdef DEFAULT_PATH
char *default_path = MAKESTR(DEFAULT_PATH);
#else
char *default_path = NULL;
#endif


void usage (FILE *f)
{
  fprintf (f, "udis - %s\n", nonpareil_release);
  fprintf (f, "Copyright 2004, 2005, 2006 Eric L. Smith\n");
  fprintf (f, "http://nonpareil.brouhaha.com/\n");
  fprintf (f, "\n");
  fprintf (f, "usage: %s [options] model\n", progname);
  fprintf (f, "options:\n");
  fprintf (f, "   -a  assembly source mode\n");
  fprintf (f, "   -l  listing mode (default)\n");
}


bool asm_mode;
bool pass_two;


typedef struct
{
  bool has_target;
  bool ends_flow;
} flow_type_info_t;

static flow_type_info_t flow_type_info [MAX_FLOW_TYPE] =
{
  [flow_no_branch]              = { false, false},
  [flow_cond_branch]            = { true,  false},
  [flow_uncond_branch]          = { true,  true},
  [flow_uncond_branch_keycode]  = { false, true},
  [flow_uncond_branch_computed] = { false, true},
  [flow_subroutine_call]        = { true,  false},
  [flow_subroutine_return]      = { false, true},
  [flow_bank_switch]            = { true,  true}
};


static void disassemble (sim_t *sim)
{
  uint8_t page, max_page;
  int page_size;
  bank_t bank, max_bank, target_bank;
  addr_t addr, max_addr, target_addr;
  addr_t delayed_select_mask, delayed_select_addr;
  flow_type_t flow_type;
  bool carry_known_clear;
  char buf [100];

  max_bank = sim_get_max_rom_bank (sim);
  page_size = sim_get_rom_page_size (sim);
  max_addr = sim_get_max_rom_addr (sim);
  max_page = max_addr / page_size;

  for (bank = 0; bank < max_bank; bank++)
    for (page = 0; page < max_page; page++)
      if (sim_page_exists (sim, bank, page))
	{
	  addr = page * page_size;
	  while (addr < ((page + 1) * page_size))
	    {
	      if (! sim_disassemble (sim,
				     & bank,
				     & addr,
				     & carry_known_clear,
				     & delayed_select_mask,
				     & delayed_select_addr,
				     & flow_type,
				     & target_bank,
				     & target_addr,
				     buf,
				     sizeof (buf)))
		{
		  warning ("disassembler error at bank %d addr %05o\n", bank, (addr + 1) % max_addr);
		}
	      if (! pass_two)
		{
		  if (flow_type_info [flow_type].has_target)
		    {
		      // $$$ create label
		    }
		}
	      else
		{
		  // $$$ print label (if applicable)
		  if (flow_type_info [flow_type].has_target)
		    {
		      char label [10];
		      sprintf (label, "L%04o", target_addr);
		      printf (buf, label);
		    }
		  else
		    printf (buf);
		  printf ("\n");
		  if (flow_type_info [flow_type].ends_flow)
		    printf ("\n");
		}
	    }
	}
}


int main (int argc, char *argv[])
{
  char *model_str = NULL;
  char *ncd_fn;
  sim_t *sim;

  progname = argv [0];

  g_thread_init (NULL);

  while (--argc)
    {
      argv++;
      if (*argv [0] == '-')
	{
	  if (strcmp (argv [0], "-a") == 0)
	    asm_mode = true;
	  else if (strcmp (argv [0], "-l") == 0)
	    asm_mode = false;
	  else
	    fatal (1, "unrecognized option '%s'\n", argv [0]);
	}
      else if (model_str)
	{
	  fatal (1, "only one model may be specified\n");
	}
      else
	model_str = argv [0];
    }

  ncd_fn = find_file_with_suffix (model_str, ".ncd", default_path);
  if (! ncd_fn)
    fatal (2, "can't find .ncd file\n");

  sim = sim_init (ncd_fn,
		  NULL,  // install_hardware_callback
		  NULL,  // install_hardware_callback_ref
		  NULL,  // display_update_callback
		  NULL); // display_udpate_callback_ref 

  asm_mode = false;
  pass_two = false;
  disassemble (sim);
  pass_two = true;
  disassemble (sim);

  exit (0);
}


// sound function stubs
bool stop_sound (int id UNUSED)
{
  return true;
}

int synth_sound (float    frequency UNUSED,
		 float    amplitude UNUSED,
		 float    duration UNUSED,
		 sample_t *waveform_table UNUSED,
		 uint32_t waveform_table_length UNUSED)
{
  return 0;
}

sample_t squarewave_waveform_table [1] = { 0 };
uint32_t squarewave_waveform_table_length = 1;
