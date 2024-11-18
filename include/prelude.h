#ifndef PRELUDE_H
#define PRELUDE_H

#define ARRAY(type) \
  struct array_##type { \
    int length; \
    int capacity; \
    type* items; \
  };

#endif
