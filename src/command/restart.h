#pragma once

#include <SimpleCLI.h>
#include <Arduino.h>

void static restartCallback(cmd* c){
    Command cmd(c);
    
    Serial.println("System force restart");
    NVIC_SystemReset();
    return;
}