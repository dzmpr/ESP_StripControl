/*



*/
#include <Arduino.h>
#include <NVSettings.h>
#include <EEPROM.h>

NVSettings::NVSettings(uint16_t am):allocatedMemory(am){}

uint8_t NVSettings::checkSum(char* str) {
    uint8_t sum = 0;
    for (;*(str) != '\0'; str++) { //Passing across the string until zero byte
        sum += *(str); //Add symbol number to sum
    }
    sum = sum % 255; //Escape byte overflow
    return sum; 
}

uint8_t NVSettings::writeString(uint16_t addres, size_t size, char* str) {
    EEPROM.begin(allocatedMemory); //Initialize EEPROM
    if (strlen(str) > size-1) str[size-1] = '\0'; //If function receive overflowed string - cut it
    eepromUpdate(addres+size,checkSum(str)); //Updating check sum in eeprom
    uint8_t i = 0;
    for (;i < size; i++) {
        eepromUpdate(addres+i, (uint8_t)str[i]); //Updating each value in eeprom
        if (str[i] == '\0') break; //Break loop when string written
    }
    EEPROM.end(); //Close EEPROM
    return i;
}

uint8_t NVSettings::readString(uint16_t addres, size_t size, char* str) {
    EEPROM.begin(allocatedMemory); //Initialize EEPROM
    uint8_t cs = EEPROM.read(addres+size); //Read string's cs from EEPROM
    uint8_t i = 0;
    for (;i < size; i++) {
        str[i] = EEPROM.read(addres+i); //Read byte from EEPROM
        if (str[i] == '\0') break;
    }
    EEPROM.end(); //Close EEPROM
    if (cs != checkSum(str)) return 0; //When string's cs doesn't match readed cs return NULL ptr
    return i;
}

uint8_t NVSettings::writeByteArray(uint16_t addres, size_t size, uint8_t* array) {
    EEPROM.begin(allocatedMemory); //Initialize EEPROM
    uint8_t cs = 0, i = 0;
    for (; i < size; i++) {
        eepromUpdate(addres+i, array[i]);
        cs = (cs + array[i]) % 255;
    }
    eepromUpdate(addres+size, cs);
    EEPROM.end();
    return i;
}

uint8_t NVSettings::readByteArray(uint16_t addres, size_t size, uint8_t* array) {
    EEPROM.begin(allocatedMemory); //Initialize EEPROM
    uint8_t cs = EEPROM.read(addres+size); //Read string's cs from EEPROM
    uint8_t i = 0, sum = 0;
    for (;i < size; i++) {
        array[i] = EEPROM.read(addres+i); //Read byte from EEPROM
        sum = (sum + array[i]) % 255;
    }
    EEPROM.end(); //Close EEPROM
    if (cs != sum) return 0; //When string's cs doesn't match readed cs return NULL ptr
    return i;
}

bool NVSettings::eepromUpdate(uint16_t addres, uint8_t value) {
    uint8_t compare = EEPROM.read(addres); //Read value from eeprom
    #ifdef DEBUG
        Serial.print(value);
        Serial.print(" -> ");
        Serial.print(compare);
    #endif
    if (compare != value) { 
        #ifdef DEBUG
            Serial.println(" - written.");
        #endif
        EEPROM.write(addres, value); //Write value to cell
        return true; //True when in cell was different symbol
    }
    #ifdef DEBUG
        Serial.println();
    #endif
    return false; //False when in cell already same symbol
}

void NVSettings::setAllocatedMemory(uint16_t allmem) {
    allocatedMemory = allmem;
}

void NVSettings::writeByte(uint16_t addres, uint8_t* b) {
    EEPROM.begin(allocatedMemory);
    eepromUpdate(addres, *b);
    eepromUpdate(addres+1, ~(*b));
    EEPROM.end();
}

bool NVSettings::readByte(uint16_t addres, uint8_t* b) {
    EEPROM.begin(allocatedMemory);
    uint8_t straight = EEPROM.read(addres);
    uint8_t inverse = ~(EEPROM.read(addres+1));
    *b = straight;
    EEPROM.end();
    if (straight == inverse) return true;
    return false;
}