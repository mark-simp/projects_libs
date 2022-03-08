#include <stdlib.h>
#include <uboot_helper.h>
#include <linux/types.h>
#include <dm/root.h>
#include <usb.h>

#include <asm/global_data.h>

/*
 * Global declaration of gd.
 */
struct global_data* gd;


int sel4_usb_init(void)
{
    // Allocation of global_data
    gd = malloc(sizeof(*gd));

    // Initialisation of (unused sections of the) global_data
    gd->bd = NULL;
    gd->flags = 0;
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

    // Initialisation of the DM specific parts of global_data
    gd->dm_root = NULL;
    gd->dm_root_f = NULL;
	gd->uclass_root_s.next = NULL;
	gd->uclass_root_s.prev = NULL;
	gd->uclass_root = NULL;

    // Initialisation of the (unused) FDT parts of global_data
    gd->fdt_blob = NULL;
	gd->new_fdt =NULL;
	gd->fdt_size = 0;
	gd->fdt_src = 0;

    // Initialisation of open-firmware data (a pre-parsed device tree)
    gd->of_root = NULL; // TODO: Need to define this!


    debug("Calling dm_init\n");
    int ret = dm_init(CONFIG_IS_ENABLED(OF_LIVE));
    assert(0 == ret);
    debug("Returned from dm_init\n");

    debug("usb_init usb_init\n");
    ret = usb_init();
    assert(0 == ret);
    debug("Returned from usb_init\n");

    return 0;
}