#include <stdlib.h>
#include <stdio.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit_nlin.h>

static double X[20][4] = {
  {24.3485677, 4.71448798, -2.19486633, 2.69405755},
  {22.4116222, 3.93075538, -1.42344852, 2.5233557},
  {17.88886, 2.9290853, .125174936, -3.96823353},
  {17.3237176, 2.99606803, 2.03285653, 2.28992327},
  {17.0906508, 3.02485425, .296995153, .0876226126},
  {16.578006, 3.1036312, -.18617941, .103262914},
  {15.692993, 3.33088442, .0706406887, 1.05923955},
  {14.3232177, 3.85604218, -2.3762839, -3.09486813},
  {14.1279266, 3.97896121, .446109351, 1.40023753},
  {13.6081961, 4.16435075, -1.51250057, -1.52510626},
  {13.4295245, 4.22697223, -.196985195, .532009293},
  {13.0176117, 4.3579261, -.353131208, .301377627},
  {12.2713535, 4.62398535, -.00183585584, .894170703},
  {11.0316144, 5.13967727, -2.38978772, -2.89510064},
  {10.8807981, 5.24558004, .230495952, 1.27315117},
  {10.4029264, 5.41141257, -1.5116632, -1.47615921},
  {10.2574435, 5.46211045, -.299855732, .451893162},
  {9.87863876, 5.57914292, -.368885288, .358086545},
  {9.1894983, 5.82082741, -.230157969, .621476534},
  {8.00589008, 6.27788753, -1.46022815, -1.33468082}
};

static double F[20] = {
  2474.05541,
  1924.69004,
  1280.63194,
  1244.81867,
  1190.53739,
  1159.34935,
  1108.44426,
  1090.11073,
  1015.92942,
  1002.43533,
  971.221084,
  949.589435,
  911.359899,
  906.522994,
  840.525729,
  833.950164,
  807.557511,
  791.00924,
  761.09598,
  726.787783,
};

int
brown_f (const gsl_vector * x, void *params, gsl_vector * f)
{
  double x0 = gsl_vector_get (x, 0);
  double x1 = gsl_vector_get (x, 1);
  double x2 = gsl_vector_get (x, 2);
  double x3 = gsl_vector_get (x, 3);
  size_t i;

  for (i = 0; i < 20; i++)
    {
      double ti = 0.2 * (i + 1);
      double ui = x0 + x1 * ti - exp (ti);
      double vi = x2 + x3 * sin (ti) - cos (ti);

      gsl_vector_set (f, i, ui * ui + vi * vi);
    }

  return GSL_SUCCESS;
}

int
brown_df (const gsl_vector * x, void *params, gsl_matrix * df)
{
  double x0 = gsl_vector_get (x, 0);
  double x1 = gsl_vector_get (x, 1);
  double x2 = gsl_vector_get (x, 2);
  double x3 = gsl_vector_get (x, 3);
  size_t i;

  for (i = 0; i < 20; i++)
    {
      double ti = 0.2 * (i + 1);
      double ui = x0 + x1 * ti - exp (ti);
      double vi = x2 + x3 * sin (ti) - cos (ti);

      gsl_matrix_set (df, i, 0, 2 * ui);
      gsl_matrix_set (df, i, 1, 2 * ui * ti);
      gsl_matrix_set (df, i, 2, 2 * vi);
      gsl_matrix_set (df, i, 3, 2 * vi * sin (ti));

    }
  return GSL_SUCCESS;
}

int
brown_fdf (const gsl_vector * x, void *params,
	   gsl_vector * f, gsl_matrix * df)
{
  brown_f (x, params, f);
  brown_df (x, params, df);

  return GSL_SUCCESS;
}

void
test_lmder (void)
{
  const gsl_multifit_fdfsolver_type *T;
  gsl_multifit_fdfsolver *s;

  int status;
  size_t iter = 0;

  const size_t n = 20;
  const size_t p = 4;

  gsl_multifit_function_fdf f = { &brown_f, &brown_df, &brown_fdf, n, p, 0 };

  double x_init[4] = { 25, 5, -5, -1 };
  gsl_vector x = gsl_vector_view (x_init, p);

  T = gsl_multifit_fdfsolver_lmsder;
  s = gsl_multifit_fdfsolver_alloc (T, &f, &x);

  do
    {
      status = gsl_multifit_fdfsolver_iterate (s);

      gsl_test_rel (gsl_vector_get (s->x, 0), X[iter][0], 1e-5, "lmsder iter=%u x0", iter);
      gsl_test_rel (gsl_vector_get (s->x, 1), X[iter][1], 1e-5, "lmsder iter=%u x1", iter);
      gsl_test_rel (gsl_vector_get (s->x, 2), X[iter][2], 1e-5, "lmsder iter=%u x2", iter);
      gsl_test_rel (gsl_vector_get (s->x, 3), X[iter][3], 1e-5, "lmsder iter=%u x3", iter);
      gsl_test_rel (gsl_blas_dnrm2 (s->f), F[iter], 1e-5, "lmsder iter=%u f", iter);

      iter++;
    }
  while (iter < 20);

  gsl_multifit_fdfsolver_free (s);

}
