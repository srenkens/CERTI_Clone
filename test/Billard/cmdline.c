/*
  File autogenerated by gengetopt version 2.10
  generated with the following command:
  gengetopt 

  The developers of gengetopt consider the fixed text that goes in all
  gengetopt output files to be in the public domain:
  we make no copyright claims on it.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "getopt.h"

#include "cmdline.h"

void
cmdline_parser_print_version (void)
{
  printf ("%s %s\n", CMDLINE_PARSER_PACKAGE, CMDLINE_PARSER_VERSION);
}

void
cmdline_parser_print_help (void)
{
  cmdline_parser_print_version ();
  printf("\n"
  "Purpose:\n"
  "  CERTI test application\n"
  "\n"
  "Usage: %s [OPTIONS]...\n", CMDLINE_PARSER_PACKAGE);
  printf("   -h         --help               Print help and exit\n");
  printf("   -V         --version            Print version and exit\n");
  printf("   -aINT      --auto=INT           auto start\n");
  printf("   -c         --coordinated        coordinated time (default=on)\n");
  printf("   -dINT      --delay=INT          delay before 1st step\n");
  printf("   -fSTRING   --federation=STRING  federation name\n");
  printf("   -lSTRING   --logfile=STRING     file to log events\n");
  printf("   -nSTRING   --name=STRING        federate name\n");
  printf("   -oSTRING   --demo=STRING        select demo\n");
  printf("   -tINT      --timer=INT          timer\n");
  printf("   -v         --verbose            verbose mode (default=off)\n");
  printf("   -xINT      --xoffset=INT        X offset (X11)\n");
  printf("   -yINT      --yoffset=INT        Y offset (X11)\n");
  printf("   -XINT      --initx=INT          ball initial X value\n");
  printf("   -YINT      --inity=INT          ball initial Y value\n");
}


static char *gengetopt_strdup (const char *s);

/* gengetopt_strdup() */
/* strdup.c replacement of strdup, which is not standard */
char *
gengetopt_strdup (const char *s)
{
  char *result = (char*)malloc(strlen(s) + 1);
  if (result == (char*)0)
    return (char*)0;
  strcpy(result, s);
  return result;
}

int
cmdline_parser (int argc, char * const *argv, struct gengetopt_args_info *args_info)
{
  int c;	/* Character of the parsed option.  */
  int missing_required_options = 0;

  args_info->help_given = 0 ;
  args_info->version_given = 0 ;
  args_info->auto_given = 0 ;
  args_info->coordinated_given = 0 ;
  args_info->delay_given = 0 ;
  args_info->federation_given = 0 ;
  args_info->logfile_given = 0 ;
  args_info->name_given = 0 ;
  args_info->demo_given = 0 ;
  args_info->timer_given = 0 ;
  args_info->verbose_given = 0 ;
  args_info->xoffset_given = 0 ;
  args_info->yoffset_given = 0 ;
  args_info->initx_given = 0 ;
  args_info->inity_given = 0 ;
#define clear_args() { \
  args_info->coordinated_flag = 1;\
  args_info->federation_arg = NULL; \
  args_info->logfile_arg = NULL; \
  args_info->name_arg = NULL; \
  args_info->demo_arg = NULL; \
  args_info->verbose_flag = 0;\
}

  clear_args();

  optarg = 0;
  optind = 1;
  opterr = 1;
  optopt = '?';

  while (1)
    {
      int option_index = 0;
      char *stop_char;

      static struct option long_options[] = {
        { "help",	0, NULL, 'h' },
        { "version",	0, NULL, 'V' },
        { "auto",	1, NULL, 'a' },
        { "coordinated",	0, NULL, 'c' },
        { "delay",	1, NULL, 'd' },
        { "federation",	1, NULL, 'f' },
        { "logfile",	1, NULL, 'l' },
        { "name",	1, NULL, 'n' },
        { "demo",	1, NULL, 'o' },
        { "timer",	1, NULL, 't' },
        { "verbose",	0, NULL, 'v' },
        { "xoffset",	1, NULL, 'x' },
        { "yoffset",	1, NULL, 'y' },
        { "initx",	1, NULL, 'X' },
        { "inity",	1, NULL, 'Y' },
        { NULL,	0, NULL, 0 }
      };

      stop_char = 0;
      c = getopt_long (argc, argv, "hVa:cd:f:l:n:o:t:vx:y:X:Y:", long_options, &option_index);

      if (c == -1) break;	/* Exit from `while (1)' loop.  */

      switch (c)
        {
        case 'h':	/* Print help and exit.  */
          clear_args ();
          cmdline_parser_print_help ();
          exit (EXIT_SUCCESS);

        case 'V':	/* Print version and exit.  */
          clear_args ();
          cmdline_parser_print_version ();
          exit (EXIT_SUCCESS);

        case 'a':	/* auto start.  */
          if (args_info->auto_given)
            {
              fprintf (stderr, "%s: `--auto' (`-a') option given more than once\n", CMDLINE_PARSER_PACKAGE);
              clear_args ();
              exit (EXIT_FAILURE);
            }
          args_info->auto_given = 1;
          args_info->auto_arg = strtol (optarg,&stop_char,0);
          break;

        case 'c':	/* coordinated time.  */
          if (args_info->coordinated_given)
            {
              fprintf (stderr, "%s: `--coordinated' (`-c') option given more than once\n", CMDLINE_PARSER_PACKAGE);
              clear_args ();
              exit (EXIT_FAILURE);
            }
          args_info->coordinated_given = 1;
          args_info->coordinated_flag = !(args_info->coordinated_flag);
          break;

        case 'd':	/* delay before 1st step.  */
          if (args_info->delay_given)
            {
              fprintf (stderr, "%s: `--delay' (`-d') option given more than once\n", CMDLINE_PARSER_PACKAGE);
              clear_args ();
              exit (EXIT_FAILURE);
            }
          args_info->delay_given = 1;
          args_info->delay_arg = strtol (optarg,&stop_char,0);
          break;

        case 'f':	/* federation name.  */
          if (args_info->federation_given)
            {
              fprintf (stderr, "%s: `--federation' (`-f') option given more than once\n", CMDLINE_PARSER_PACKAGE);
              clear_args ();
              exit (EXIT_FAILURE);
            }
          args_info->federation_given = 1;
          args_info->federation_arg = gengetopt_strdup (optarg);
          break;

        case 'l':	/* file to log events.  */
          if (args_info->logfile_given)
            {
              fprintf (stderr, "%s: `--logfile' (`-l') option given more than once\n", CMDLINE_PARSER_PACKAGE);
              clear_args ();
              exit (EXIT_FAILURE);
            }
          args_info->logfile_given = 1;
          args_info->logfile_arg = gengetopt_strdup (optarg);
          break;

        case 'n':	/* federate name.  */
          if (args_info->name_given)
            {
              fprintf (stderr, "%s: `--name' (`-n') option given more than once\n", CMDLINE_PARSER_PACKAGE);
              clear_args ();
              exit (EXIT_FAILURE);
            }
          args_info->name_given = 1;
          args_info->name_arg = gengetopt_strdup (optarg);
          break;

        case 'o':	/* select demo.  */
          if (args_info->demo_given)
            {
              fprintf (stderr, "%s: `--demo' (`-o') option given more than once\n", CMDLINE_PARSER_PACKAGE);
              clear_args ();
              exit (EXIT_FAILURE);
            }
          args_info->demo_given = 1;
          args_info->demo_arg = gengetopt_strdup (optarg);
          break;

        case 't':	/* timer.  */
          if (args_info->timer_given)
            {
              fprintf (stderr, "%s: `--timer' (`-t') option given more than once\n", CMDLINE_PARSER_PACKAGE);
              clear_args ();
              exit (EXIT_FAILURE);
            }
          args_info->timer_given = 1;
          args_info->timer_arg = strtol (optarg,&stop_char,0);
          break;

        case 'v':	/* verbose mode.  */
          if (args_info->verbose_given)
            {
              fprintf (stderr, "%s: `--verbose' (`-v') option given more than once\n", CMDLINE_PARSER_PACKAGE);
              clear_args ();
              exit (EXIT_FAILURE);
            }
          args_info->verbose_given = 1;
          args_info->verbose_flag = !(args_info->verbose_flag);
          break;

        case 'x':	/* X offset (X11).  */
          if (args_info->xoffset_given)
            {
              fprintf (stderr, "%s: `--xoffset' (`-x') option given more than once\n", CMDLINE_PARSER_PACKAGE);
              clear_args ();
              exit (EXIT_FAILURE);
            }
          args_info->xoffset_given = 1;
          args_info->xoffset_arg = strtol (optarg,&stop_char,0);
          break;

        case 'y':	/* Y offset (X11).  */
          if (args_info->yoffset_given)
            {
              fprintf (stderr, "%s: `--yoffset' (`-y') option given more than once\n", CMDLINE_PARSER_PACKAGE);
              clear_args ();
              exit (EXIT_FAILURE);
            }
          args_info->yoffset_given = 1;
          args_info->yoffset_arg = strtol (optarg,&stop_char,0);
          break;

        case 'X':	/* ball initial X value.  */
          if (args_info->initx_given)
            {
              fprintf (stderr, "%s: `--initx' (`-X') option given more than once\n", CMDLINE_PARSER_PACKAGE);
              clear_args ();
              exit (EXIT_FAILURE);
            }
          args_info->initx_given = 1;
          args_info->initx_arg = strtol (optarg,&stop_char,0);
          break;

        case 'Y':	/* ball initial Y value.  */
          if (args_info->inity_given)
            {
              fprintf (stderr, "%s: `--inity' (`-Y') option given more than once\n", CMDLINE_PARSER_PACKAGE);
              clear_args ();
              exit (EXIT_FAILURE);
            }
          args_info->inity_given = 1;
          args_info->inity_arg = strtol (optarg,&stop_char,0);
          break;


        case 0:	/* Long option with no short option */

        case '?':	/* Invalid option.  */
          /* `getopt_long' already printed an error message.  */
          exit (EXIT_FAILURE);

        default:	/* bug: option not considered.  */
          fprintf (stderr, "%s: option unknown: %c\n", CMDLINE_PARSER_PACKAGE, c);
          abort ();
        } /* switch */
    } /* while */


  if (! args_info->federation_given)
    {
      fprintf (stderr, "%s: '--federation' ('-f') option required\n", CMDLINE_PARSER_PACKAGE);
      missing_required_options = 1;
    }
  if (! args_info->name_given)
    {
      fprintf (stderr, "%s: '--name' ('-n') option required\n", CMDLINE_PARSER_PACKAGE);
      missing_required_options = 1;
    }
  if ( missing_required_options )
    exit (EXIT_FAILURE);

  return 0;
}
