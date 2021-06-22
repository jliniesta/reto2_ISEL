/* From: http://www.cs.uleth.ca/~holzmann/C/system/ttyraw.c
   Set terminal (tty) into "raw" mode: no line or other processing done
   Terminal handling documentation:
       curses(3X)  - screen handling library.
       tput(1)     - shell based terminal handling.
       terminfo(4) - SYS V terminal database.
       termcap     - BSD terminal database. Obsoleted by above.
       termio(7I)  - terminal interface (ioctl(2) - I/O control).
       termios(3)  - preferred terminal interface (tc* - terminal control).
*/

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "ttyraw.h"

static struct termios orig_termios;  /* TERMinal I/O Structure */
static int ttyfd = STDIN_FILENO;     /* STDIN_FILENO is 0 by default */

/* reset tty - useful also for restoring the terminal when this process
   wishes to temporarily relinquish the tty
*/
int
tty_reset (void)
{
  /* flush and reset */
  if (tcsetattr (ttyfd, TCSAFLUSH, &orig_termios) < 0)
    return -1;
  write (ttyfd, "\e8\e[?25h", 8); // show cursor, restore cursor position
  return 0;
}

static void tty_atexit (void) { tty_reset(); }
static void tty_reset_handler (int sig) { tty_reset(); exit (0); }

/* put terminal in raw mode - see termio(7I) for modes */
int
tty_raw (void)
{
  /* check that input is from a tty */
  if (! isatty (ttyfd))
    return -1; /* not on a tty */

  /* store current tty settings in orig_termios */
  if (tcgetattr (ttyfd, &orig_termios) < 0)
    return -2; /* can't get tty settings */

  /* register the tty reset with the exit handler */
  if (atexit (tty_atexit) != 0)
    return -3; /* atexit: can't register tty reset */

  signal (SIGINT,  tty_reset_handler);
  signal (SIGTSTP, tty_reset_handler);
  signal (SIGQUIT, tty_reset_handler);
  write (ttyfd, "\e7\e[?25l", 8); // save cursor, hide cursor
  
  struct termios raw = orig_termios;  /* copy original and then modify below */

  /* input modes - clear indicated ones giving: no break, 
     no parity check, no strip char, no start/stop output (sic) control */
  raw.c_iflag &= ~(BRKINT | INPCK | ISTRIP | IXON);

  /* output modes - clear giving: no post processing such as NL to CR+NL */
  raw.c_oflag &= ~(OPOST);

  /* control modes - set 8 bit chars */
  raw.c_cflag |= (CS8);

  /* local modes - clear giving: echoing off, canonical off (no erase with 
     backspace, ^U,...),  no extended functions, no signal chars (^Z,^C) */
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

  /* control chars - set return condition: min number of bytes and timer */
  raw.c_cc[VMIN] = 0; raw.c_cc[VTIME] = 0; /* immediate - anything       */

  /* put terminal in raw mode after flushing */
  if (tcsetattr (ttyfd, TCSAFLUSH, &raw) < 0)
    return -4; /* can't set raw mode */
  return 0;
}
