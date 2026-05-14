#!/bin/bash

# Skrypt instalujący zależności dla projektu Magazyn_cpp (Raylib + X11)
# Przeznaczony dla systemów bazujących na Debian/Ubuntu/WSL

echo "--- Rozpoczynanie instalacji zależności systemowych ---"

# Aktualizacja listy pakietów
sudo apt update

# Instalacja niezbędnych bibliotek deweloperskich
sudo apt install -y \
    build-essential \
    libx11-dev \
    libxcursor-dev \
    libxinerama-dev \
    libxrandr-dev \
    libxi-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libxkbcommon-dev

echo "--- Instalacja zakończona sukcesem! ---"
echo "Teraz możesz skompilować projekt wpisując: make"
