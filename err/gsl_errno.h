/* err/gsl_errno.h
 * 
 * Copyright (C) 1996, 1997, 1998, 1999, 2000 Gerard Jungman, Brian Gough
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __GSL_ERRNO_H__
#define __GSL_ERRNO_H__

#include <stdio.h>
#include <errno.h>

#undef __BEGIN_DECLS
#undef __END_DECLS
#ifdef __cplusplus
# define __BEGIN_DECLS extern "C" {
# define __END_DECLS }
#else
# define __BEGIN_DECLS /* empty */
# define __END_DECLS /* empty */
#endif

__BEGIN_DECLS

enum { 
  GSL_SUCCESS  = 0, 
  GSL_FAILURE  = -1,
  GSL_CONTINUE = -2,  /* iteration has not converged */
  GSL_EDOM     = 1,   /* input domain error, e.g sqrt(-1) */
  GSL_ERANGE   = 2,   /* output range error, e.g. exp(1e100) */
  GSL_EFAULT   = 3,   /* invalid pointer */
  GSL_EINVAL   = 4,   /* invalid argument supplied by user */
  GSL_EFAILED  = 5,   /* generic failure */
  GSL_EFACTOR  = 6,   /* factorization failed */
  GSL_ESANITY  = 7,   /* sanity check failed - shouldn't happen */
  GSL_ENOMEM   = 8,   /* malloc failed */
  GSL_EBADFUNC = 9,   /* problem with user-supplied function */
  GSL_ERUNAWAY = 10,  /* iterative process is out of control */
  GSL_EMAXITER = 11,  /* exceeded max number of iterations */
  GSL_EZERODIV = 12,  /* tried to divide by zero */
  GSL_EBADTOL  = 13,  /* user specified an invalid tolerance */
  GSL_ETOL     = 14,  /* failed to reach the specified tolerance */
  GSL_EUNDRFLW = 15,  /* underflow */
  GSL_EOVRFLW  = 16,  /* overflow  */
  GSL_ELOSS    = 17,  /* loss of accuracy */
  GSL_EROUND   = 18,  /* failed because of roundoff error */
  GSL_EBADLEN  = 19,  /* matrix, vector lengths are not conformant */
  GSL_ENOTSQR  = 20,  /* matrix not square */
  GSL_ESING    = 21,  /* apparent singularity detected */
  GSL_EDIVERGE = 22,  /* integral or series is divergent */
  GSL_EUNSUP   = 23,  /* requested feature is not supported by the hardware */
  GSL_EUNIMPL  = 24,  /* requested feature not (yet) implemented */
  GSL_ECACHE   = 25,  /* cache table limit exceeded */
  GSL_ENOPROG  = 26,  /* iteration is not making progress towards solution */
  GSL_ENOPROGJ = 27   /* jacobian evaluations are not improving the solution */
} ;

void gsl_error (const char * reason, const char * file, int line,
		int gsl_errno);

void gsl_warning (const char * reason, const char * file, int line,
		  int gsl_errno) ;

void gsl_stream_printf (const char *label, const char *file,
			int line, const char *reason);

const char * gsl_strerror (const int gsl_errno);

typedef void gsl_error_handler_t (const char * reason, const char * file,
				  int line, int gsl_errno);

typedef void gsl_stream_handler_t (const char * label, const char * file,
				   int line, const char * reason);

gsl_error_handler_t * 
gsl_set_error_handler (gsl_error_handler_t * new_handler);

gsl_stream_handler_t * 
gsl_set_stream_handler (gsl_stream_handler_t * new_handler);

FILE * gsl_set_stream (FILE * new_stream);

/* GSL_ERROR: call the error handler, and return the error code */

#define GSL_ERROR(reason, gsl_errno) \
       do { \
       gsl_error (reason, __FILE__, __LINE__, gsl_errno) ; \
       return gsl_errno ; \
       } while (0)

/* GSL_ERROR_VAL: call the error handler, and return the given value */

#define GSL_ERROR_VAL(reason, gsl_errno, value) \
       do { \
       gsl_error (reason, __FILE__, __LINE__, gsl_errno) ; \
       return value ; \
       } while (0)

/* GSL_ERROR_VOID: call the error handler, and then return
   (for void functions which still need to generate an error) */

#define GSL_ERROR_VOID(reason, gsl_errno) \
       do { \
       gsl_error (reason, __FILE__, __LINE__, gsl_errno) ; \
       return ; \
       } while (0)

/* GSL library code can occasionally generate warnings, which are not
   intended to be fatal. You can compile a version of the library with
   warnings turned off globally by defining the preprocessor constant
   GSL_WARNINGS_OFF. This turns off the warnings, but does not disable
   error handling in any way or turn off error messages.
 
   GSL_WARNING() is not intended for use in client code -- use
   GSL_MESSAGE() instead.  */
    
#ifdef GSL_WARNINGS_OFF   /* throw away warnings */
#define GSL_WARNING(warning, gsl_errno) \
       do { } while(0)
#else                     /* output all warnings */
#define GSL_WARNING(warning, gsl_errno) \
       do { \
       gsl_warning (warning, __FILE__, __LINE__, gsl_errno) ; \
       } while (0)
#endif

/* Warnings can also be turned off at runtime by setting the variable
   gsl_warnings_off to a non-zero value */

extern int gsl_warnings_off ;


/* Sometimes you have several status results returned from
 * function calls and you want to combine them in some sensible
 * way. You cannot produce a "total" status condition, but you can
 * pick one from a set of conditions based on an implied hierarchy.
 *
 * In other words:
 *    you have: status_a, status_b, ...
 *    you want: status = (status_a if it is bad, or status_b if it is bad,...)
 *
 * In this example you consider status_a to be more important and
 * it is checked first, followed by the others in the order specified.
 *
 * Here are some dumb macros to do this.
 */
#define GSL_ERROR_SELECT_2(a,b)       ((a) != GSL_SUCCESS ? (a) : ((b) != GSL_SUCCESS ? (b) : GSL_SUCCESS))
#define GSL_ERROR_SELECT_3(a,b,c)     ((a) != GSL_SUCCESS ? (a) : GSL_ERROR_SELECT_2(b,c))
#define GSL_ERROR_SELECT_4(a,b,c,d)   ((a) != GSL_SUCCESS ? (a) : GSL_ERROR_SELECT_3(b,c,d))
#define GSL_ERROR_SELECT_5(a,b,c,d,e) ((a) != GSL_SUCCESS ? (a) : GSL_ERROR_SELECT_4(b,c,d,e))


__END_DECLS

#endif /* __GSL_ERRNO_H__ */
