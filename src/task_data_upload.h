/** @file task_data_upload.h
 *  This file contains the definition for the data_upload task function
 */

#ifndef _TASK_DATA_UPLOAD_    // These lines prevent the contents of this file from
#define _TASK_DATA_UPLOAD_    // being #included more than once in any .cpp file

//includes
#include <Arduino.h>    

//Defining data_upload, a task function with a void pointer.
void data_upload (void* p);

#endif // _TASK_DATA_UPLOAD_
