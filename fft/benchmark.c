#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#include <time.h>

#include <gsl_complex.h>
#include <gsl_fft_complex.h>

#include <gsl_errno.h>

void my_error_handler (const char *reason, const char *file, int line);

int
main (int argc, char *argv[])
{
  complex *data, *fft_data;
  gsl_fft_complex_wavetable complex_wavetable;
  int i, status;
  clock_t start, end;
  int resolution = CLOCKS_PER_SEC;
  unsigned int n = 1;

  gsl_error_set_handler (&my_error_handler);

  if (argc == 2)
    {
      n = strtol (argv[1], NULL, 0);
    }
  else
    {
      printf ("test n\n");
      exit (0);
    }

  status = gsl_fft_complex_wavetable_alloc (n, &complex_wavetable);
  status = gsl_fft_complex_init (n, &complex_wavetable);
  status = gsl_fft_complex_generate_wavetable (n, &complex_wavetable);

  data = malloc (n * sizeof (complex));
  fft_data = malloc (n * sizeof (complex));

  for (i = 0; i < n; i++)
    {
      data[i].real = ((double) rand ()) / RAND_MAX;
      data[i].imag = ((double) rand ()) / RAND_MAX;
    }

  /* compute the fft with radix2 */
  memcpy (fft_data, data, n * sizeof (complex));

  start = clock ();
  i = 0;
  do
    {
      status = gsl_fft_complex_radix2_forward (fft_data, n);
      i++;
      end = clock ();
    }
  while (end < start + resolution && status == 0);

  if (status == 0)
    {
      printf ("gsl_fft_complex_radix2_forward %d %f seconds\n", n, (end - start) / ((double) i) / ((double) CLOCKS_PER_SEC));
    }
  else
    {
      printf ("fft_radix2: not a power of 2\n");
    }

  /* compute the fft */

  memcpy (fft_data, data, n * sizeof (complex));

  start = clock ();
  i = 0;
  do
    {
      status = gsl_fft_complex_forward (fft_data, n, &complex_wavetable);
      i++;
      end = clock ();
    }
  while (end < start + resolution && status == 0);

  if (status == 0)
    {
      printf ("gsl_fft_complex_forward %d %f seconds\n", n, (end - start) / ((double) i) / ((double) CLOCKS_PER_SEC));
    }
  else
    {
      printf ("MR fft failed\n");
    }

  return 0;
}


void
my_error_handler (const char *reason, const char *file, int line)
{
  printf ("error: %s in %s at %d\n", reason, file, line);
}
