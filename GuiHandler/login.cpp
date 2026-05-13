#include "../Raylib/include/raylib.h"
#include "../Raylib/raygui.h"
#include "../Settings/settings.h"
#include <cstdlib>
#include <cstdint>

char username[9] = {0};

void popWindowLoginScreen(uint8_t *user, bool *state){
    /* popWindowLoginScreen() to funkcja ktora autoryzuje uzytkownika w trybie okienkowym
    Pobiera od uzytkownika kodu INT i przypisanie odpowienich uprawnien w systemie
    0 - None
    10 - User
    100 - Admin */
    Rectangle box = { (float)G_SCREEN_WIDTH/3, (float)G_SCREEN_HEIGHT/3, 300, 200 };
    int res = GuiTextInputBox(box, "LOGIN", "Wprowadz kod (0, 10, 100)", "OK;ANULUJ", username, 8, NULL);
    /* Tutaj jest rysowanie okienka pop-up oraz przypisanie wartosci return do zmiennej res
    1 = OK
    2 = ANULUJ
    inna niż pozostale zamyka okno pop-up */
    if(res == 1){
        int val = atoi(username);
        if(val == 100){
            *user = SM_Admin;
        }
        else if(val == 10){
            *user = SM_User;
        } else {
            *user = SM_None;
        }
        *state = false;
    } 
    else if(res >= 2){
        *state = false;
    }
}