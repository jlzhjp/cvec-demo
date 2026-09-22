#include "cvec.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int is_power_of_two(size_t x) { return x != 0 && (x & (x - 1)) == 0; }

int cvec_init(cvec *v, size_t elem_size, size_t elem_align) {
  if (v == NULL || elem_size == 0 || !is_power_of_two(elem_align)) {
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

static void *cvec_alloc(size_t alignment, size_t size) {
  if (size == 0) {
    return NULL;
  }

  size_t rem = size % alignment;

  if (rem != 0) {
    size_t padding = alignment - rem;

    if (size > SIZE_MAX - padding) {
      return NULL;
    }

    size += padding;
  }

  return aligned_alloc(alignment, size);
}

static int cvec_grow(cvec *v) {
  size_t new_cap = 0;

  if (v->cap == 0) {
    new_cap = 4;
  } else {
    if (v->cap > SIZE_MAX / 2) {
      return -1;
    }
    new_cap = v->cap * 2;
  }

  if (new_cap > SIZE_MAX / v->elem_size) {
    return -1;
  }

  size_t new_size = new_cap * v->elem_size;

  void *new_data = cvec_alloc(v->elem_align, new_size);

  if (new_data == NULL) {
    return -1;
  }

  if (v->data != NULL) {
    memcpy(new_data, v->data, v->len * v->elem_size);
  }

  free(v->data);

  v->data = new_data;
  v->cap = new_cap;

  return 0;
}

int cvec_push(cvec *v, const void *elem) {
  if (v == NULL || elem == NULL) {
    return -1;
  }

  if (v->len == v->cap) {
    if (cvec_grow(v) != 0) {
      return -1;
    }
  }

  unsigned char *base = v->data;

  void *dst = base + v->len * v->elem_size;

  memcpy(dst, elem, v->elem_size);

  ++v->len;

  return 0;
}

const void *cvec_get(const cvec *v, size_t index) {
  if (v == NULL || index >= v->len) {
    return NULL;
  }

  const unsigned char *base = v->data;

  return base + index * v->elem_size;
}

void *cvec_get_mut(cvec *v, size_t index) {
  if (v == NULL || index >= v->len) {
    return NULL;
  }

  unsigned char *base = v->data;

  return base + index * v->elem_size;
}
