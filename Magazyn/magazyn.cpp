#include "magazynHandler.h"
#include "../Raylib/include/raylib.h"
#include <stdio.h>
#include <string>
#include <cinttypes>

bool eksportujTransport(int secIdx){
    /*Ta funkcja generuje raport w postaci pliku tekstowego w lokalizacji folderu /Transport
    dla konkretnej sekcji secIdx oraz czysci sekcje
    */
    if(secIdx < 0 || secIdx >= 6){
        // prosta walidacja
        return false;
    }
    Section* s = &g_magazyn.sections[secIdx];
    if(s->count == 0){
        return false;
    }
    int transportIdx = 1;
    const char* idxPath = "Database/transportIndex.txt";
    // zmienna ktora odcztuje aktualny numer indeksu transportu z folderu /Database

    if(FileExists(idxPath)){
        // walidacja numeru, zmiana znaku Char na Int
        char* text = LoadFileText(idxPath);
        if(text){
            transportIdx = atoi(text);
            UnloadFileText(text);
        }
    }

    std::string fileName = "Transport/wyjazd_" + std::to_string(transportIdx) + ".txt";
    // Polaczenie stringow w jedea nazwe pliku

    FILE* f = fopen(fileName.c_str(), "w");
    // walidacja pliku
    if(f == nullptr){
        return false; 
    }
    fprintf(f, "--- RAPORT TRANSPORTOWY NR %d ---\n", transportIdx);
    fprintf(f, "SEKCJA: %d\n\n", secIdx + 1);
    // standard C99 fprintf czyli formatowanie bufforowego do pliku
    
    for(int i = 0; i < 6; i++){
        // znowu fprintf oraz bezpieczne wpisanie 64bitowej liczby, a RPIu64 czyli std C99 pozwala na bezpieczne wpisanie int64
        Product* p = s->slots[i];
        if(p != nullptr){
            fprintf(f, "Slot %d: [UPC: %" PRIu64 "] Typ: %s\n", i, p->upcRaw, p->getTypeName());
        }
    }
    fclose(f);

    transportIdx++;
    SaveFileText(idxPath, (char*)std::to_string(transportIdx).c_str());
    // tego nie trzeba tlumaczyc

    s->clear();
    return true;
}

void zapiszMagazyn(){
    /* Sama funkcja mowi za siebie, jest to zapis do magazynu do pliku magazyn.txt
    zapisujemy wszystkie sekcje
    */
    std::string dane = "";
    for(int s = 0; s < 6; s++){
        for(int i = 0; i < 6; i++){
            if(g_magazyn.sections[s].slots[i] != nullptr){
                // prosta walidacja
                dane += std::to_string(s) + "|" + std::to_string(i) + "|" + std::to_string(g_magazyn.sections[s].slots[i]->upcRaw) + "\n";
            }
        }
    }
    SaveFileText("Database/magazyn.txt", (char*)dane.c_str());
}

void wczytajMagazyn(){
    /* Analogicznie do funkcji wyzej, tutaj wczytujemy z pliku do pamieci ram
    */
    if(!FileExists("Database/magazyn.txt")){
        // prosta walidacja
        return;
    }
    char* plikTekst = LoadFileText("Database/magazyn.txt");
    if(plikTekst == nullptr){
        // prosta walidacja
        return;
    }
    std::string tresc = plikTekst;
    size_t start = 0;
    size_t end = tresc.find("\n");
    // szukamy znaku konca linii NULL

    while(end != std::string::npos){
        // petla wczytujaca dane, reczny parser tekstu, po prostu dzieli jeden wielki ciag znakow na pojedyncze linie i wyciaga z nich liczby
        std::string linia = tresc.substr(start, end - start);
        if(linia.length() > 0){
            int s, i;
            unsigned long long upc;
            if(sscanf(linia.c_str(), "%d|%d|%llu", &s, &i, &upc) == 3){
                g_magazyn.addByUPCAtSlot(s, i, upc);
            }
        }
        start = end + 1;
        end = tresc.find("\n", start);
    }
    UnloadFileText(plikTekst);
}

void generujRaportMagazynowy(){
    /* funkcja generujaca dokument txt przedstawiajacy aktualny stan magazynu
    */
    FILE* f = fopen("Database/raport_stanu.txt", "w");
    if(!f){
        //prosta walidacja
        return;
    }
    fprintf(f, "--- AKTUALNY STAN MAGAZYNU ---\n");
    for(int s = 0; s < 6; s++){
        // petla zagniezdzona 6 x 6 czyli 6 sekcji po 6 slotow
        fprintf(f, "\nSEKCJA %d (%d/6):\n", s + 1, g_magazyn.sections[s].count);
        for(int i = 0; i < 6; i++){
            Product* p = g_magazyn.sections[s].slots[i];
            if(p){
                // waruinek czy slot jest zajety, jesli tak to wpisuje szczegoly jesli nie zostawia jako pusty
                fprintf(f, "  Slot %d: [UPC: %" PRIu64 "] Typ: %s | ID: %u\n", i, p->upcRaw, p->getTypeName(), p->decoded.id);
            } else {
                fprintf(f, "  Slot %d: PUSTY\n", i);
            }
        }
    }
    fclose(f);
}