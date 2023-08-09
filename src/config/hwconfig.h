#pragma once

#if defined(SPAIOTCFG_SPAIOT)
#include "hwconfig_spaiot.h"

#elif defined(SPAIOTCFG_SPAIOT_EXTENDED)
#include "hwconfig_spaiot_extended.h"

#elif defined(SPAIOTCFG_SPAIOT_SHIELD_4051)
#include "hwconfig_spaiot_shield_4051.h"

#elif defined(SPAIOTCFG_SPAIOT_SHIELD_PCF8574)
#include "hwconfig_spaiot_shield_pcf8574.h"

#elif defined(SPAIOTCFG_SPAIOT_SHIELD_S3_4051)
#include "hwconfig_spaiot_shield_s3_4051.h"

#elif defined(SPAIOTCFG_SPAIOT_SHIELD_S3_PCF8574)
#include "hwconfig_spaiot_shield_s3_pcf8574.h"

#elif defined(SPAIOTCFG_CUSTOM)
#include "hwconfig_custom.h"

#endif
