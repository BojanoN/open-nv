#include "util/container.h"

#define INIT_CONSTRUCTOR_MAP(type) type##_init_constructor_map()
#define FREE_CONSTRUCTOR_MAP(type) shfree(type##_constructor_map)

#define FETCH_CONSTRUCTOR_MAP(type)      extern type##ConstructorEntry* type##_constructor_map
#define GET_CONSTRUCTOR(type, key)       shget(type##_constructor_map, (key))
#define ADD_CONSTRUCTOR(type, key, func) shput(type##_constructor_map, (key), (func))

#define INIT_DESTRUCTOR_MAP(type) type##_init_destructor_map()
#define FREE_DESTRUCTOR_MAP(type) shfree(type##_destructor_map)

#define FETCH_DESTRUCTOR_MAP(type)      extern type##DestructorEntry* type##_destructor_map
#define GET_DESTRUCTOR(type, key)       shget(type##_destructor_map, (key))
#define ADD_DESTRUCTOR(type, key, func) shput(type##_destructor_map, (key), (func))

#define DEFINE_OBJECT_TYPEDEFS(type)        \
    typedef type* type##Constructor(FILE*); \
    typedef void  type##Destructor(type*);  \
    typedef struct {                        \
        sds                key;             \
        type##Constructor* value;           \
    } type##ConstructorEntry;               \
    typedef struct {                        \
        sds               key;              \
        type##Destructor* value;            \
    } type##DestructorEntry

#define DECLARE_MAP_INITIALIZERS(type)  \
    void type##_init_constructor_map(); \
    void type##_init_destructor_map()

#define DECLARE_FUNCTION_MAPS(type)                        \
    type##ConstructorEntry* type##_constructor_map = NULL; \
    type##DestructorEntry*  type##_destructor_map  = NULL
