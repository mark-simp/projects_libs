int initialise_uboot_drivers(char* fdt_blob);

int run_uboot_command(char* cmd);

// Provide acces to stdin test character and get character.
int uboot_stdin_tstc();
int uboot_stdin_getc();

void shutdown_uboot_drivers(void);