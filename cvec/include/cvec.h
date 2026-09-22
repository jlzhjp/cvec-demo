#pragma once

#include <stddef.h>

typedef struct {
  void *data;
  size_t len;
  size_t cap;
  size_t elem_size;
  size_t elem_align;
} cvec;

int cvec_init(cvec *v, size_t elem_size, size_t elem_align);
void cvec_destroy(cvec *v);

size_t cvec_len(const cvec *v);

int cvec_push(cvec *v, const void *elem);

const void *cvec_get(const cvec *v, size_t index);
void *cvec_get_mut(cvec *v, size_t index);
