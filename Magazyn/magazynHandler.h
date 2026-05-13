#ifndef MAGAZYN_HANDLER_H
#define MAGAZYN_HANDLER_H

#include <cstdint>

struct UPC {
    uint8_t type;
    uint32_t id;
    uint8_t year;
};

class Product {
public:
    uint64_t upcRaw;
    UPC decoded;

    Product() : upcRaw(0) { decoded = {0,0,0}; }

    Product(uint64_t code) : upcRaw(code) {
    // Format 12-cyfrowy: TT-IIIIII-RR-VV
        decoded.type = code / 100000000000ULL;
        decoded.id   = (code / 10000ULL) % 1000000ULL;
        decoded.year = (code / 100ULL) % 100ULL;
}

    virtual ~Product() {}
    virtual const char* getTypeName() = 0;
};

class SolidProduct : public Product {
public:
    SolidProduct() : Product(0) {} 
    SolidProduct(uint64_t c) : Product(c) {}
    const char* getTypeName() override { return "STALY"; }
};

class Section {
public:
    Product* slots[6];
    int count;

    Section() : count(0) {
        for(int i = 0; i < 6; i++) slots[i] = nullptr;
    }

    bool add(Product* p) {
        if (count < 6 && p != nullptr) {
            slots[count++] = p;
            return true;
        }
        return false;
    }

    void clear() {
        count = 0;
        for(int i = 0; i < 6; i++) slots[i] = nullptr;
    }
    
    void remove(int slotIdx) {
        if (slotIdx >= 0 && slotIdx < 6) {
            slots[slotIdx] = nullptr;
        
            int newCount = 0;
            for(int i = 0; i < 6; i++) {
                if(slots[i] != nullptr) newCount++;
            }
        count = newCount;
        }
    }
    void addAtSlot(int slotIdx, Product* p) {
        if (slotIdx >= 0 && slotIdx < 6) {
            slots[slotIdx] = p;
            int currentCount = 0;
            for(int i = 0; i < 6; i++) {
                if(slots[i] != nullptr) currentCount++;
            }
            count = currentCount;
        }
    }
};

class Magazyn {
public:
    Section sections[6];
    SolidProduct pool[36];
    int poolIdx = 0;

    void addByUPC(int secIdx, uint64_t code) {
        if (secIdx >= 0 && secIdx < 6 && poolIdx < 36) {
            pool[poolIdx] = SolidProduct(code);
            sections[secIdx].add(&pool[poolIdx++]);
        }
    }
    void addByUPCAtSlot(int secIdx, int slotIdx, uint64_t code) {
        if (secIdx >= 0 && secIdx < 6 && poolIdx < 36) {
            pool[poolIdx] = SolidProduct(code);
            sections[secIdx].addAtSlot(slotIdx, &pool[poolIdx++]);
        }
    }
};

extern Magazyn g_magazyn;
#endif