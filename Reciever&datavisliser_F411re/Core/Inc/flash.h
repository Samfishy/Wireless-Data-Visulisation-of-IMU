/*
 * flash.h
 *
 *  Created on: Nov 9, 2025
 *      Author: samfishy
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

void W25_rst (void);
uint32_t W25_devID (void);

void W25_nRead (uint32_t stPage, uint8_t offset,uint32_t size,uint8_t *rData);
void W25_fRead (uint32_t stPage, uint8_t offset,uint32_t size,uint8_t *rData);

void W25_erase(uint16_t numSec);
void W25Q_Write_Page (uint32_t page, uint16_t offset, uint32_t size, uint8_t *data);

#endif /* INC_FLASH_H_ */
