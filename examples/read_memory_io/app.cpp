/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
#include <stdint.h>
void app_init(void)
{
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
#define RAM_SIZE_REG_ADDR 0xFE081FAUL
#define PART_NUM_REG_ADDR 0xFE081FCUL
#define PART_FAMILY_REG_ADDR 0xFE081FEUL


void app_process_action(void)
{

  volatile uint16_t* ramSizePtr = (uint16_t*)RAM_SIZE_REG_ADDR;
  volatile uint16_t* partNumPtr = (uint16_t*)PART_NUM_REG_ADDR;
  volatile uint8_t* partFamilyPtr = (uint8_t*)PART_FAMILY_REG_ADDR;

  unsigned int ramSize = *ramSizePtr;
  unsigned int partNumber = *partNumPtr;
  unsigned int partFamily = *partFamilyPtr;



  int dummy=3+3;
}
