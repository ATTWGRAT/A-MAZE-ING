#include "route.h"

int is_route_empty(route *r) { return r->last == -1; }

route* make_route() {
  route* s = malloc(sizeof *s);
  s->size = 200;
  s->last = -1;
  s->array = malloc(200 * sizeof *s->array);
  return s;
}

int push_route(route *r, linked_node p) {
  if (r->last == r->size - 1) {
    r->array = realloc(r->array, (r->size + 200) * sizeof *(r->array));
    if (r->array == NULL) {
      return -1;
    }
    r->size += 200;
  }

  r->array[++(r->last)] = p;

  return 0;
}

linked_node pop_route(route *proute) {
  linked_node p = proute->array[proute->last];

  proute->last--;

  return p;
}

linked_node peek_route(route *proute) { return proute->array[proute->last]; }
