/*
 * Copyright (c) 2021 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <drivers/behavior.h>
#include <logging/log.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/event_manager.h>
#include <zmk/events/mouse_button_state_changed.h>
#include <zmk/events/mouse_move_state_changed.h>
#include <zmk/events/mouse_scroll_state_changed.h>
#include <zmk/events/mouse_tick.h>
#include <zmk/hid.h>
#include <zmk/endpoints.h>
#include <zmk/mouse.h>
