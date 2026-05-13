# SYSTEM ZARZĄDZANIA MAGAZYNEM (WMS)

## 1. Autorzy Projektu

- **MB** – Architekt systemowy  
- **FB** – Logika magazynowa  
- **AD** – Tester systemowy  
- **PK** – Walidacja danych oraz ich standardy

--- 

## 2. Opis zrealizowanego projektu

Projekt obejmuje autorską aplikację typu **Warehouse Management System (WMS)** napisaną w języku **C++20** przy użyciu biblioteki graficznej **Raylib**.  
System pozwala na ewidencję towarów w czasie rzeczywistym w ramach sześciu niezależnych sekcji magazynowych.

### Kluczowe cechy techniczne

#### Standard UPC-12
System wykorzystuje realny standard kodów kreskowych do automatycznego identyfikowania:
- ID produktu  
- typu (stały, ciekły, wrażliwy)  
- parametrów  

#### Polimorfizm
Zastosowano dziedziczenie klas produktów, co pozwala na dynamiczne zarządzanie różnymi typami towarów.

#### Trwałość danych
System posiada własną mini-bazę danych opartą na plikach tekstowych oraz generator raportów transportowych.

#### Bezpieczeństwo
Zaimplementowano system ról:
- **GUEST / NONE**
- **OPERATOR / USER**
- **ADMIN**

System steruje dostępem do krytycznych funkcji aplikacji.

---

## 3. Instrukcja użytkownika

### Uruchomienie

```bash
make
./main
```

### Logowanie:
- Kliknij przycisk LOGIN
- Wprowadź kod:
  - 10 → Operator/User
  - 100 → Administrator
- (Emulacja czytnika RFID)

### Nawigacja:
- Kliknij SEKCJE
- Wybierz numer sekcji (1–6) z dolnego panelu

### Zarządzanie towarami:
- Kliknij PUSTY [+], aby dodać produkt
  - wymagany 12-cyfrowy kod
- Administrator:
  - kliknięcie zajętego slotu usuwa produkt

### Wyszukiwanie:
- Wpisz ID produktu w polu SZUKAJ ID
- Produkt zostaje podświetlony w magazynie

### Raportowanie:
- Kliknij RAPORT
- Generowany jest spis stanu magazynu w folderze Database/

### Transport:
- Kliknij TRANSPORT (Administrator)
- Po potwierdzeniu:
  - generowany jest list przewozowy
  - wybrana sekcja zostaje opróżniona

---

## 4. Ciekawostki związane z realizacją

## Emulacja RFID:
System logowania został zaprojektowany tak, aby w przyszłości można było go łatwo zintegrować z fizycznymi czytnikami kart zbliżeniowych. Zastosowano kody numeryczne zamiast haseł tekstowych.

## Walka z "Input Bleeding":
Jednym z ciekawszych wyzwań było rozwiązanie problemu przenikania kliknięć przez okna pop-up. Rozwiązano to za pomocą autorskiego mechanizmu Modal Lock, który czasowo zamraża warstwę pod spodem.

## Ewolucja interfejsu:
Projekt zaczął się od prostej listy tekstowej, a ewoluował w stronę nowoczesnego interfejsu kafelkowego 

---

### 5. Podsumowanie i wnioski

## Projekt został ukończony zgodnie z założeniami wstępnymi. Udało się zrealizować stabilny system, który nie tylko przechowuje dane, ale również je waliduje i wizualizuje.

## Wnioski:

- Zastosowanie statycznej puli obiektów znacząco podniosło stabilność aplikacji  
  - brak błędów typu segmentation fault przy intensywnym dodawaniu produktów  

- Biblioteka Raylib okazała się wydajnym narzędziem do tworzenia szybkich interfejsów typu narzędziowego  
  - mimo początkowych trudności z obsługą zdarzeń modalnych

---

### 6. Możliwości rozbudowy projektu

## System został zaprojektowany w sposób modułowy, co otwiera drogę do dalszego rozwoju:

## Integracja SQL:
- Zamiana plików tekstowych na bazę SQLite dla lepszej wydajności przy tysiącach produktów

## Obsługa skanera:
- Dodanie modułu OpenCV do skanowania kodów UPC bezpośrednio z kamery

## Moduł sieciowy:
- Umożliwienie pracy wielu magazynierom na jednej bazie danych poprzez protokół TCP/IP

Wizualizacja 3D:
- Rozbudowa widoku sekcji o model 3D regałów magazynowych
