/*
 * Copyright 2014, NICTA
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(NICTA_BSD)
 */

#include "mmc.h"
#include "sdhc.h"
#include "services.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

static uint32_t slice_bits(uint32_t *val, int start, int size)
{
	int idx;
	int high, low;
	uint32_t ret = 0;

	/* Can not return more than 32 bits. */
	assert(size <= 32);

	idx = start / 32;
	low = start % 32;
	high = (start + size) % 32;

	if (high == 0 && low == 0) {
		ret = val[idx];
	} else if (high == 0 && low != 0) {
		ret = val[idx] >> low;
	} else {
		if (high > low) {
			ret = val[idx] & ((1U << high) - 1);
			ret = ret >> low;
		} else {
			ret = val[idx] >> low;
			ret |= (val[idx + 1] & ((1U << high) - 1)) << (32 - low);
		}

	}

	return ret;
}

#if 0 /* Commenting this out as it appears unused. */
static int mmc_decode_cid(mmc_card_t mmc_card, struct cid *cid)
{
	if (mmc_card == NULL || cid == NULL) {
		return -1;
	}

	if (mmc_card->type == CARD_TYPE_SD) {
		cid->manfid         = slice_bits(mmc_card->raw_cid, 120,  8);
		cid->sd_cid.oemid   = slice_bits(mmc_card->raw_cid, 104, 16);
		cid->sd_cid.name[0] = slice_bits(mmc_card->raw_cid,  96,  8);
		cid->sd_cid.name[1] = slice_bits(mmc_card->raw_cid,  88,  8);
		cid->sd_cid.name[2] = slice_bits(mmc_card->raw_cid,  80,  8);
		cid->sd_cid.name[3] = slice_bits(mmc_card->raw_cid,  72,  8);
		cid->sd_cid.name[4] = slice_bits(mmc_card->raw_cid,  64,  8);
		cid->sd_cid.rev     = slice_bits(mmc_card->raw_cid,  56,  8);
		cid->sd_cid.serial  = slice_bits(mmc_card->raw_cid,  24, 32);
		cid->sd_cid.date    = slice_bits(mmc_card->raw_cid,   8, 12);

		printf("manfid(%x), oemid(%x), name(%c%c%c%c%c), rev(%x), serial(%x), date(%x)\n",
			cid->manfid, cid->sd_cid.oemid,
			cid->sd_cid.name[0], cid->sd_cid.name[1], cid->sd_cid.name[2],
			cid->sd_cid.name[3], cid->sd_cid.name[4],
			cid->sd_cid.rev, cid->sd_cid.serial, cid->sd_cid.date);
	} else {
		printf("Not Implemented!\n");
		return -1;
	}

	return 0;
}
#endif

static int mmc_decode_csd(mmc_card_t mmc_card, struct csd *csd)
{
	if (mmc_card == NULL || csd == NULL) {
		return -1;
	}

#define CSD_BITS(start, size) \
	slice_bits(mmc_card->raw_csd, start, size)

	csd->structure = CSD_BITS(126, 2);

	if (csd->structure == 0) {
		printf("CSD Version 1.0\n");
		csd->c_size      = CSD_BITS(62, 12);
		csd->c_size_mult = CSD_BITS(47,  3);
		csd->read_bl_len = CSD_BITS(80,  4);
		csd->tran_speed  = CSD_BITS(96,  8);
	} else if (csd->structure == 1) {
		printf("CSD Version 2.0\n");
		csd->c_size      = CSD_BITS(48, 22);
		csd->c_size_mult = 0;
		csd->read_bl_len = CSD_BITS(80,  4);
		csd->tran_speed  = CSD_BITS(96,  8);
	} else {
		printf("Unknown CSD version!\n");
		return -1;
	}

	return 0;
}

int mmc_default_id(void){
    /* We may have an interface that is not sdhc? */
    return plat_sdhc_default_id();
}


int
mmc_init(int id, mmc_card_t* mmc_card,
         struct dma_allocator* dma_allocator,
         struct ps_io_mapper* io_map){
    mmc_card_t mmc;
    sdhc_dev_t sdhc = NULL;

    /* Allocate the mmc card structure */
    mmc = (mmc_card_t)_malloc(sizeof(*mmc));
    assert(mmc);
    if(!mmc){
        return -1;
    }

    /* Need some abstraction here... May not be an sdhc iface */
    sdhc = sdhc_plat_init(id, mmc, dma_allocator, io_map);
    assert(sdhc);
    if(!sdhc){
        free(mmc);
        return -1;
    }
    mmc->dalloc = dma_allocator;
    *mmc_card = mmc;
    assert(mmc);
    return 0;
}

unsigned long
mmc_block_read(mmc_card_t mmc_card, unsigned long start,
               int nblocks, void* data){
    /* sdhc has a standard register set so this is a poor level
     * of abstraction */
    /* Try write one block */
    struct mmc_data mdata;
    dma_mem_t dma_buf;
    void* buf;
    int bs = 512;
    int bytes = bs * nblocks;
    unsigned long ret;
    /* Allocate the dma buffer */
    buf = dma_alloc(mmc_card->dalloc, bytes, 0x1000, DMAF_HRW, &dma_buf);
    assert(buf);
    /* Populate the data descriptor */
    mdata.dma_buf = dma_buf;
    mdata.data_addr = start;
    mdata.block_size = bs;
    mdata.blocks = nblocks;
    /* Write the block */
    ret = sdhc_card_block_read(mmc_card, &mdata);
    /* Copy in the data */
    memcpy(data, buf, ret);
    dma_free(dma_buf);
    return ret;
}

unsigned long
mmc_block_write(mmc_card_t mmc_card, unsigned long start,
                int nblocks, const void* data){
    /* sdhc has a standard register set so this is a poor level
     * of abstraction */
    struct mmc_data mdata;
    dma_mem_t dma_buf;
    void* buf;
    int bs = 512;
    int bytes = bs * nblocks;
    unsigned long ret;
    /* Allocate the dma buffer */
    buf = dma_alloc(mmc_card->dalloc, bytes, 0x1000, DMAF_HRW, &dma_buf);
    assert(buf);
    /* Populate the data descriptor */
    mdata.dma_buf = dma_buf;
    mdata.data_addr = start;
    mdata.block_size = bs;
    mdata.blocks = nblocks;
    /* Copy in the data */
    memcpy(buf, data, bytes);
    /* Write the block */
    ret = sdhc_card_block_write(mmc_card, &mdata);
    dma_free(dma_buf);
    return ret;
}

unsigned long long
mmc_card_capacity(mmc_card_t mmc_card) {
	int ret;
	unsigned long long capacity;
	struct csd csd;

	ret = mmc_decode_csd(mmc_card, &csd);
	if (ret) {
		return -1;
	}

	if (csd.structure == 0) {
		capacity = (csd.c_size + 1) * (1U << (csd.c_size_mult + 2));
		capacity *= (1U << csd.read_bl_len);
	} else if (csd.structure == 1) {
		capacity = (csd.c_size + 1) * 512 * 1024;
	} else {
		return -1;
	}

	return capacity;
}

