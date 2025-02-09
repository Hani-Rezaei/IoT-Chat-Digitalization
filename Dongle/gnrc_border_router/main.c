/*
 * Copyright (C) 2015 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Example application for demonstrating the RIOT network stack
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>

#include "shell.h"
#include "msg.h"

#define MAIN_QUEUE_SIZE     (8) /**< Size of the main message queue */

/**
 * @brief Main message queue for the application
 * 
 * This queue is used for communication between the main thread and the shell thread,
 * ensuring that potentially fast incoming networking packets are processed.
 */
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

/**
 * @brief Main function for the RIOT border router application.
 * 
 * Initializes the message queue, prints a startup message, and runs the shell for user interaction.
 * 
 * @return 0 when the application ends (though this point is never reached).
 */
int main(void)
{
    /* Initialize the message queue to handle incoming packets */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    puts("RIOT border router example application");

    /* Start the shell for user interaction */
    puts("All up, running the shell now");
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    /* This line should never be reached */
    return 0;
}
