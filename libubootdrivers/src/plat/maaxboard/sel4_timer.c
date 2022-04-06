#include <stdio.h>
#include <sel4platsupport/io.h>
#include <platsupport/mach/gpt.h>
#include <platsupport/plat/gpt_constants.h>
#include <platsupport/delay.h>

gpt_t *gpt = NULL;

int init_and_start_timer(ps_io_ops_t *io_ops, const char *timer_path)
{
    gpt_config_t config = {
        .io_ops = *io_ops,
        .user_callback = NULL,
        .user_callback_token = NULL,
        .device_path = (char *) timer_path,
        .prescaler = GPT_PRESCALER
    };

    gpt = malloc(sizeof(gpt_t));

    int ret = gpt_init(gpt, config);
    if (ret != 0) {
        ZF_LOGE("Failed to initialise timer.");
        free(gpt);
        gpt = NULL;
        return -1;
    }

    ret = gpt_start(gpt);
    if (ret != 0) {
        ZF_LOGE("Failed to start timer.");
        free(gpt);
        gpt = NULL;
        return -1;
    }

    return 0;
}


unsigned long timer_get_us(void) {
    if (gpt == NULL)
        return 0;
    else
        return gpt_get_time(gpt) / 1000;
}


unsigned long timer_get_ms(void) {
    return timer_get_us() / 1000;
}


unsigned long get_timer(unsigned long base) {
    unsigned long time = timer_get_ms();
    if (time <= base)
        return 0;
    else
        return time - base;
}