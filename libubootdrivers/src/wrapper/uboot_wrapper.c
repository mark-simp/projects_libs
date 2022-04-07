#include <uboot_helper.h>
#include <dm/root.h>
#include <dm/util.h>
#include <usb.h>
#include <asm-generic/global_data.h>
#include <fdtdec.h>
#include <of_live.h>
#include <driver_data.h>
#include <stdio_dev.h>
#include <console.h>
#include <mmc.h>

// Global declaration of global_data.
struct global_data* gd;

// Global declaration of errno_uboot.
int errno_uboot = 0;

// Global declaration of version_string.
const char version_string[] = "seL4 U-Boot driver";

// State determining whether the library has been initialised.
static bool library_initialised = false;

int initialise_uboot_wrapper(char* fdt_blob)
{
    // Return immediately if already initialised (nothing to do).
    if (library_initialised) return 0;

    // Set up the driver data.
    initialise_driver_data();

    // Allocation of global_data.
    gd = malloc(sizeof(gd_t));
    if (gd == NULL)
        return -ENOMEM;

    // Initialisation of (unused sections of the) global_data.
    gd->bd = NULL;
    gd->flags = 0;
    gd->flags |= GD_FLG_RELOC;  /* Work as if u-boot has finished relocation to RAM */
    gd->baudrate = 0;
	gd->cpu_clk = 0;
	gd->bus_clk = 0;
	gd->pci_clk = 0;
	gd->mem_clk = 0;
    gd->have_console = 1;
	gd->ram_base = 0;
	gd->ram_top = 0;
	gd->relocaddr = 0;
	gd->ram_size = 0;
	gd->mon_len = 0;
	gd->irq_sp = 0;
	gd->start_addr_sp = 0;
	gd->reloc_off = 0;
	gd->new_gd = NULL;
    gd->jt = NULL;
    gd->env_buf[0] = '\n';
    gd->timebase_h = 0;
	gd->timebase_l = 0;
    gd->cur_serial_dev = NULL;

    // Initialisation of the DM specific parts of global_data.
    gd->dm_root = NULL;
    gd->dm_root_f = NULL;
	gd->uclass_root_s.next = NULL;
	gd->uclass_root_s.prev = NULL;
	gd->uclass_root = NULL;

    // Manually initialise of the FDT parts of global_data as if FDT handling
    // had been set up for an embedded FDT.
    gd->fdt_blob = fdt_blob;
	gd->new_fdt = NULL;
	gd->fdt_size = fdt_totalsize(fdt_blob);
	gd->fdt_src = FDTSRC_EMBED;

    // Build the live tree from the FDT.
    gd->of_root = NULL;
    int ret = of_live_build(gd->fdt_blob, (struct device_node **)gd_of_root_ptr());
    if (0 != ret)
        goto error;

    // Manually initialise the environment system. We do this by marking the
    // the pre-relocation environment as invalid, i.e. not present, and then
    // asking for the environment to be relocated to RAM. This will set up
    // the RAM with the default environment as a starting point.
    gd->env_addr = 0;
	gd->env_valid = ENV_INVALID;
	gd->env_has_init = 0;
	gd->env_load_prio = 0;
    env_relocate();

    // Initialise the stdio files / devices and the (stubbed) cosole.
    ret = stdio_init();
    if (0 != ret)
        goto error;
    ret = console_init_r();
    if (0 != ret)
        goto error;

    // Scan the device tree for compatible drivers.
    ret = dm_init_and_scan(false);
    if (0 != ret)
        goto error;

#ifdef CONFIG_DM_MMC
    // Initialize the MMC system.
    ret = mmc_initialize(NULL);
    if (0 != ret)
        goto error;
#endif

    // Success.
    library_initialised = true;
    return 0;

error:
    // Failed to initialise library, clean up and return error code.
    free(gd);
    gd = NULL;
    return -1;
}

int run_uboot_command(char* cmd)
{
    // Fail immediately if library not initialised.
    if (!library_initialised)
        return -1;

    // Perform the command.
    return run_command(cmd, CMD_FLAG_ENV);
}

void shutdown_uboot_wrapper(void)
{
    // Return immediately if library not initialsed (nothing to do).
    if (!library_initialised)
        return;

    // Delete persistant state.
    free(gd);
    gd = NULL;

    return;
}

int uboot_stdin_tstc(void)
{
    if (NULL == stdio_devices[UBOOT_STDIN])
        return 0;

    return stdio_devices[UBOOT_STDIN]->tstc(stdio_devices[UBOOT_STDIN]);
}

int uboot_stdin_getc(void)
{
    if (NULL == stdio_devices[UBOOT_STDIN])
        return 0;

    return stdio_devices[UBOOT_STDIN]->getc(stdio_devices[UBOOT_STDIN]);
}