/*
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdint.h>
#include "dwc3.h"

void dwc3_set_mode(struct dwc3 *dwc3_reg, u32 mode);

void dwc3_core_soft_reset(struct dwc3 *dwc3_reg);

int dwc3_core_init(struct dwc3 *dwc3_reg);

void dwc3_set_fladj(struct dwc3 *dwc3_reg, u32 val);
