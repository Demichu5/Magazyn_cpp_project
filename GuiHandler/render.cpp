#include "../Raylib/include/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "../Raylib/raygui.h"
#include "renderHandler.h"
#include "../Settings/settings.h"
#include "../Magazyn/magazynHandler.h"

Magazyn g_magazyn;
uint8_t activeUser = SM_None;
bool guiButtonLogin = false;
bool sectionWindow = false;
int sectionSwitch = 0;
bool guiProductPopUp = false;
int slotSwitch = 0;
char searchBuffer[8] = {0};
bool searchActive = false;
bool showTransportConfirm = false;

/*
Jest to najwazniejszy plik bo to on renderuje nam obraz i to co powinno sie wyswietlac a co nie :) nie ruszac - MB
*/

void drawUpperState(){
    /* drawUpperState rysuje nam gorny pasek, czyli przyciski, login etc.
    */
    DrawRectangle(0, 0, G_SCREEN_WIDTH, 64, GetColor(C_COLOR_GRAY2));
    
    const char* userLabel = "NONE";
    if(activeUser == SM_User){
        userLabel = "OPERATOR";
    }
    if(activeUser == SM_Admin){
        userLabel = "ADMIN";
    }
    DrawText(TextFormat("Rola: %s", userLabel), G_SCREEN_WIDTH - 320, 22, 20, DARKGRAY);
    // tutaj jest rysowany aktywny user oraz w sumie brama bezpieczenstwa

    GuiToggle({G_SCREEN_WIDTH - 100, 2, 96, 60}, "LOGIN", &guiButtonLogin);
    // rysowanie guzika login

    if(activeUser >= SM_User) {
        if(GuiButton({2, 2, 96, 60}, "GLOWNY")){
            sectionWindow = false;
        }
        if(GuiButton({100, 2, 96, 60}, "SEKCJE")){
            sectionWindow = true;
            if(sectionSwitch == 0){
                sectionSwitch = 1;
            }
        }
        
        if(GuiButton({198, 2, 96, 60}, "RAPORT")){
            extern void generujRaportMagazynowy();
            generujRaportMagazynowy();
        }
    }
    // tutaj jest tez rysowanie przyciskow ale zalezne od tego jaki jest aktywnyUser


    if(activeUser == SM_Admin){
        if(GuiButton({296, 2, 96, 60}, "TRANSPORT")){
            if(sectionSwitch > 0 && g_magazyn.sections[sectionSwitch-1].count > 0){
                showTransportConfirm = true; 
            }
        }
    }
    // jesli tutaj jest typowo Admin to jeszcze dodatkowo rysujemy guzik transport

    if(activeUser >= SM_User){
        DrawText("SZUKAJ ID:", 420, 22, 20, DARKGRAY);
        if(GuiTextBox({540, 12, 120, 40}, searchBuffer, 7, searchActive)){
            searchActive = !searchActive;
        }
    }
    // a tutaj jesli user to Operator/User albo Admin to jeszcze mamy opcje wyszukania przedmiotu po ID
}

void drawMiddleWindow(int typeScene){
    // jest to scena srodkowa, to co jest dynamiczne, powinno wszystko dzialac
    if(typeScene < 1 || typeScene > 6){
        // prosta walidacja
        return;
    }
    Section* s = &g_magazyn.sections[typeScene - 1];

    bool lockButtons = guiProductPopUp || guiButtonLogin || showTransportConfirm;
    // zmienna blokujaca - Lock mechanism, blokuje ona przyciski pod spodem

    if(lockButtons){
        GuiDisable();
    }

    const int startX = 250;
    const int startY = 200;
    const int spacing = 20;
    const int btnW = 250;
    const int btnH = 150;

    for(int i = 0; i < 6; i++){
        int col = i % 3;
        int row = i / 3;

        Rectangle r = { 
            (float)startX + (col * (btnW + spacing)), 
            (float)startY + (row * (btnH + spacing)), 
            (float)btnW, 
            (float)btnH 
        };

        Product* p = s->slots[i];

        if(p != nullptr){
            unsigned int typeColor = C_COLOR_TYPE_SOLID;
            if(p->decoded.type == 2){
                typeColor = C_COLOR_TYPE_LIQUID;
            }
            if (p->decoded.type == 3){
                typeColor = C_COLOR_TYPE_FRAGILE;
            }
            if(TextLength(searchBuffer) > 0){
                if(p->decoded.id == (uint32_t)atoi(searchBuffer)){
                    typeColor = C_COLOR_HIGHLIGHT; 
                }
            }
            // a i jeszcze tutaj podsiwetla sie przedmiot o wyszukiwanym ID

            DrawRectangleRec({r.x - 4, r.y - 4, r.width + 8, r.height + 8}, GetColor(typeColor));

            if(GuiButton(r, TextFormat("ID: %u\n\n%s", p->decoded.id, p->getTypeName()))){
                if(activeUser == SM_Admin && !lockButtons){
                    s->remove(i);
                }
            }
        } else {
            if(GuiButton(r, TextFormat("#%d\nPUSTY\n[ + ]", i + 1))){
                if(!lockButtons){
                    sectionSwitch = typeScene;
                    slotSwitch = i;
                    guiProductPopUp = true;
                }
            }
        }
    }
    // ten for i zmienne wyzej to sa x,y przyciskow dla przedmiotow w sekcji
    // Edit: Teraz wyswietlane sa na srodku i sa wieksze
    
    if(lockButtons){
        // walidacja po zakonczeniu
        GuiEnable();
    }
    if(guiProductPopUp){
        popWindowProductSection(typeScene, &guiProductPopUp);
    }
}

void DrawSectionProgressBar(int x, int y, int width, int height, int count){
    // funkcja ktora rysuje nam progress bar dla zapelniania sekcji, zielony, czerwony ze juz prawie zapelniony
    float fill = (float)count / 6.0f;
    
    DrawRectangle(x, y, width, height, GetColor(C_COLOR_GRAY3));
    
    unsigned int barColorHex = C_COLOR_GREEN1;
    if(count >= 5){
        barColorHex = C_COLOR_RED1;
    }

    DrawRectangle(x, y, (int)(width * fill), height, GetColor(barColorHex));
    DrawRectangleLines(x, y, width, height, BLACK);
}

void drawDownState(){
    // ta funkcja rysuje dolny pasek, czyli przysiki sekcji, progress bar etc.
    if(sectionWindow == true){
        DrawRectangle(0, G_SCREEN_HEIGHT - 80, G_SCREEN_WIDTH, 80, GetColor(C_COLOR_GRAY2));
        
        for(int i = 0; i < 6; i++){
            int xPos = 2 + 98 * i;
            
            if(GuiButton({ (float)xPos, (float)G_SCREEN_HEIGHT - 64 + 2, 96, 60 }, TextFormat("%d", i + 1))){
                sectionSwitch = i + 1;
            }
            
            int currentCount = g_magazyn.sections[i].count;
            DrawSectionProgressBar(xPos, G_SCREEN_HEIGHT - 75, 96, 10, currentCount);
        }
    }
}

void drawMainWindow(){
    // ta funkcja rysuje nam glowny ekran czyli laczy wszystkie funkcje powyzej
    if(sectionWindow){
        drawMiddleWindow(sectionSwitch);
        drawDownState();
    } else {
        // jesli jestesmy w ekranie glownym to podliczamy co mamy aktualnie
        int totalItems = 0;
        int typeSolid = 0, typeLiquid = 0, typeFragile = 0;

        for(int s = 0; s < 6; s++){
            totalItems += g_magazyn.sections[s].count;
            for(int i = 0; i < 6; i++){
                Product* p = g_magazyn.sections[s].slots[i];
                if (p) {
                    if(p->decoded.type == 10){
                        typeSolid++;
                    } 
                    else if(p->decoded.type == 20){
                        typeLiquid++; 
                    }
                    else if(p->decoded.type == 30){
                        typeFragile++;
                    }
                }
            }
        }

        DrawRectangle(200, 150, 880, 400, GetColor(C_COLOR_WHITE1));
        DrawRectangleLines(200, 150, 880, 400, DARKGRAY);
        
        DrawText("PODSUMOWANIE MAGAZYNU", 220, 170, 30, BLACK);
        DrawText(TextFormat("Wszystkich produktow: %d / 36", totalItems), 250, 230, 25, DARKGRAY);
        DrawText(TextFormat("- Produkty stale: %d", typeSolid), 280, 270, 20, BLACK);
        DrawText(TextFormat("- Produkty ciekle: %d", typeLiquid), 280, 300, 20, BLUE);
        DrawText(TextFormat("- Produkty wrazliwe: %d", typeFragile), 280, 330, 20, ORANGE);

        float globalFill = (float)totalItems / 6.0f;
        DrawSectionProgressBar(250, 450, 780, 40, (int)globalFill); 

        DrawText(TextFormat("Calkowite zapelnienie: %d%%", (totalItems * 100) / 36), 520, 460, 20, BLACK);
        // a tutaj rysujemy progress bary oraz tekst i statystyki
    }

    drawUpperState();
    if(guiButtonLogin){
        // no jesli nacisniecie przycisk login to pokaze wam sie okno pop-up z loginem i zablokuje reszte - Lock mechanism
        popWindowLoginScreen(&activeUser, &guiButtonLogin);
    }
    if(showTransportConfirm){
        int res = GuiMessageBox({ (float)G_SCREEN_WIDTH/2 - 125, (float)G_SCREEN_HEIGHT/2 - 50, 250, 100 }, 
                  "POTWIERDZENIE", "Wyslac transport i wyczyscic sekcje?", "TAK;ANULUJ");
        
        if(res == 1){
            extern bool eksportujTransport(int idx);
            eksportujTransport(sectionSwitch - 1);
            showTransportConfirm = false;
        } 
        else if(res == 2 || res == 0){
            showTransportConfirm = false;
        }
    }
    //Edit: a tutaj jest zatwierdzenie aby wyslac transport, wczesniej nie trzeba bylo i latwo o wyslanie wiec naprawilem
}