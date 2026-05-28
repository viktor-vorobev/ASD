add_executable(Application main.cpp)

target_link_libraries(Application PRIVATE
    lib_polynom
    lib_list
    lib_queue
    lib_binary_tree
    lib_avl_tree
    lib_rb_tree
    lib_itable
    lib_sorted_table_avl
    lib_sorted_table_rb
)