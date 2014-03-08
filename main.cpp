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
#include "can.h"  // CAN_init, RX, TX, etc.

int main(void)
{

    while(1)
    {
//Initialize, check if successful

        if(!(CAN_init(can1, 100, 10, 10, 0, 0)))   //CAN port 1, 100 kbps, 10-deep rx/tx queues,
        {                                          //ignore bus_off_cb and data_over_cb (0,0)
            printf("CAN initialization failed\n");
        }

        CAN_reset_bus(can1);

//Transmit
        can_data_t Query;      //Send data of:
        Query.qword = 0;       //
        Query.bytes[0] = 0x02; //02 = byte 0, 2 following bytes
        Query.bytes[1] = 0x01; //01 = byte 1, mode 01
        Query.bytes[2] = 0x05; //05 = byte 2, PID 05
        Query.bytes[3] = 0x00; //00 = byte 3, not used

        can_msg_t MSGtoSend;
        can_msg_t MSGtoRx;

        MSGtoSend.msg_id = 0x7DF;   //Broadcast message (all available ECUs)
        MSGtoSend.data = Query;     //Load message with the query data

        if(CAN_tx(can1,&MSGtoSend, 1000)) //Send Message with a 1 second timeout
        {
            printf("CAN msg sent to TX queue\n");
        }
//Receive
        if(CAN_rx(can1,&MSGtoRx, 1000))    //Poll for Message with a 1 second timeout
        {                                   //***Change to interrupt-driven in FreeRTOS
            printf("CAN msg received from RX queue\n");
        }
        can_data_t Response;

        Response = MSGtoRx.data;
        int test = Response.bytes[3];       //

        printf("Engine coolant Temp: %f\n", test);

        delay_ms(1000);
    }

    return -1;
}
