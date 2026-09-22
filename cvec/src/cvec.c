#include "cvec.h"

#include <stdlib.h>

int cvec_init(cvec *v, size_t elem_size, size_t elem_align) {
  if (v == NULL || elem_size == 0) {
    return -1;
  }

  v->data = NULL;
  v->len = 0;
  v->cap = 0;
  v->elem_size = elem_size;
  v->elem_align = elem_align;

  return 0;
}

void cvec_destroy(cvec *v) {
  if (v == NULL) {
    return;
  }

  free(v->data);

  v->data = NULL;
  v->len = 0;
  v->cap = 0;
}

size_t cvec_len(const cvec *v) { return v->len; }
