/* bracketing.c -- find an initial bracketing interval for a function to minimize */

#include <gsl/gsl_math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_min.h>
#include <gsl/gsl_machine.h>

#include "min.h"

int 
gsl_min_find_bracket(gsl_function *f,double *minimum,double * f_minimum,
		     gsl_interval *x, double * f_lower,double * f_upper,
		     size_t eval_max)
{
  /* The three following variables must be declared volatile to avoid storage
     in extended precision registers available on some architecture. The code
     relies on the ability to compare double values. As the values will be
     store in regular memory, the extended precision will then be lost and
     values that are different in extended precision might have equal
     representation in double precision. This behavior might break the
     algorithm. 
   */
  volatile double f_left = *f_lower;
  volatile double f_right = *f_upper;
  volatile double f_center;
  double x_left = x->lower;
  double x_right= x->upper; 
  double x_center;
  const double golden = 0.318966;	/* golden = (3 - sqrt(5))/2 */
  size_t nb_eval = 0;
  
  
  if (f_right >= f_left) 
    {
      x_center = (x_right - x_left) * golden + x_left;
      nb_eval++;
      SAFE_FUNC_CALL (f, x_center, &f_center);
    }
  else
    {
      x_center = x_right ;
      f_center = f_right ;
      x_right = (x_center - x_left) / golden + x_left;
      nb_eval++;
      SAFE_FUNC_CALL (f, x_right, &f_right);
    }
  
  do
    {
      if (f_center < f_left )
	{
	  if (f_center < f_right)
	    {
	      x->lower = x_left;
	      x->upper = x_right;
	      *minimum = x_center;
	      *f_lower = f_left;
	      *f_upper = f_right;
	      *f_minimum = f_center;
/*	      gsl_ieee_printf_double (&f_left);
	      printf(" ");
	      gsl_ieee_printf_double (&f_center);
	      printf("\n");*/
	      return GSL_SUCCESS;
	    }
	  else if (f_center > f_right)
	    {
	      x_left = x_center;
	      f_left = f_center;
	      x_center = x_right;
	      f_center = f_right;
	      x_right = (x_center - x_left) / golden + x_left;
	      nb_eval++;
	      SAFE_FUNC_CALL (f, x_right, &f_right);
	    }
	  else /* f_center == f_right */
	    {
	      x_right = x_center;
	      f_right = f_center;
	      x_center = (x_right - x_left) * golden + x_left;
	      nb_eval++;
	      SAFE_FUNC_CALL (f, x_center, &f_center);
	    }
	}
      else /* f_center >= f_left */
	{
	  x_right = x_center;
	  f_right = f_center;
	  x_center = (x_right - x_left) * golden + x_left;
	  nb_eval++;
	  SAFE_FUNC_CALL (f, x_center, &f_center);
	}
    }
  while (nb_eval < eval_max 
	 && (x_right - x_left) > GSL_SQRT_DBL_EPSILON * ( (x_right + x_left) * 0.5 ) + GSL_SQRT_DBL_EPSILON);
  x->lower = x_left;
  x->upper = x_right;
  *minimum = x_center;
  *f_lower = f_left;
  *f_upper = f_right;
  *f_minimum = f_center;
  return GSL_FAILURE;
}
