/* Routines to perform initialisation and shutdown of the U-Boot wrapper.
 * The initialise routine performs the actions that would normally be
 * performed by U-Boot when it is started.
 */

int initialise_uboot_wrapper(char* fdt_blob);

void shutdown_uboot_wrapper(void);