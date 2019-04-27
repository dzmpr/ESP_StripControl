/*



*/
#ifndef NVSettings_h
#define NVSettings_h
    #include <Arduino.h>
    #include <EEPROM.h>
    
    class NVSettings {
    private:
        uint16_t allocatedMemory;
        bool eepromUpdate(uint16_t, uint8_t);
        uint8_t checkSum(char*);
    public:
        uint8_t writeString(uint16_t, size_t, char*);
        uint8_t readString(uint16_t, size_t, char*);
        uint8_t writeByteArray(uint16_t, size_t, uint8_t*);
        uint8_t readByteArray(uint16_t, size_t, uint8_t*);
        bool readByte(uint16_t, uint8_t*);
        void writeByte(uint16_t, uint8_t*);
        void setAllocatedMemory(uint16_t);
        NVSettings(uint16_t);
    };
#endif