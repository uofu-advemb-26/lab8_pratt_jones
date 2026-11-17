# This is the main binary. List your C files here.
# Activity 1
add_executable(activity1 activity1.c)
pico_enable_stdio_uart(activity1 1)
pico_enable_stdio_usb(activity1 1)
target_link_libraries(activity1 PRIVATE
pico_stdlib
pico_async_context_freertos
FreeRTOS-Kernel-Heap4
can2040
)
pico_add_extra_outputs(activity1)

# Activity 2
add_executable(activity2 activity2.c)
pico_enable_stdio_uart(activity2 1)
pico_enable_stdio_usb(activity2 1)
target_link_libraries(activity2 PRIVATE
pico_stdlib
pico_async_context_freertos
FreeRTOS-Kernel-Heap4
can2040
)
pico_add_extra_outputs(activity2)


find_program(PICOTOOL picotool)
add_custom_target(flash1 ${PICOTOOL} load -f $<TARGET_FILE:activity1> DEPENDS activity1)
add_custom_target(flash2 ${PICOTOOL} load -f $<TARGET_FILE:activity2> DEPENDS activity2)
