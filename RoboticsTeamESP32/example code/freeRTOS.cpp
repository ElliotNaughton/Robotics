
void Test1(void * parameter){
  for(;;){ //start infinite loop freeRTOS can break out of loop for other tasks
    //code to run during task
  }
}

xTaskCreate(
    Test1,    // Function that should be called
    "Testing",  // Name of the task (for debugging)
    1000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    NULL             // Task handle
);