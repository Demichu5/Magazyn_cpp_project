#ifndef RENDER_HANDLER_H
#define RENDER_HANDLER_H

#include <cstdint>

void drawMainWindow();
void popWindowLoginScreen(uint8_t *user, bool *state);
void popWindowProductSection(int prodNumber, bool *state);

void zapiszMagazyn();
void wczytajMagazyn();
bool eksportujTransport(int secIdx);

#endif