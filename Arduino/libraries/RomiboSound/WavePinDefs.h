/**
 * \file
 * Pin definitions
 */
#include <ArduinoPins.h>

#ifndef WavePinDefs_h
#define WavePinDefs_h

// Include the compile-time configuration.
#include <RomiboConfig.h>

//SPI pin definitions

/** SPI slave select pin. Warning: SS may be redefined as another pin
 but the hardware SS_PIN must be set to output mode before any calls to
 WaveHC functions. The SS_PIN can then be used as a general output pin */
#define SS   SS_PIN

/** SPI master output, slave input pin. */
#define MOSI MOSI_PIN

/** SPI master input, slave output pin. */
#define MISO MISO_PIN

/** SPI serial clock pin. */
#define SCK  SCK_PIN

//------------------------------------------------------------------------------
// DAC pin definitions

#if (ROMIBO_ELECTRONICS_MAJOR_REV==1)||(ROMIBO_ELECTRONICS_MAJOR_REV==2)

// LDAC may be connected to ground to save a pin
/** Set USE_MCP_DAC_LDAC to 0 if LDAC is grounded. */
#define USE_MCP_DAC_LDAC 1

/** Data direction register for DAC chip select. */
#define MCP_DAC_CS_DDR  PIN42_DDRREG
/** Port register for DAC chip select. */
#define MCP_DAC_CS_PORT PIN42_PORTREG
/** Port bit number for DAC chip select. */
#define MCP_DAC_CS_BIT  PIN42_BITNUM

// pin 3 is DAC serial clock
/** Data direction register for DAC clock. */
#define MCP_DAC_SCK_DDR  PIN43_DDRREG
/** Port register for DAC clock. */
#define MCP_DAC_SCK_PORT PIN43_PORTREG
/** Port bit number for DAC clock. */
#define MCP_DAC_SCK_BIT  PIN43_BITNUM

// pin 4 is DAC serial data in

/** Data direction register for DAC serial in. */
#define MCP_DAC_SDI_DDR  PIN44_DDRREG
/** Port register for DAC clock. */
#define MCP_DAC_SDI_PORT PIN44_PORTREG
/** Port bit number for DAC clock. */
#define MCP_DAC_SDI_BIT  PIN44_BITNUM

// pin 5 is LDAC if used
#if USE_MCP_DAC_LDAC
/** Data direction register for Latch DAC Input. */
#define MCP_DAC_LDAC_DDR  PIN45_DDRREG
/** Port register for Latch DAC Input. */
#define MCP_DAC_LDAC_PORT PIN45_PORTREG
/** Port bit number for Latch DAC Input. */
#define MCP_DAC_LDAC_BIT  PIN45_BITNUM
#endif // USE_MCP_DAC_LDAC

#endif // (ROMIBO_ELECTRONICS_MAJOR_REV==1)||(ROMIBO_ELECTRONICS_MAJOR_REV==2)

//------------------------------------------------------------------------------
// DAC pin definitions

#if (ROMIBO_ELECTRONICS_MAJOR_REV==3)

// LDAC may be connected to ground to save a pin
/** Set USE_MCP_DAC_LDAC to 0 if LDAC is grounded. */
#define USE_MCP_DAC_LDAC 0

#define MCP_DAC_CS_DDR  PIN34_DDRREG   /** Data direction register for DAC chip select. */
#define MCP_DAC_CS_PORT PIN34_PORTREG  /** Port register for DAC chip select. */
#define MCP_DAC_CS_BIT  PIN34_BITNUM   /** Port bit number for DAC chip select. */

// pin 3 is DAC serial clock
#define MCP_DAC_SCK_DDR  PIN35_DDRREG  /** Data direction register for DAC clock. */
#define MCP_DAC_SCK_PORT PIN35_PORTREG /** Port register for DAC clock. */
#define MCP_DAC_SCK_BIT  PIN35_BITNUM  /** Port bit number for DAC clock. */

// pin 4 is DAC serial data in
#define MCP_DAC_SDI_DDR  PIN36_DDRREG  /** Data direction register for DAC serial in. */
#define MCP_DAC_SDI_PORT PIN36_PORTREG /** Port register for DAC clock. */
#define MCP_DAC_SDI_BIT  PIN36_BITNUM  /** Port bit number for DAC clock. */

// pin 5 is LDAC if used
#if USE_MCP_DAC_LDAC
#define MCP_DAC_LDAC_DDR  PIN37_DDRREG /** Data direction register for Latch DAC Input. */
#define MCP_DAC_LDAC_PORT PIN37_PORTREG /** Port register for Latch DAC Input. */
#define MCP_DAC_LDAC_BIT  PIN37_BITNUM /** Port bit number for Latch DAC Input. */
#endif // USE_MCP_DAC_LDAC

#endif //  (ROMIBO_ELECTRONICS_MAJOR_REV==3)

//------------------------------------------------------------------------------

#if ROMIBO_AUDIO_PWM_ENABLED
// The audio PWM is generated using OC1C (Timer1 Output Compare C).
// On the Rev2 hardware this conflicts with the IR input; the IR receiver must be removed.
#define AUDIO_PWM_DDR  PIN13_DDRREG   /** Data direction register for audio PWM output. */
#define AUDIO_PWM_PORT PIN13_PORTREG  /** Port register for audio PWM output. */
#define AUDIO_PWM_BIT  PIN13_BITNUM   /** Port bit number for audio PWM output. */
#endif

//------------------------------------------------------------------------------

#endif // WavePinDefs_h
