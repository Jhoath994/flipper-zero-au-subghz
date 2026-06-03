#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <furi_hal_gpio.h>
#include <lib/toolbox/level_duration.h>

/*
 * External CC1101 bit-bang driver for Flipper Zero FAP.
 *
 * Pin mapping (bypass OS SPI bus locks):
 *   CS   = PC0  (Pin 16, rerouted from PA4 to avoid OS auto-lock)
 *   SCK  = PB3  (Pin 5)
 *   MOSI = PA7  (Pin 2)
 *   MISO = PA6  (Pin 3)
 *   GDO0 = PB2  (Pin 6)
 */

/* ---- Pin definitions ---- */
#define EXT_CC1101_CS_PIN      (&gpio_ext_pc0)
#define EXT_CC1101_SCK_PIN     (&gpio_ext_pb3)
#define EXT_CC1101_MOSI_PIN   (&gpio_ext_pa7)
#define EXT_CC1101_MISO_PIN    (&gpio_ext_pa6)
#define EXT_CC1101_GDO0_PIN   (&gpio_ext_pb2)

/* ---- CC1101 register addresses ---- */
#define CC1101_IOCFG0     0x02
#define CC1101_FREQ2      0x0D
#define CC1101_FREQ1      0x0E
#define CC1101_FREQ0      0x0F
#define CC1101_MDMCFG2    0x12
#define CC1101_MCSM0      0x14
#define CC1101_AGCTRL2    0x1B
#define CC1101_AGCTRL1    0x1C
#define CC1101_AGCTRL0    0x1D
#define CC1101_FREND0     0x22
#define CC1101_FSCAL3     0x23
#define CC1101_FSCAL2     0x24
#define CC1101_FSCAL1     0x25
#define CC1101_FSCAL0     0x26
#define CC1101_PKTCTRL0   0x08
#define CC1101_PA_TABLE0  0x3E
#define CC1101_TXBYTES    0x7B
#define CC1101_RXBYTES    0x7C
#define CC1101_MARCSTATE  0x35

/* ---- CC1101 command strobes ---- */
#define CC1101_SIDLE      0x36
#define CC1101_STX        0x35
#define CC1101_SRX        0x34
#define CC1101_SFTX       0x3B
#define CC1101_SFRX       0x3A
#define CC1101_SRES       0x30

/* ---- CC1101 FIFO address ---- */
#define CC1101_FIFO       0x3F

/* ---- SPI command bit patterns ---- */
#define CC1101_WRITE_BURST  0x40
#define CC1101_READ_BURST   0xC0
#define CC1101_WRITE_SINGLE 0x00
#define CC1101_READ_SINGLE  0x80

/* ---- DWT cycles per microsecond at 64 MHz ---- */
#define DWT_CYCLES_PER_US  64

/* ---- Public API ---- */

void ext_cc1101_init(void);
void ext_cc1101_deinit(void);
void ext_cc1101_reset(void);
bool ext_cc1101_check_present(void);
void ext_cc1101_write_reg(uint8_t addr, uint8_t val);
uint8_t ext_cc1101_read_reg(uint8_t addr);
void ext_cc1101_write_burst(uint8_t addr, const uint8_t* data, uint16_t len);
void ext_cc1101_set_frequency(uint32_t hz);
void ext_cc1101_start_cw_jam(uint32_t freq_hz, int32_t offset_hz);
void ext_cc1101_stop_cw_jam(void);
void ext_cc1101_start_async_tx(uint32_t freq_hz);
void ext_cc1101_stop_async_tx(void);
void ext_cc1101_atomic_replay_signal(const LevelDuration samples[], size_t count);
