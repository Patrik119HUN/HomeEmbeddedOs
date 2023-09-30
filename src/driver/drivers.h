#pragma once
#include "digital_output/digital_output.h"
#include "full/full.h"
#include "null/null.h"
#include "rtc/rtc.h"
#include "screen/screen.h"
#include "terminal/terminal.h"
#include "zero/zero.h"
#include "random/random.h"
#include "ethernet/ethernet_adapter.h"
#include "wifi/wifi_adapter.h"

rtc rtcdev;
full fulldev;
zero zerodev;
null nulldev;
terminal term;
frandom randomdev;