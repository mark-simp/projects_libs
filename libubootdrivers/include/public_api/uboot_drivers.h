#include <sel4platsupport/io.h>

/**
 * initialise_uboot_drivers() - initialise the u-boot driver library.
 *
 * This must be called before any other interface exported by the library.
 *
 * @io_ops: the thread's platform support IO operations
 * @device_paths: list of strings for the device tree paths to handled
 * @device_count: the length of the device_paths list
 *
 * Return: 0 if OK, otherwise failure.
 */
int initialise_uboot_drivers(ps_io_ops_t *io_ops, const char **device_paths, uint32_t device_count);

/**
 * run_uboot_command() - executes a u-boot command as if entered at the
 *   u-boot command prompt
 *
 * @cmd: the command string
 *
 * Return: 0 if OK, otherwise failure.
 */
int run_uboot_command(char* cmd);

/**
 * uboot_stdin_tstc() - tests whether a character is available to read from
 *   u-boot's stdin.
 *
 * This routine can be used to capture data from a device mapped to u-boot's
 * stdin file, for example a USB keyboard.
 *
 * Return: 0 if no character is available, 1 otherwise.
 */
int uboot_stdin_tstc();

/**
 * uboot_stdin_tstc() - returns the next character available to read from
 *   u-boot's stdin. If no character is currently available this routine
 *   will block. To prevent blocking the uboot_stdin_tstc function should
 *   be used to determine whether a character is available.
 *
 * Return: The character.
 */
int uboot_stdin_getc();

/**
 * shutdown_uboot_drivers() - shutdown the u-boot driver library.
 */
void shutdown_uboot_drivers(void);