/* Author:  G. Jungman
 * RCS:     $Id$
 */
#ifndef BESSEL_OLVER_H_
#define BESSEL_OLVER_H_

#include "gsl_sf_result.h"

int gsl_sf_bessel_Jnu_asymp_Olver_impl(double nu, double x, gsl_sf_result * result);
int gsl_sf_bessel_Ynu_asymp_Olver_impl(double nu, double x, gsl_sf_result * result);

double gsl_sf_bessel_Olver_zofmzeta(double minus_zeta);


#endif  /* !BESSEL_OLVER_H_ */
