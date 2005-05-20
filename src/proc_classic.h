/*
$Id$
Copyright 1995, 2003, 2004, 2005 Eric L. Smith <eric@brouhaha.com>

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


#define WSIZE 14

typedef uint8_t digit_t;
typedef digit_t reg_t [WSIZE];


#define SSIZE 12
#define EXT_FLAG_SIZE 12

#define MAX_GROUP 2
#define MAX_ROM 8
#define ROM_SIZE 256

#define MAX_CHIP_COUNT 3  // A&R, C&T CPU
                          // program storage (HP-65 only)
                          // CRC (HP-65 only)


typedef struct
{
  reg_t a;
  reg_t b;
  reg_t c;
  reg_t d;
  reg_t e;
  reg_t f;
  reg_t m;

  digit_t p;

  uint8_t carry, prev_carry;

  bool s [SSIZE];

  uint8_t pc;
  uint8_t rom;
  uint8_t group;

  uint8_t del_rom;
  uint8_t del_grp;

  uint8_t ret_pc;

  int prev_pc;  /* used to store complete five-digit octal address of instruction */

  bool ext_flag [SSIZE];  /* external flags, e.g., slide switches,
			     magnetic card inserted */

  // keyboard
  bool key_flag;      /* true if a key is down */
  int key_buf;        /* most recently pressed key */

  // display
  bool display_enable;

  int left_scan;
  int right_scan;
  int display_scan_position;   /* word index, left_scan down to right_scan */
  int display_digit_position;  /* character index, 0 to MAX_DIGIT_POSITION-1 */

  void (* display_scan_fn) (sim_t *sim);

  void (* op_fcn [1024])(struct sim_t *sim, int opcode);

  // RAM
  int ram_addr;  /* selected RAM address */
  reg_t *ram;
} classic_cpu_reg_t;
