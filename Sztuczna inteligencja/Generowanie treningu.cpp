#include <iostream>
#include <stdio.h>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <graphics.h>
#include <libpq-fe.h>

using namespace std;

int wys = getmaxheight() * 0.94;
int szer = wys * 4/3;
const int kolor = COLOR(0,255,0);
const int niebieski = COLOR(255,255,255);
const float promien_biali = 1.0 * wys / 902.4;
const float promien_gracz = 20.0 * wys / 902.4;
const float promien_niebiescy = promien_gracz;
const int tab_wsk_biali = 2048;
const int tab_wsk_niebiescy = 50;
const float zwiekszanie_predkosci = 1.0;
int ilosc_bialych = 0;
int ilosc_niebieskich = 0;
float pole_do_rozdzialu = wys * szer / 2; //nalezy unikać pełnego pola obszaru do gry
float poczatkowe_pole = pole_do_rozdzialu;

struct punkt
{
    float x;
    float y;
};

struct do_zakresu_projektu //struktura zrobiona po to, żeby stworzyć potrzebną w zakresie projektu tablicę struktur
{
    int x;
    int y;
};

const punkt srodek = {szer/2,wys/2};

struct wektor
{
    float x;
    float y;
};

class obiekt //każdy obiekt występujący w grze
{
public:
    float promien;
    punkt wspolrzedne;
    wektor kierunek;
    float pole;
    int rodzaj;
    obiekt(int kto);
};

class obiekt_niebieski: public obiekt //klasa przeciwnika dziedzicząca właściwości klasy "obiekt"
{
public:
    time_t wystrzelenie;
    obiekt *cel;
    obiekt_niebieski():obiekt(2) { //konstruktor klasy
        wystrzelenie = 0;
        cel = NULL;
    }
};

obiekt::obiekt(int kto) { //konstruktor klasy "obiekt"
    if (kto == 1) {
        promien = promien_gracz;
        wspolrzedne = srodek;
        kierunek = {0,0};
        pole = M_PI * promien * promien;
        pole_do_rozdzialu -= pole;
        rodzaj = 1;
    }
    else  if (kto == 0) {
        rodzaj = 0;
    }
    else if (kto == 2) {
        rodzaj = 2;
        promien = promien_niebiescy;
        kierunek = {0,0};
        pole = M_PI * promien * promien;
    }
}

obiekt *biali[tab_wsk_biali]; //tablica wskaźników zawierająca adresy białych kółek
obiekt_niebieski *niebiescy[tab_wsk_niebiescy]; //tablica wskaźników zawierająca adresy przeciwnika

int main() {
    srand(time(NULL));
    float odleglosc_przed, odleglosc_po;
    float sinus, cosinus, kat, kat1, kat2;
    float wejscieobiekt_x, wejscieobiekt_y, wejsciecel_x, wejsciecel_y;
    float wyjscie_x, wyjscie_y;
    float wiekszy, kierunek_ruchu;
    float bufor_x, bufor_y;
    obiekt_niebieski *wskaznik_obiekt;
    obiekt *wskaznik_cel;
    FILE *status;
    status = fopen("AI.dat", "w");
    fprintf(status, "5000 6 2\n");
    for(int i = 1; i <= 5000; i++) {
        wskaznik_obiekt = new obiekt_niebieski();
        wskaznik_obiekt -> promien = rand()%200;
        wskaznik_obiekt -> pole = M_PI * wskaznik_obiekt -> promien * wskaznik_obiekt -> promien;
        wskaznik_obiekt -> wspolrzedne.x = (rand()%int(szer - 2 * (0.05 * szer + wskaznik_obiekt -> promien + 1))) + int(szer * 0.05 + wskaznik_obiekt -> promien);
        wskaznik_obiekt -> wspolrzedne.y = (rand()%int(wys - 2 * (0.05 * wys + wskaznik_obiekt -> promien + 1))) + int(wys * 0.05 + wskaznik_obiekt -> promien);
        wskaznik_obiekt -> kierunek.x = rand() / (float(RAND_MAX)+1)*500;
        wskaznik_obiekt -> kierunek.y = rand() / (float(RAND_MAX)+1)*500;
        wskaznik_cel = new obiekt(0);
        wskaznik_cel -> promien = rand()%200;
        wskaznik_cel -> pole = M_PI * wskaznik_cel -> promien * wskaznik_cel -> promien;
        wskaznik_cel -> wspolrzedne.x = (rand()%int(szer - 2 * (0.05 * szer + wskaznik_cel -> promien + 1))) + int(szer * 0.05 + wskaznik_cel -> promien);
        wskaznik_cel -> wspolrzedne.y = (rand()%int(wys - 2 * (0.05 * wys + wskaznik_cel -> promien + 1))) + int(wys * 0.05 + wskaznik_cel -> promien);
        wskaznik_cel -> kierunek.x = rand() / (float(RAND_MAX)+1)*500;
        wskaznik_cel -> kierunek.y = rand() / (float(RAND_MAX)+1)*500;
        odleglosc_przed = sqrt(pow(wskaznik_obiekt -> wspolrzedne.x - wskaznik_cel -> wspolrzedne.x, 2) + pow(wskaznik_obiekt -> wspolrzedne.y - wskaznik_cel -> wspolrzedne.y,2));
        odleglosc_po = sqrt(pow((wskaznik_obiekt -> wspolrzedne.x + wskaznik_obiekt -> kierunek.x) - (wskaznik_cel -> wspolrzedne.x + wskaznik_cel -> kierunek.x),2) + pow((wskaznik_obiekt -> wspolrzedne.x + wskaznik_obiekt -> kierunek.x) - (wskaznik_cel -> wspolrzedne.x + wskaznik_cel -> kierunek.x),2));
        if ( i % 500 == 0) {
            wys = (rand()%3000+1000)/2;
            szer = rand()%3000+1000;
        }
        sinus = float(sqrt(pow((wskaznik_cel->wspolrzedne.y+wskaznik_cel->kierunek.y) - wskaznik_obiekt->wspolrzedne.y, 2))/sqrt(pow((wskaznik_cel->wspolrzedne.x+wskaznik_cel->kierunek.x) - wskaznik_obiekt->wspolrzedne.x, 2)+pow((wskaznik_cel->wspolrzedne.y+wskaznik_cel->kierunek.y) - wskaznik_obiekt->wspolrzedne.y, 2)));
        cosinus = float(sqrt(pow((wskaznik_cel->wspolrzedne.x+wskaznik_cel->kierunek.x) - wskaznik_obiekt->wspolrzedne.x, 2))/sqrt(pow((wskaznik_cel->wspolrzedne.x+wskaznik_cel->kierunek.x) - wskaznik_obiekt->wspolrzedne.x, 2)+pow((wskaznik_cel->wspolrzedne.y+wskaznik_cel->kierunek.y) - wskaznik_obiekt->wspolrzedne.y, 2)));
        if (wskaznik_cel -> wspolrzedne.x + wskaznik_cel -> kierunek.x >= wskaznik_obiekt -> wspolrzedne.x) {
            if (wskaznik_cel -> wspolrzedne.y + wskaznik_cel -> kierunek.y >= wskaznik_obiekt -> wspolrzedne.y) {
                kat1 = asinf(sinus);
                kat2 = acosf(cosinus);
                if (kat1 != 0.0)
                    kat = kat1;
                else
                    kat = kat2;
            }
            else {
                kat1 = 2 * M_PI - asinf(sinus);
                kat2 = 2 * M_PI - acosf(cosinus);
                if (kat1 != 0.0)
                    kat = kat1;
                else
                    kat = kat2;
            }
        }
        else {
            if (wskaznik_cel -> wspolrzedne.y + wskaznik_cel -> kierunek.y >= wskaznik_obiekt -> wspolrzedne.y) {
                kat1 = M_PI - asinf(sinus);
                kat2 = M_PI - asinf(cosinus);
                if (kat1 != 0.0)
                    kat = kat1;
                else
                    kat = kat2;
            }
            else {
                kat1 = M_PI + asinf(sinus);
                kat2 = M_PI + acosf(cosinus);
                if (kat1 != 0.0)
                    kat = kat1;
                else
                    kat = kat2;
            }
        }
        if (wskaznik_obiekt -> pole * 0.9 <= wskaznik_cel -> pole) {
            wiekszy = 1.0;
        }
        else {
            wiekszy = -1.0;
        }
        if (odleglosc_przed < odleglosc_po) { //ucieka
            kierunek_ruchu = 1.0;
        }
        else if (odleglosc_przed == odleglosc_po) { //stoi
            kierunek_ruchu = 0.0;
        }
        else { //zbliza sie
            kierunek_ruchu = -1.0;
        }
        wejscieobiekt_x = (wskaznik_obiekt -> wspolrzedne.x - (szer / 2)) / (szer/2);
        wejscieobiekt_y = (wskaznik_obiekt -> wspolrzedne.y - (wys / 2)) / (wys/2);
        wejsciecel_x = ((wskaznik_cel -> wspolrzedne.x + wskaznik_cel -> kierunek.x) - (szer / 2)) / (szer/2);
        wejsciecel_y = ((wskaznik_cel -> wspolrzedne.y + wskaznik_cel -> kierunek.x) - (wys / 2)) / (wys/2);
        if (wiekszy == -1.0 && (kierunek_ruchu == 1.0 || kierunek_ruchu == 0.0)) {
            wyjscie_x = 2 * wejscieobiekt_x - wejsciecel_x;
            wyjscie_y = 2 * wejscieobiekt_y - wejsciecel_y;
        }
        else if (wiekszy == -1.0 && kierunek_ruchu == -1.0) {
            wyjscie_x = 0.0;
            wyjscie_y = 0.0;
        }
        else if (wiekszy == 1.0 && kierunek_ruchu == 1.0) {
            wyjscie_x = 0.0;
            wyjscie_y = 0.0;
        }
        else {
            wyjscie_x = wejsciecel_x;
            wyjscie_y = wejsciecel_y;
        }
        if (wyjscie_x > 1.0 || wyjscie_x < -1.0 || wyjscie_y > 1.0 || wyjscie_y < -1.0) {
            while(wyjscie_x > 1.0 || wyjscie_x < -1.0 || wyjscie_y > 1.0 || wyjscie_y < -1.0) {
                wyjscie_x = 0.5 * wejscieobiekt_x + 0.5 * wyjscie_x;
                cout << wejscieobiekt_x << " " << wyjscie_x << endl;
                wyjscie_y = 0.5 * wejscieobiekt_y + 0.5 * wyjscie_y;
                cout << wejscieobiekt_y << " " << wyjscie_y << endl;
            }
        }
        fprintf(status, "%f %f %f %f %f %f\n", wejscieobiekt_x, wejscieobiekt_y, wejsciecel_x, wejsciecel_y, kierunek_ruchu, wiekszy);
        fprintf(status, "%f %f\n", wyjscie_x, wyjscie_y);
        delete wskaznik_cel;
        delete wskaznik_obiekt;
    }
    fclose(status);
}
