/*
 *     SocialLedge.com - Copyright (C) 2013
 *
 *     This file is part of free software framework for embedded processors.
 *     You can use it and/or distribute it as long as this copyright header
 *     remains unmodified.  The code is free for personal use and requires
 *     permission to use in a commercial product.
 *
 *      THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *      OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *      I SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 *      CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *     You can reach the author of this software at :
 *          p r e e t . w i k i @ g m a i l . c o m
 */

/**
 * @file
 * @brief This is the application entry point.
 */
#include <stdio.h>      // printf() / scanf()
#include "io.hpp"       // Board sensors
#include "utilities.h"  // delay_ms()


int main(void)
{
    // Repeat the code inside while(1) forever
    while(1)
    {
        can_data_t DataToSend;
        DataToSend.dwords[0] = '02010500';

        can_msg_t *MSGtoSend;
        can_msg_t *MSGtoRx;
        MSGtoSend->msg_id = '7DF';
        MSGtoSend->data = DataToSend;

        if(!(CAN_init(can1, 100, 10, 10, 0, 0)))
        {
            printf("CAN initialization failed");
        }

        CAN_reset_bus(can1);

        CAN_tx(can1,MSGtoSend, 1000);



        CAN_rx(can1,MSGtoRx, 1000);

        can_data_t DataToRx;

        DataToRx = MSGtoRx->data;
        int test = DataToRx.bytes[3];

        printf("Engine coolant Temp: %f\n", test);

        delay_ms(1000);
    }

    return -1;
}
