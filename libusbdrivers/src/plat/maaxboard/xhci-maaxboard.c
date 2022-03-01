/*
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdint.h>
#include <uboot_io.h>
#include <usb/plat/usb.h>

#include "../usb_otg.h"
#include "../../services.h"

#include "../../xhci/types.h"
#include "../../xhci/xhci.h"
#include "../../xhci/dwc3.h"
#include "../../xhci/xhci-dwc3.h"

/* XHCI registers */
static void *_usb_regs[]= {
    [USB_HOST0] = NULL,
    [USB_HOST1] = NULL
};

static const int _usb_irqs[] = {
    [USB_HOST0] = USB2_HOST0_IRQ,
    [USB_HOST1] = USB2_HOST1_IRQ
};

static int imx8_xhci_core_init(struct dwc3 *dwc3_reg)
{
    /* Initialise the DWC3 core */
    int err = dwc3_core_init(dwc3_reg);
    if (err) {
        return err;
    }

    /* Set dwc3 usb2 phy config */
    u32 reg = readl(&dwc3_reg->g_usb2phycfg[0]);

    if (DWC3_UTMI_WIDE) {
        reg |= DWC3_GUSB2PHYCFG_PHYIF;
        reg &= ~DWC3_GUSB2PHYCFG_USBTRDTIM_MASK;
        reg |= DWC3_GUSB2PHYCFG_USBTRDTIM_16BIT;
    }

    if (DWC3_DIS_ENBLSLPM_QUIRK)
        reg &= ~DWC3_GUSB2PHYCFG_ENBLSLPM;

    if (DWC3_DIS_U2_FREECLK_EXISTS_QUIRK)
        reg &= ~DWC3_GUSB2PHYCFG_U2_FREECLK_EXISTS;

    if (DWC3_DIS_U2_SUPPLY_QUIRK)
        reg &= ~DWC3_GUSB2PHYCFG_SUSPHY;

    writel(reg, &dwc3_reg->g_usb2phycfg[0]);

    /* Hard-code DWC3 core to Host Mode */
	dwc3_set_mode(dwc3_reg, DWC3_GCTL_PRTCAP_HOST);

    return 0;
}

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
    if (_usb_regs[id] == NULL) {
        switch (id) {
            case USB_HOST0:
                _usb_regs[id] = GET_RESOURCE(io_ops, USB2_HOST0_XHCI);
                break;
            case USB_HOST1:
                _usb_regs[id] = GET_RESOURCE(io_ops, USB2_HOST1_XHCI);
                break;
            default:
                ZF_LOGF("Invalid host\n");
                return -1;
                break;
        }
    }
    if (_usb_regs[id] == NULL) {
        return -1;
    }

    /* Create references to mapped data structures */
	struct xhci_hccr *hccr = (struct xhci_hccr *)((uintptr_t)_usb_regs[id]);
    struct dwc3 *dwc3_reg = (struct dwc3 *)((char *)(hccr) + DWC3_REG_OFFSET);

    err = imx8_xhci_core_init(dwc3_reg);
    if (err) {
        ZF_LOGD("failed to initialise core\n");
        return err;
    }

    err = xhci_host_init(hdev, hccr);

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
