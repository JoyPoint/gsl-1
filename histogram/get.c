/* histogram/get.c
 * 
 * Copyright (C) 1996, 1997, 1998, 1999, 2000 Brian Gough
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

#include <config.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_histogram.h>

double
gsl_histogram_get (const gsl_histogram * h, size_t i)
{
  const size_t n = h->n;

  if (i >= n)
    {
      GSL_ERROR_VAL ("index lies outside valid range of 0 .. n - 1",
			GSL_EDOM, 0);
    }

  return h->bin[i];
}

int
gsl_histogram_get_range (const gsl_histogram * h, size_t i,
			 double *lower, double *upper)
{
  const size_t n = h->n;

  if (i >= n)
    {
      GSL_ERROR ("index lies outside valid range of 0 .. n - 1", GSL_EDOM);
    }

  *lower = h->range[i];
  *upper = h->range[i + 1];

  return GSL_SUCCESS;
}
