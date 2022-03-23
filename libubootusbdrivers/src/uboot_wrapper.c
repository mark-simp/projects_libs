#include <stdlib.h>
#include <uboot_helper.h>
#include <linux/types.h>
#include <dm/root.h>
#include <dm/util.h>
#include <usb.h>
#include <asm-generic/global_data.h>
#include <fdtdec.h>
#include <of_live.h>
#include <driver_data.h>

/*
 * Global declaration of global_data.
 */
struct global_data* gd;

int init_uboot(char* fdt_blob)
{
    // Set up the driver data.
    initialise_driver_data();

    // Allocation of global_data.
    gd = malloc(sizeof(gd_t));

    // Initialisation of (unused sections of the) global_data.
    gd->bd = NULL;
    gd->flags = GD_FLG_RELOC; /* Work as if u-boot has finished relocation to RAM */
	gd->baudrate = 0;
	gd->cpu_clk = 0;
	gd->bus_clk = 0;
	gd->pci_clk = 0;
	gd->mem_clk = 0;
    gd->have_console = 1;
	gd->env_addr = 0;
	gd->env_valid = 0;
	gd->env_has_init = 0;
	gd->env_load_prio = 0;
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
    int ret = of_live_build(gd->fdt_blob, (struct device_node **)gd_of_root_ptr());
    assert(0 == ret);

    debug("Calling dm_init_and_scan\n");
    ret = dm_init_and_scan(false);
    assert(0 == ret);
    debug("Returned from dm_init_and_scan\n");

    // Dump out what info dm_init_and_scan has determined.
    printf("------\n\n");
    dm_dump_all();
    printf("------\n\n");
    dm_dump_static_driver_info();
    printf("------\n\n");
    dm_dump_drivers();
    printf("------\n\n");
    dm_dump_driver_compat();
    printf("------\n\n");
    dm_dump_uclass();
    printf("------\n\n");

    debug("Calling usb_init\n");
    ret = usb_init();
    assert(0 == ret);
    debug("Returned from usb_init\n");

    // Dump out what info dm_init_and_scan has determined.
    printf("------\n\n");
    dm_dump_all();
    printf("------\n\n");
    dm_dump_static_driver_info();
    printf("------\n\n");
    dm_dump_drivers();
    printf("------\n\n");
    dm_dump_driver_compat();
    printf("------\n\n");
    dm_dump_uclass();
    printf("------\n\n");

    debug("Calling usb_stor_scan\n");
    ret = usb_stor_scan(1);
    assert(0 == ret);
    debug("Returned from usb_stor_scan\n");

    return 0;
}