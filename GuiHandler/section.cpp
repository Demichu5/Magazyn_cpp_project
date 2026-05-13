#include "../Raylib/include/raylib.h"
#include "../Raylib/raygui.h"
#include "../Magazyn/magazynHandler.h"
#include "../Settings/settings.h"
#include <cstdlib>

char productCode[13] = {0};
extern int slotSwitch; // zmienna z render.cpp

void popWindowProductSection(int prodNumber, bool *state){
    /* popWindowProductSection() to funkcja ktora dodaje produkt do konkretnej pozycji w sekcji
    robi walidacje czy ciag liczb jest rowny 12 */
    Rectangle box = { (float)G_SCREEN_WIDTH/4, (float)G_SCREEN_HEIGHT/4, 400, 300 };
    

    int res = GuiTextInputBox(box, "DODAJ PRODUKT", "Wprowadz 12-cyfrowy kod UPC", "DODAJ;ANULUJ", productCode, 13, NULL);
    /* Tworzymy okno pop-up i przypisujemy wynik do zmiennej res
    1 - DODAJ
    2 lub więcej - ANULUJ/ZAMKNIJ */
    if(res == 1){
        if(TextLength(productCode) == 12){
            uint64_t code = strtoull(productCode, NULL, 10);
            g_magazyn.addByUPCAtSlot(prodNumber - 1, slotSwitch, code);
            /* wywolanie metody
            prodNumber - 1 = indeks sekcji
            slotSiwtch = wybrany wczesniej slot w sekcji
            code = numer UPC */
            for(int i = 0; i < 13; i++){
                productCode[i] = '\0';
            }
            *state = false;
        }
    } 
    else if(res >= 2){
        // czyszczenie bufora
        for(int i = 0; i < 13; i++){
            productCode[i] = '\0';
        }
        *state = false;
    }
}