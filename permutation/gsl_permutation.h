#ifndef __GSL_PERMUTATION_H__
#define __GSL_PERMUTATION_H__

#include <stdlib.h>
#include <gsl/gsl_errno.h>

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

struct gsl_permutation_struct
{
  size_t size;
  size_t *data;
};

typedef struct gsl_permutation_struct gsl_permutation;

gsl_permutation *gsl_permutation_alloc (size_t n);
gsl_permutation *gsl_permutation_calloc (size_t n);
void gsl_permutation_init (gsl_permutation * p);
void gsl_permutation_free (gsl_permutation * p);

int gsl_permutation_fread (FILE * stream, gsl_permutation * p);
int gsl_permutation_fwrite (FILE * stream, const gsl_permutation * p);
int gsl_permutation_fscanf (FILE * stream, gsl_permutation * p);
int gsl_permutation_fprintf (FILE * stream, const gsl_permutation * p, const char *format);

size_t gsl_permutation_size (const gsl_permutation * p);
size_t * gsl_permutation_data (const gsl_permutation * p);

size_t gsl_permutation_get (const gsl_permutation * p, const size_t i);
int gsl_permutation_swap (gsl_permutation * p, const size_t i, const size_t j);

int gsl_permutation_valid (gsl_permutation * p);
void gsl_permutation_reverse (gsl_permutation * p);
int gsl_permutation_invert (gsl_permutation * inv, const gsl_permutation * p);

extern int gsl_check_range;

#ifdef HAVE_INLINE

extern inline
size_t
gsl_permutation_get (const gsl_permutation * p, const size_t i)
{
#ifndef GSL_RANGE_CHECK_OFF
  if (i >= p->size)
    {
      GSL_ERROR_RETURN ("index out of range", GSL_EINVAL, 0);
    }
#endif
  return p->data[i];
}

#endif /* HAVE_INLINE */

__END_DECLS

#endif /* __GSL_PERMUTATION_H__ */