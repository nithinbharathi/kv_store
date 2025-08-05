project(kv_store C)
include_directories(include)
add_executable(kv_store src/main.c src/kv_store.c src/hash.c)

add_executable(kv_test tests/test.c src/kv_store.c src/hash.c)