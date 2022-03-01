/*
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdint.h>

#include "io.h"

#include "../usb_otg.h"
#include "../../services.h"

#include "../../xhci/uboot/types.h"
#include "../../xhci/xhci.h"
#include "../../xhci/dwc3.h"
#include "../../xhci/xhci-dwc3.h"

/* XHCI registers */
static void *_usb_regs = NULL;
static const int _usb_irqs[] = {
    [USB_HOST0] = USB2_HOST0_IRQ,
    [USB_HOST1] = USB2_HOST1_IRQ
};

int usb_host_init(enum usb_host_id id, ps_io_ops_t *io_ops, ps_mutex_ops_t *sync,
                  usb_host_t *hdev)
{
    int err;
    if (id < 0 || id > USB_NHOSTS) {
        return -1;
    }

    if (!io_ops || !hdev) {
        ZF_LOGF("Invalid arguments\n");
    }

    hdev->id = id;
    hdev->dman = &io_ops->dma_manager;
    hdev->sync = sync;

    /* Check device mappings */
    if (_usb_regs == NULL) {
        switch (id) {
            case USB_HOST0:
                _usb_regs = GET_RESOURCE(io_ops, USB2_HOST0_XHCI);
                break;
            case USB_HOST1:
                _usb_regs = GET_RESOURCE(io_ops, USB2_HOST1_XHCI);
                break;
            default:
                ZF_LOGF("Invalid host\n");
                return -1;
                break;
        }
    }
    if (_usb_regs == NULL) {
        return -1;
    }

    // err = xhci_dwc3_probe((uintptr_t)_usb_regs);

    struct xhci_hcor *hcor;
	struct xhci_hccr *hccr;
    struct dwc3 *dwc3_reg;

	hccr = (struct xhci_hccr *)((uintptr_t)_usb_regs);
	hcor = (struct xhci_hcor *)((uintptr_t)hccr +
			HC_LENGTH(xhci_readl(&(hccr)->cr_capbase)));

    dwc3_reg = (struct dwc3 *)((char *)(hccr) + DWC3_REG_OFFSET);

    dwc3_core_init(dwc3_reg);

    // revision = readl(&dwc3_reg->g_snpsid);
    // ZF_LOGD("Revision: 0x%x\n", revision);
	// /* This should read as U3 followed by revision number */
	// if ((revision & DWC3_GSNPSID_MASK) != 0x55330000 &&
	//     (revision & DWC3_GSNPSID_MASK) != 0x33310000) {
    //     ZF_LOGF("this is not a DesignWare USB3 DRD Core\n");
	// 	return -1;
	// }





    // err = xhci_host_init(hdev, (uintptr_t)_usb_regs, NULL);

    // return err;

    return 0;
}

const int *usb_host_irqs(usb_host_t *host, int *nirqs)
{
    if (host->id < 0 || host->id > USB_NHOSTS) {
        return NULL;
    }

    if (nirqs) {
        *nirqs = 1;
    }

    host->irqs = &_usb_irqs[host->id];
    return host->irqs;
}

int usb_plat_otg_init(usb_otg_t odev, ps_io_ops_t *io_ops)
{
    return -1;
}


