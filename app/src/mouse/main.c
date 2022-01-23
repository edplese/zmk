/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <kernel.h>
#include <zmk/mouse.h>
#include <device.h>
#include <devicetree.h>

#if IS_ENABLED(CONFIG_ZMK_MOUSE_WORK_QUEUE_DEDICATED)
K_THREAD_STACK_DEFINE(mouse_work_stack_area, CONFIG_ZMK_MOUSE_DEDICATED_THREAD_STACK_SIZE);
static struct k_work_q mouse_work_q;
#endif

struct k_work_q *zmk_mouse_work_q() {
#if IS_ENABLED(CONFIG_ZMK_MOUSE_WORK_QUEUE_DEDICATED)
    return &mouse_work_q;
#else
    return &k_sys_work_q;
#endif
}

BUILD_ASSERT(DT_NODE_HAS_COMPAT(DT_CHOSEN(zmk_mouse), avago_adns5030),
             "ADNS5030 not found");

const struct device *mouse_dev;

/*
#define ADNS5030 DT_INST(0, avago_adns5030)
#if DT_NODE_HAS_STATUS(ADNS5030, okay)
#define ADNS5030_LABEL DT_LABEL(ADNS5030)
#else
#error Your devicetree has no enabled nodes with compatible "avago,adns5030"
#define ADNS5030_LABEL "<none>"
#endif
*/

int zmk_mouse_init() {
#if IS_ENABLED(CONFIG_ZMK_MOUSE_WORK_QUEUE_DEDICATED)
    k_work_q_start(&mouse_work_q, mouse_work_stack_area,
                   K_THREAD_STACK_SIZEOF(mouse_work_stack_area),
                   CONFIG_ZMK_MOUSE_DEDICATED_THREAD_PRIORITY);
#endif

    mouse_dev = DEVICE_DT_GET(DT_CHOSEN(zmk_mouse));
    if (mouse_dev == NULL) {
        //LOG_DBG("Error getting adns5030");
    }

    return 0;
}