#ifndef __GSL_SORT_USHORT_H__
#define __GSL_SORT_USHORT_H__

#include <stdlib.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_permutation.h>

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

void gsl_sort_ushort (unsigned short * data, size_t stride, size_t n);
int gsl_sort_ushort_index (size_t * p, const unsigned short * data, size_t stride, size_t n);

__END_DECLS

#endif /* __GSL_SORT_USHORT_H__ */