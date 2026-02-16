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

/**
 * Point Structure for Task 4
 */
typedef struct
{
  int x,y;

} Point;


/**
 * Add to Point objects by Reference
 * @param a A Point reference
 * @param b A Point reference
 * @param result A Point reference that is the "addition" of each individual point
 */
void addReference(Point& a, Point& b, Point& result)
{

    // your implementation here
  return;
}

/**
 * Add together Point Structs when passed by pointer
 * @param a A Point pointer
 * @param b A Point pointer
 * @param result Pointer to Point object that is "addition" of each individual point
 */

void addPointer(Point* a, Point* b, Point* result)
{
  // your implementation here
}

/**
 * Task 3 Variables Declared here
 */
//since is modifiable, should end up in RAM
static int evilGlobalVar=0;

//since is static const, will end up in Flash
const static int constantVar=10;

//since is static const, should end up in Flash
//but is never read so it may be removed during compile.
const static int neverReferenced=10;

void app_init(void)
{
  /**
   * Task 1 - Variable Sizes
   */

  int charSize = sizeof(char);
  int halt_at_this_variable=-1;
  /**
   * Continue with other variable types below...
   */


  /**
   * Task 3 - Variable and Function Locations
   * Your code below...
   */


  /**
   * Task 4 - Structs
   * Your code below...
   */


  Point a, b, result, result2;
  a.x=0;
  a.y=10;

  b.x=27;
  b.y=15;

}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
}
