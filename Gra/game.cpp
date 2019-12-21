#include <iostream>
#include <graphics.h>
#include <stdio.h>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include "libpq-fe.h"
#include "floatfann.h"

using namespace std;
//w kodzie: Inteligentny przeciwnik -> niebieski; zwykly obiekt -> bialy
const int wys = getmaxheight() * 0.94;
const int szer = wys * 4/3;
const int kolor = COLOR(0,255,0);
const int niebieski = COLOR(255,255,255); //od kiedy dodalem zmienianie sie kolorow w trakcie gry to barwy nie maja za wiele wspolnego z nazwami zmiennych
const float promien_biali = 1.0 * wys / 902.4;
const float promien_gracz = 20.0 * wys / 902.4;
const float promien_niebiescy = promien_gracz;
const int tab_wsk_biali = 100000;
const int tab_wsk_niebiescy = 50;
const float zwiekszanie_predkosci = 1.0;
int ilosc_bialych = 0;
int ilosc_niebieskich = 0;
float pole_do_rozdzialu = wys * szer / 3 * 2; //nalezy unikać pełnego pola obszaru do gry
float poczatkowe_pole = pole_do_rozdzialu;
wys = 80;
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
    unsigned long int wystrzelenie;
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
        kierunek = {0.0, 0.0};
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
        kierunek = {0.0, 0.0};
        pole = M_PI * promien * promien;
    }
}

obiekt *biali[tab_wsk_biali]; //tablica wskaźników zawierająca adresy białych kółek
obiekt_niebieski *niebiescy[tab_wsk_niebiescy]; //tablica wskaźników zawierająca adresy przeciwnika
obiekt gracz(1);

void czyszczenie_bufora() { //funkcja sluzaca do czyszczenia bufora klawiatury
    while (kbhit()) {
        getch();
    }
}

void brak_pamieci(obiekt *wskaznik) {
    if (wskaznik == NULL) {
        int wylaczanie[12] {
        srodek.x - 300, srodek.y,
        srodek.x - 150, srodek.y - 150,
        srodek.x + 150, srodek.y - 150,
        srodek.x + 300, srodek.y,
        srodek.x + 150, srodek.y + 150,
        srodek.x - 150, srodek.y + 150 };
        int myszkax, myszkay;
        bool kontynuuj = false;
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, RED);
        fillpoly(6, wylaczanie);
        setcolor(WHITE);
        outtextxy(srodek.x - 55, srodek.y - 125,"JAMES BLAD 404");
        readimagefile("err.is", srodek.x - 150, srodek.y - 100, srodek.x + 150, srodek.y + 25);
        outtextxy(srodek.x - 100, srodek.y + 35,"BRAK PAMIECI = Koniec gry.");
        setcolor(BLACK);
        rectangle(srodek.x - 45, srodek.y + 75, srodek.x + 45, srodek.y + 125);
        setfillstyle(SOLID_FILL, BLACK);
        floodfill(srodek.x - 44, srodek.y + 76, BLACK);
        setcolor(WHITE);
        outtextxy(srodek.x - 29, srodek.y + 95, "Zakoncz");
        do {
            czyszczenie_bufora();
            if (ismouseclick(WM_LBUTTONDOWN)) {
                getmouseclick(WM_LBUTTONDOWN, myszkax, myszkay);
                if (myszkax >= srodek.x - 45 && myszkax <= srodek.x + 50 && myszkay >= srodek.y + 75 && myszkay <= srodek.y + 125) {
                    kontynuuj = true; }
        }} while (kontynuuj == false);
        exit(-1);
    }
}

void brak_pamieci(obiekt_niebieski *wskaznik) { //funkcja informujaca o braku dostepnej pamieci w systemie
    if (wskaznik == NULL) {
        int wylaczanie[12] {
        srodek.x - 300, srodek.y,
        srodek.x - 150, srodek.y - 150,
        srodek.x + 150, srodek.y - 150,
        srodek.x + 300, srodek.y,
        srodek.x + 150, srodek.y + 150,
        srodek.x - 150, srodek.y + 150 };
        int myszkax, myszkay;
        bool kontynuuj = false;
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, RED);
        fillpoly(6, wylaczanie);
        setcolor(WHITE);
        outtextxy(srodek.x - 55, srodek.y - 125,"JAMES BLAD 404");
        readimagefile("err.is", srodek.x - 150, srodek.y - 100, srodek.x + 150, srodek.y + 25);
        outtextxy(srodek.x - 100, srodek.y + 35,"BRAK PAMIECI = Koniec gry.");
        setcolor(BLACK);
        rectangle(srodek.x - 45, srodek.y + 75, srodek.x + 45, srodek.y + 125);
        setfillstyle(SOLID_FILL, BLACK);
        floodfill(srodek.x - 44, srodek.y + 76, BLACK);
        setcolor(WHITE);
        outtextxy(srodek.x - 29, srodek.y + 95, "Zakoncz");
        do {
            czyszczenie_bufora();
            if (ismouseclick(WM_LBUTTONDOWN)) {
                getmouseclick(WM_LBUTTONDOWN, myszkax, myszkay);
                if (myszkax >= srodek.x - 45 && myszkax <= srodek.x + 50 && myszkay >= srodek.y + 75 && myszkay <= srodek.y + 125) {
                    kontynuuj = true; }
        }} while (kontynuuj == false);
        exit(-1);
    }
}

void odbicie(obiekt *dofunkcji) { //funkcja odpowiedzialna za odbijanie się obiektów od krawędzi planszy
    if ((((dofunkcji -> wspolrzedne.y) <= (0.05 * wys + dofunkcji -> promien - dofunkcji -> kierunek.y)) && ((dofunkcji -> wspolrzedne.x) <= (0.05 * szer + dofunkcji -> promien - dofunkcji -> kierunek.x))) || (((dofunkcji -> wspolrzedne.y) >= (wys - 0.05 * wys - (dofunkcji -> promien) - dofunkcji -> kierunek.y)) && ((dofunkcji -> wspolrzedne.x) >= (szer - 0.05 * szer - (dofunkcji -> promien) - dofunkcji -> kierunek.x)))) {
        dofunkcji -> kierunek.y *= -1;
        dofunkcji -> kierunek.x *= -1; }
    else if ((dofunkcji -> wspolrzedne.x) <= (0.05 * szer + dofunkcji -> promien - dofunkcji -> kierunek.x + 1) || (dofunkcji -> wspolrzedne.x) >= (szer - 0.05 * szer - (dofunkcji -> promien) - dofunkcji -> kierunek.x - 1))
        dofunkcji -> kierunek.x *= -1;
    else if ((dofunkcji -> wspolrzedne.y) <= (0.05 * wys + dofunkcji -> promien - dofunkcji -> kierunek.y + 1) || (dofunkcji -> wspolrzedne.y) >= (wys - 0.05 * wys - (dofunkcji -> promien) - dofunkcji -> kierunek.y - 1))
        dofunkcji -> kierunek.y *= -1;
}

void rysowanie(obiekt *dofunkcji) { //funkcja odpowiedzialna za narysowanie przekazanego obiektu
    if (dofunkcji -> rodzaj == 1) {
            setcolor(kolor);
            circle(dofunkcji -> wspolrzedne.x, dofunkcji -> wspolrzedne.y, (int)dofunkcji -> promien);
             }
    else if (dofunkcji -> rodzaj == 0) {
            int bialy;
            if (dofunkcji -> pole > gracz.pole)
                bialy = COLOR(255,64,0);
            else
                bialy = COLOR(0,64,255);
            setcolor(bialy);
            circle(dofunkcji -> wspolrzedne.x, dofunkcji -> wspolrzedne.y, (int)dofunkcji -> promien);
             }
    else if (dofunkcji -> rodzaj == 2) {
            setcolor(niebieski);
            circle(dofunkcji -> wspolrzedne.x, dofunkcji -> wspolrzedne.y, (int)dofunkcji -> promien);
             }
}

void ruch(obiekt *dofunkcji) { //funkcja generująca nowe współrzędne obiektów tworząca pozorny ruch
    dofunkcji -> wspolrzedne.x += dofunkcji -> kierunek.x;
    dofunkcji -> wspolrzedne.y += dofunkcji -> kierunek.y;
    rysowanie(dofunkcji);
}

void uruchamianie_grafiki () { //funkcja odpowiedzialna za otwarcie okna graficznego
    int errorcode;
    initwindow(szer, wys, "Kolka", (getmaxwidth() - szer) / 2, 0, false, false);
    errorcode = graphresult(); // uda³o siê?
    if (errorcode != grOk) { // problem !
        printf("Blad BGI: %s\n", grapherrormsg(errorcode));
        printf("Wcisnij dowolny klawisz, zeby zakonczyc:");
        getch();
        exit(-1); }
}

void zmiana_strony() { //funkcja generująca podwójne buforowanie w grze
    if (getactivepage() == 0) {
        setvisualpage(0);
        setactivepage(1);
        cleardevice(); }
    else {
        setvisualpage(1);
        setactivepage(0);
        cleardevice(); }
}

void walka(obiekt *obiekt1, obiekt *obiekt2) { //funkcja odpowiedzialna za "zjadanie się" kółek
    if (obiekt1 != 0 && obiekt2 != 0) {
        int szereg = 25;
        float pedx1, pedy1, pedx2, pedy2, pedszeregx, pedszeregy;
        if ((obiekt1 -> promien + obiekt2 -> promien) >= (sqrt(pow(obiekt1 -> wspolrzedne.x - obiekt2 -> wspolrzedne.x, 2) + pow(obiekt1 -> wspolrzedne.y - obiekt2 -> wspolrzedne.y, 2)))) {
            if (obiekt1 -> pole >= obiekt2 -> pole)
                do {
                    pedx1 = obiekt1 -> pole * obiekt1 -> kierunek.x;
                    pedy1 = obiekt1 -> pole * obiekt1 -> kierunek.y;
                    pedx2 = obiekt2 -> pole * obiekt2 -> kierunek.x;
                    pedy2 = obiekt2 -> pole * obiekt2 -> kierunek.y;
                    pedszeregx = szereg / obiekt2 -> pole * pedx2;
                    pedszeregy = szereg / obiekt2 -> pole * pedy2;
                    pedx1 += pedszeregx;
                    pedy1 += pedszeregy;
                    pedx2 -= pedszeregx;
                    pedy2 -= pedszeregy;
                    if (obiekt2 -> pole <= 25) {
                        obiekt1 -> pole += obiekt2 -> pole;
                        obiekt1 -> kierunek.x = pedx1 / obiekt1 -> pole;
                        obiekt1 -> kierunek.y = pedy1 / obiekt1 -> pole;
                        obiekt2 -> pole = 0.0;
                        obiekt1 -> promien = sqrtf(obiekt1 -> pole/M_PI);
                        obiekt2 -> promien = 0.0; }
                    else {
                        obiekt1 -> pole += szereg;
                        obiekt2 -> pole -= szereg;
                        obiekt1 -> kierunek.x = pedx1 / obiekt1 -> pole;
                        obiekt1 -> kierunek.y = pedy1 / obiekt1 -> pole;
                        obiekt2 -> kierunek.x = pedx2 / obiekt2 -> pole;
                        obiekt2 -> kierunek.y = pedy2 / obiekt2 -> pole;
                        obiekt1 -> promien = sqrtf(obiekt1 -> pole/M_PI);
                        obiekt2 -> promien = sqrtf(obiekt2 -> pole/M_PI);
                         }
                    if (obiekt1 -> wspolrzedne.x <= szer * 0.05 + obiekt1 -> promien + obiekt1 -> kierunek.x + 1)
                        obiekt1 -> wspolrzedne.x += (szer * 0.05 + obiekt1 -> promien - obiekt1 -> kierunek.x + 1) - obiekt1 -> wspolrzedne.x;
                    else if (obiekt1 -> wspolrzedne.x >= szer - szer * 0.05 - obiekt1 -> promien - obiekt1 -> kierunek.x - 1)
                        obiekt1 -> wspolrzedne.x -= obiekt1 -> wspolrzedne.x - (szer - szer * 0.05 - obiekt1 -> promien - obiekt1 -> kierunek.x - 1);
                    if (obiekt1 -> wspolrzedne.y <= wys * 0.05 + obiekt1 -> promien - obiekt1 -> kierunek.y + 1)
                        obiekt1 -> wspolrzedne.y += (wys * 0.05 + obiekt1 -> promien - obiekt1 -> kierunek.y + 1) - obiekt1 -> wspolrzedne.y;
                    else if (obiekt1 -> wspolrzedne.y >= wys - wys * 0.05 - obiekt1 -> promien - obiekt1 -> kierunek.y - 1)
                        obiekt1 -> wspolrzedne.y -= obiekt1 -> wspolrzedne.y - (wys - wys * 0.05 - obiekt1 -> promien - obiekt1 -> kierunek.y - 1);
                } while(((obiekt1 -> promien + obiekt2 -> promien) >= (sqrt(pow(obiekt1 -> wspolrzedne.x - obiekt2 -> wspolrzedne.x, 2) + pow(obiekt1 -> wspolrzedne.y - obiekt2 -> wspolrzedne.y, 2)))) && obiekt2 -> pole > 0.0);
            else
                do {
                    pedx1 = obiekt1 -> pole * obiekt1 -> kierunek.x;
                    pedy1 = obiekt1 -> pole * obiekt1 -> kierunek.y;
                    pedx2 = obiekt2 -> pole * obiekt2 -> kierunek.x;
                    pedy2 = obiekt2 -> pole * obiekt2 -> kierunek.y;
                    pedszeregx = szereg / obiekt1 -> pole * pedx1;
                    pedszeregy = szereg / obiekt1 -> pole * pedy1;
                    pedx2 += pedszeregx;
                    pedy2 += pedszeregy;
                    pedx1 -= pedszeregx;
                    pedy1 -= pedszeregy;
                    if (obiekt1 -> pole <= 25) {
                        obiekt2 -> pole += obiekt1 -> pole;
                        obiekt2 -> kierunek.x = pedx2 / obiekt2 -> pole;
                        obiekt2 -> kierunek.y = pedy2 / obiekt2 -> pole;
                        obiekt1 -> pole = 0.0;
                        obiekt2 -> promien = sqrtf(obiekt2 -> pole/M_PI);
                        obiekt1 -> promien = 0.0; }
                    else {
                        obiekt1 -> pole -= szereg;
                        obiekt2 -> pole += szereg;
                        obiekt2 -> kierunek.x = pedx2 / obiekt2 -> pole;
                        obiekt2 -> kierunek.y = pedy2 / obiekt2 -> pole;
                        obiekt1 -> kierunek.x = pedx1 / obiekt1 -> pole;
                        obiekt1 -> kierunek.y = pedy1 / obiekt1 -> pole;
                        obiekt2 -> promien = sqrtf(obiekt2 -> pole/M_PI);
                        obiekt1 -> promien = sqrtf(obiekt1 -> pole/M_PI); }
                    if (obiekt2 -> wspolrzedne.x <= szer * 0.05 + obiekt2 -> promien - obiekt2 -> kierunek.x + 1)
                        obiekt2 -> wspolrzedne.x += (szer * 0.05 + obiekt2 -> promien - obiekt2 -> kierunek.x + 1) - obiekt2 -> wspolrzedne.x;
                    else if (obiekt2 -> wspolrzedne.x >= szer - szer * 0.05 - obiekt2 -> promien - obiekt2 -> kierunek.x - 1)
                        obiekt2 -> wspolrzedne.x -= obiekt2 -> wspolrzedne.x - (szer - szer * 0.05 - obiekt2 -> promien - obiekt2 -> kierunek.x - 1);
                    if (obiekt2 -> wspolrzedne.y <= wys * 0.05 + obiekt2 -> promien - obiekt2 -> kierunek.y + 1)
                        obiekt2 -> wspolrzedne.y += (wys * 0.05 + obiekt2 -> promien - obiekt2 -> kierunek.y + 1) - obiekt2 -> wspolrzedne.y;
                    else if (obiekt2 -> wspolrzedne.y >= wys - wys * 0.05 - obiekt2 -> promien - obiekt2 -> kierunek.y - 1)
                        obiekt2 -> wspolrzedne.y -= obiekt2 -> wspolrzedne.y - (wys - wys * 0.05 - obiekt2 -> promien - obiekt2 -> kierunek.y - 1);
                } while(((obiekt1 -> promien + obiekt2 -> promien) >= (sqrt(pow(obiekt1 -> wspolrzedne.x - obiekt2 -> wspolrzedne.x, 2) + pow(obiekt1 -> wspolrzedne.y - obiekt2 -> wspolrzedne.y, 2)))) && obiekt1 -> pole > 0.0);
        }
        }
}

void generator_niebieskich(int ile) { //funkcja generująca niebieskie obiekty na planszy
    srand( time( NULL ) );
    int stworzeni = 0;
    int slepa_uliczka = 0;
    bool anuluj = 0;
    while (stworzeni < ile) {
        cout << "Generuje niebieskich..." << endl;
        niebiescy[ilosc_niebieskich] = new obiekt_niebieski();
        niebiescy[ilosc_niebieskich] -> promien = promien_niebiescy;
        niebiescy[ilosc_niebieskich] -> wspolrzedne.x = (rand()%int(szer - 2 * (0.05 * szer + promien_niebiescy))) + int(szer * 0.05 + promien_niebiescy + 1);
        niebiescy[ilosc_niebieskich] -> wspolrzedne.y = (rand()%int(wys - 2 * (0.05 * wys + promien_niebiescy))) + int(wys * 0.05 + promien_niebiescy + 1);
        if (ilosc_niebieskich != 0) {
            for (int i = 0; i < ilosc_niebieskich; i++) {
                if (niebiescy[ilosc_niebieskich] != niebiescy[i])
                    if (niebiescy[ilosc_niebieskich] -> promien + niebiescy[i] -> promien >= sqrt(pow(niebiescy[ilosc_niebieskich] -> wspolrzedne.x - niebiescy[i] -> wspolrzedne.x, 2) + pow(niebiescy[ilosc_niebieskich] -> wspolrzedne.y - niebiescy[i] -> wspolrzedne.y, 2))) {
                        anuluj = 1;
                    }
            }
        }
        if (anuluj == 1 || niebiescy[ilosc_niebieskich] -> promien + gracz.promien >= sqrt(pow(niebiescy[ilosc_niebieskich] -> wspolrzedne.x - gracz.wspolrzedne.x, 2) + pow(niebiescy[ilosc_niebieskich] -> wspolrzedne.y - gracz.wspolrzedne.y, 2)) ) {
            delete niebiescy[ilosc_niebieskich];
            niebiescy[ilosc_niebieskich] = NULL;
            anuluj = 0;
            slepa_uliczka++;
        }
        else {
            niebiescy[ilosc_niebieskich] -> pole = niebiescy[ilosc_niebieskich] -> promien * niebiescy[ilosc_niebieskich] -> promien * M_PI;
            pole_do_rozdzialu -= niebiescy[ilosc_niebieskich] -> pole;
            stworzeni++;
            ilosc_niebieskich++;
            slepa_uliczka = 0;
        }
        if (slepa_uliczka > 10) {
            break;
            slepa_uliczka = 0;
            stworzeni++;
        }
}}

void generator_bialych(int ile, int max_promien) { //funkcja generująca białe obiekty na planszy
    srand( time( NULL ) );
    int stworzeni = 0;
    int slepa_uliczka = 0;
    bool anuluj = false;
    float max_pole = max_promien * max_promien * M_PI;
    do {
        cout << "Generuje bialych..." << endl;
        biali[ilosc_bialych] = new obiekt(0);
        biali[ilosc_bialych] -> promien = rand()%max_promien + 5;
        biali[ilosc_bialych] -> wspolrzedne.x = (rand()%int(szer - 2 * (0.05 * szer + biali[ilosc_bialych] -> promien))) + int(szer * 0.05 + biali[ilosc_bialych] -> promien + 1);
        biali[ilosc_bialych] -> wspolrzedne.y = (rand()%int(wys - 2 * (0.05 * wys + biali[ilosc_bialych] -> promien))) + int(wys * 0.05 + biali[ilosc_bialych] -> promien + 1);
        if (ilosc_bialych != 0) {
            for (int i = 0; i < ilosc_bialych; i++) {
                if (biali[ilosc_bialych] != biali[i])
                    if (biali[ilosc_bialych] -> promien + biali[i] -> promien >= sqrt(pow(biali[ilosc_bialych] -> wspolrzedne.x - biali[i] -> wspolrzedne.x, 2) + pow(biali[ilosc_bialych] -> wspolrzedne.y - biali[i] -> wspolrzedne.y, 2))) {
                        anuluj = 1;
                    }
            }
        }
        if (ilosc_niebieskich != 0 && anuluj == false) {
            for (int i = 0; i < ilosc_niebieskich; i++) {
                if (biali[ilosc_bialych] -> promien + niebiescy[i] -> promien >= sqrt(pow(biali[ilosc_bialych] -> wspolrzedne.x - niebiescy[i] -> wspolrzedne.x, 2) + pow(biali[ilosc_bialych] -> wspolrzedne.y - niebiescy[i] -> wspolrzedne.y, 2))) {
                    anuluj = 1;
                }
            }
        }
        if (anuluj == true || biali[ilosc_bialych] -> promien + gracz.promien >= sqrt(pow(biali[ilosc_bialych] -> wspolrzedne.x - gracz.wspolrzedne.x, 2) + pow(biali[ilosc_bialych] -> wspolrzedne.y - gracz.wspolrzedne.y, 2)) ) {
            delete biali[ilosc_bialych];
            biali[ilosc_bialych] = NULL;
            anuluj = 0;
            slepa_uliczka++;
        }
        else {
            biali[ilosc_bialych] -> pole = biali[ilosc_bialych] -> promien * biali[ilosc_bialych] -> promien * M_PI;
            pole_do_rozdzialu -= biali[ilosc_bialych] -> pole;
            stworzeni++;
            ilosc_bialych++;
            slepa_uliczka = 0;
        }
        if (slepa_uliczka > 10) {
            break;
            slepa_uliczka = 0;
            stworzeni++;
        }
        } while (stworzeni < ile && pole_do_rozdzialu >= max_pole);
}


void tworzenie_bialych(obiekt *obiekt1, int *ilosc_bialych, int myszkax, int myszkay) { //funkcja tworząca białe obiekty dzięki którym gracz oraz niebiescy mogą się poruszać wykorzystując zjawisko odrzutu
    float sinkat, coskat;
    bool anuluj;
    float kat, kat1, kat2;
    wektor tempwektor;
    float szereg;
    if (*ilosc_bialych < tab_wsk_biali && obiekt1 -> pole > 15) {
                anuluj = 0;
                sinkat = abs(myszkay-obiekt1 -> wspolrzedne.y) / sqrt(pow(myszkax-obiekt1 -> wspolrzedne.x,2) + (pow(myszkay-obiekt1 -> wspolrzedne.y,2)));
                coskat = abs(obiekt1 -> wspolrzedne.x - myszkax) / sqrt(pow(obiekt1 -> wspolrzedne.x-myszkax, 2) + pow(obiekt1 -> wspolrzedne.y - myszkay, 2));
                if (myszkax >= obiekt1 -> wspolrzedne.x) {
                    if (myszkay >= obiekt1 -> wspolrzedne.y) {
                        kat1 = asinf(sinkat) + M_PI;
                        kat2 = acosf(coskat) + M_PI;
                        if (kat1 != 0.0)
                            kat = kat1;
                        else
                            kat = kat2; }
                    else {
                        kat1 = M_PI - asinf(sinkat);
                        kat2 = M_PI - acosf(coskat);
                        if (kat1 != 0.0)
                            kat = kat1;
                        else
                            kat = kat2; }}
                else {
                    if (myszkay >= obiekt1 -> wspolrzedne.y) {
                        kat1 = 2 * M_PI - asinf(sinkat);
                        kat2 = 2 * M_PI - asinf(sinkat);
                        if (kat1 != 0.0)
                            kat = kat1;
                        else
                            kat = kat2; }
                    else {
                         kat1 = asinf(sinkat);
                         kat2 = acosf(sinkat);
                         if (kat1 != 0.0)
                            kat = kat1;
                         else
                            kat = kat2; }}
                tempwektor.x = cosf(kat) * zwiekszanie_predkosci;
                tempwektor.y = sinf(kat) * zwiekszanie_predkosci;
                biali[*ilosc_bialych] = new obiekt(0);
                brak_pamieci(biali[*ilosc_bialych]);
                biali[*ilosc_bialych] -> kierunek.x = obiekt1 -> kierunek.x;
                biali[*ilosc_bialych] -> kierunek.y = obiekt1 -> kierunek.y;
                szereg = 0.5;
                biali[*ilosc_bialych] -> pole = 0.1 * obiekt1 -> pole * wys / 902.4;
                biali[*ilosc_bialych] -> promien = sqrtf(biali[*ilosc_bialych] -> pole/M_PI);
                if (myszkax >= obiekt1 -> wspolrzedne.x) {
                    if (myszkay >= obiekt1 -> wspolrzedne.y) {
                             biali[*ilosc_bialych] -> wspolrzedne.x = obiekt1 -> wspolrzedne.x + obiekt1 -> promien * coskat;
                             biali[*ilosc_bialych] -> wspolrzedne.y = obiekt1 -> wspolrzedne.y + obiekt1 -> promien * sinkat;
                             while(biali[*ilosc_bialych] -> promien + obiekt1 -> promien >= sqrt(pow(biali[*ilosc_bialych] -> wspolrzedne.x - obiekt1 -> wspolrzedne.x, 2) + pow(biali[*ilosc_bialych] -> wspolrzedne.y - obiekt1 -> wspolrzedne.y, 2)) ) {
                                biali[*ilosc_bialych] -> wspolrzedne.x += obiekt1 -> promien * szereg * coskat;
                                biali[*ilosc_bialych] -> wspolrzedne.y += obiekt1 -> promien * szereg * sinkat;
                                szereg /= 2;
                             }
                             if(biali[*ilosc_bialych] -> wspolrzedne.x >= szer - szer * 0.05 - biali[*ilosc_bialych] -> promien - biali[*ilosc_bialych] -> kierunek.x - 1 || biali[*ilosc_bialych] -> wspolrzedne.y >= wys - wys * 0.05 - biali[*ilosc_bialych] -> promien - biali[*ilosc_bialych] -> kierunek.y - 1) {
                                delete biali[*ilosc_bialych];
                                biali[*ilosc_bialych] = NULL;
                                anuluj = 1;
                             }}
                    else {
                             biali[*ilosc_bialych] -> wspolrzedne.x = obiekt1 -> wspolrzedne.x + obiekt1 -> promien * coskat;
                             biali[*ilosc_bialych] -> wspolrzedne.y = obiekt1 -> wspolrzedne.y - obiekt1 -> promien * sinkat;
                             while(biali[*ilosc_bialych] -> promien + obiekt1 -> promien >= sqrt(pow(biali[*ilosc_bialych] -> wspolrzedne.x - obiekt1 -> wspolrzedne.x, 2) + pow(biali[*ilosc_bialych] -> wspolrzedne.y - obiekt1 -> wspolrzedne.y, 2)) ) {
                                biali[*ilosc_bialych] -> wspolrzedne.x += obiekt1 -> promien * szereg * coskat;
                                biali[*ilosc_bialych] -> wspolrzedne.y -= obiekt1 -> promien * szereg * sinkat;
                                szereg /= 2;
                             }
                             if(biali[*ilosc_bialych] -> wspolrzedne.x >= szer - szer * 0.05 - biali[*ilosc_bialych] -> kierunek.x - biali[*ilosc_bialych] -> promien - 1 || biali[*ilosc_bialych] -> wspolrzedne.y <= wys * 0.05 + biali[*ilosc_bialych] -> promien - biali[*ilosc_bialych] -> kierunek.y + 1) {
                                delete biali[*ilosc_bialych];
                                biali[*ilosc_bialych] = NULL;
                                anuluj = 1;
                             }}}
                else {
                    if (myszkay >= obiekt1 -> wspolrzedne.y) {
                             biali[*ilosc_bialych] -> wspolrzedne.x = obiekt1 -> wspolrzedne.x - obiekt1 -> promien * coskat;
                             biali[*ilosc_bialych] -> wspolrzedne.y = obiekt1 -> wspolrzedne.y + obiekt1 -> promien * sinkat;
                             while(biali[*ilosc_bialych] -> promien + obiekt1 -> promien >= sqrt(pow(biali[*ilosc_bialych] -> wspolrzedne.x - obiekt1 -> wspolrzedne.x, 2) + pow(biali[*ilosc_bialych] -> wspolrzedne.y - obiekt1 -> wspolrzedne.y, 2)) ) {
                                biali[*ilosc_bialych] -> wspolrzedne.x -= obiekt1 -> promien * szereg * coskat;
                                biali[*ilosc_bialych] -> wspolrzedne.y += obiekt1 -> promien * szereg * sinkat;
                                szereg /= 2;
                             }
                             if(biali[*ilosc_bialych] -> wspolrzedne.x <= szer * 0.05 + biali[*ilosc_bialych] -> promien - biali[*ilosc_bialych] -> kierunek.x + 1 || biali[*ilosc_bialych] -> wspolrzedne.y >= wys - wys * 0.05 - biali[*ilosc_bialych] -> promien - biali[*ilosc_bialych] -> kierunek.y - 1) {
                                delete biali[*ilosc_bialych];
                                biali[*ilosc_bialych] = NULL;
                                anuluj = 1;
                             }}
                    else {
                             biali[*ilosc_bialych] -> wspolrzedne.x = obiekt1 -> wspolrzedne.x - obiekt1 -> promien * coskat;
                             biali[*ilosc_bialych] -> wspolrzedne.y = obiekt1 -> wspolrzedne.y - obiekt1 -> promien * sinkat;
                             while(biali[*ilosc_bialych] -> promien + obiekt1 -> promien >= sqrt(pow(biali[*ilosc_bialych] -> wspolrzedne.x - obiekt1 -> wspolrzedne.x, 2) + pow(biali[*ilosc_bialych] -> wspolrzedne.y - obiekt1 -> wspolrzedne.y, 2)) ) {
                                biali[*ilosc_bialych] -> wspolrzedne.x -= obiekt1 -> promien * szereg * coskat;
                                biali[*ilosc_bialych] -> wspolrzedne.y -= obiekt1 -> promien * szereg * sinkat;
                                szereg /= 2;
                             }
                             if(biali[*ilosc_bialych] -> wspolrzedne.x <= szer * 0.05 + biali[*ilosc_bialych] -> promien - biali[*ilosc_bialych] -> kierunek.x + 1 || biali[*ilosc_bialych] -> wspolrzedne.y <= wys * 0.05 + biali[*ilosc_bialych] -> promien - biali[*ilosc_bialych] -> kierunek.y + 1) {
                                delete biali[*ilosc_bialych];
                                biali[*ilosc_bialych] = NULL;
                                anuluj = 1;
                             }}}
                if (anuluj != 1) {
                    biali[*ilosc_bialych] -> kierunek.x -= tempwektor.x;
                    biali[*ilosc_bialych] -> kierunek.y -= tempwektor.y;
                    obiekt1 -> kierunek.x += tempwektor.x;
                    obiekt1 -> kierunek.y += tempwektor.y;
                    obiekt1 -> pole -= biali[*ilosc_bialych] -> pole;
                    if (obiekt1 -> pole < 0.0)
                        obiekt1 -> pole = 0.0;
                    obiekt1 -> promien = sqrtf(obiekt1 -> pole/M_PI);
                    setcolor(WHITE);
                    circle(biali[*ilosc_bialych] -> wspolrzedne.x, biali[*ilosc_bialych] -> wspolrzedne.y,biali[*ilosc_bialych] -> promien);
                    if (biali[*ilosc_bialych] -> promien > 2) {
                        setfillstyle(SOLID_FILL, WHITE);
                        floodfill(biali[*ilosc_bialych] -> wspolrzedne.x, biali[*ilosc_bialych] -> wspolrzedne.y, WHITE); }
                    *ilosc_bialych += 1; }
            }
}

void sprzatanie_bialych (int *ilosc_bialych) { //funkcja kasująca białe obiekty z pamięci, gdy posiadają one pole <= 0
    for (int i = 0; i<*ilosc_bialych; i++) {
            if (biali[i] != NULL) {
                if (biali[i] -> pole <= 0.0){
                    delete biali[i];
                    *ilosc_bialych -= 1;
                    if (*ilosc_bialych > 0) {
                        biali[i] = biali[*ilosc_bialych];
                        biali[*ilosc_bialych] = NULL; }
                    else
                        biali[i] = NULL; }
    }}}

void sprzatanie_niebieskich (int *ilosc_niebieskich) { //funkcja kasująca niebieskie obiekty z pamięci, gdy posiadają one pole <= 0
    for (int i = 0; i<*ilosc_niebieskich; i++) {
        if (niebiescy[i] != NULL) {
            if (niebiescy[i] -> pole <= 0.0) {
                delete niebiescy[i];
                *ilosc_niebieskich -= 1;
                if (*ilosc_niebieskich > 0) {
                    niebiescy[i] = niebiescy[*ilosc_niebieskich];
                    niebiescy[*ilosc_niebieskich] = NULL; }
                else
                    niebiescy[i] = NULL; }
        }}
}

void walki(int *ilosc_bialych, int *ilosc_niebieskich) { //funkcja generująca walki pomiędzy wszystkimi obiektami
        for (int i = 0; i<*ilosc_bialych; i++) {
            walka(&gracz, biali[i]);
        }
        for (int i = 0; i<*ilosc_niebieskich; i++) {
            walka(&gracz, niebiescy[i]);
        }
        for (int j = 0; j<*ilosc_bialych; j++) {
            for (int i = 0; i<*ilosc_bialych; i++) {
                if (biali[j] != biali[i])
                    walka(biali[j], biali[i]);
            }
            sprzatanie_bialych(ilosc_bialych);}
        for (int j = 0; j<*ilosc_niebieskich; j++) {
            for (int i = 0; i<*ilosc_bialych; i++) {
                if (niebiescy[j] != biali[i])
                    walka(niebiescy[j], biali[i]);
            }}
        for (int j = 0; j<*ilosc_niebieskich; j++) {
            for (int i = 0; i<*ilosc_niebieskich; i++) {
                if (niebiescy[j] != niebiescy[i])
                    walka(niebiescy[j], niebiescy[i]);
            }
            sprzatanie_niebieskich(ilosc_niebieskich);}
}

void odbicia(int *ilosc_bialych,int *ilosc_niebieskich) { //funkcja generująca odbicia wszystkich obiektów
    odbicie(&gracz);
    for (int i = 0; i<*ilosc_bialych; i++) {
        if (biali[i] != NULL)
            odbicie(biali[i]);
    }
    for (int i = 0; i<*ilosc_niebieskich; i++) {
        if (niebiescy[i] != NULL)
            odbicie(niebiescy[i]);
    }
}

void ruchy(int *ilosc_bialych, int *ilosc_niebieskich) { //funkcja generująca ruchy wszystkich obiektów
    ruch(&gracz);
    for (int i = 0; i<*ilosc_bialych; i++) {
        if (biali[i] != NULL) {
            ruch(biali[i]); }
    }
    for (int i = 0; i<*ilosc_niebieskich; i++) {
        if (niebiescy[i] != NULL) {
            ruch(niebiescy[i]); }
    }
}

struct fann *ann = fann_create_from_file("AI.net"); //wczytanie sieci neuronowej do pamieci

void AI(obiekt_niebieski *obiekt1, int *ilosc_bialych, int *ilosc_niebieskich) { //funkcja odpowiedzialna za podejmowanie decyzji przez przeciwnika
    unsigned long int aktualny;
    float odleglosc_przed, odleglosc_po;
    float wejscieobiekt_x, wejscieobiekt_y, wejsciecel_x, wejsciecel_y;
    float wiekszy, kierunek_ruchu;
    fann_type *wyjscie;
    fann_type wejscie[6];
    obiekt *najblizej;
    aktualny = GetTickCount();
    if (aktualny - obiekt1 -> wystrzelenie > 500) {
        najblizej = &gracz;
        float odleglosc = sqrt(pow(gracz.wspolrzedne.x - obiekt1 -> wspolrzedne.x, 2) + pow(gracz.wspolrzedne.y - obiekt1 -> wspolrzedne.y, 2));
        for (int i=0; i<*ilosc_niebieskich; i++) {
            if (obiekt1 != niebiescy[i])
                if (odleglosc > sqrt(pow(niebiescy[i] -> wspolrzedne.x - obiekt1 -> wspolrzedne.x, 2) + pow(niebiescy[i] -> wspolrzedne.y - obiekt1 -> wspolrzedne.y, 2))) {
                    najblizej = niebiescy[i];
                    odleglosc = sqrt(pow(niebiescy[i] -> wspolrzedne.x - obiekt1 -> wspolrzedne.x, 2) + pow(niebiescy[i] -> wspolrzedne.y - obiekt1 -> wspolrzedne.y, 2));
                }
        }
        for (int i=0; i<*ilosc_bialych; i++) {
            if (odleglosc > sqrt(pow(biali[i] -> wspolrzedne.x - obiekt1 -> wspolrzedne.x, 2) + pow(biali[i] -> wspolrzedne.y - obiekt1 -> wspolrzedne.y, 2))) {
                najblizej = biali[i];
                odleglosc = sqrt(pow(biali[i] -> wspolrzedne.x - obiekt1 -> wspolrzedne.x, 2) + pow(biali[i] -> wspolrzedne.y - obiekt1 -> wspolrzedne.y, 2));
            }
        }
        obiekt1 -> cel = najblizej;
        odleglosc_przed = sqrt(pow(obiekt1 -> wspolrzedne.x - obiekt1 -> cel -> wspolrzedne.x, 2) + pow(obiekt1 -> wspolrzedne.y - obiekt1 -> cel -> wspolrzedne.y,2));
        odleglosc_po = sqrt(pow((obiekt1 -> wspolrzedne.x + obiekt1 -> kierunek.x) - (obiekt1 -> cel -> wspolrzedne.x + obiekt1 -> cel -> kierunek.x),2) + pow((obiekt1 -> wspolrzedne.x + obiekt1 -> kierunek.x) - (obiekt1 -> cel -> wspolrzedne.x + obiekt1 -> cel -> kierunek.x),2));
        if (obiekt1 -> pole * 0.9 <= obiekt1 -> cel -> pole) {
            wiekszy = 1.0;
        }
        else {
            wiekszy = -1.0;
        }
        if (odleglosc_przed < odleglosc_po) { //ucieka
            kierunek_ruchu = 1.0;
        }
        else if (odleglosc_przed = odleglosc_po) { //stoi
            kierunek_ruchu = 0.0;
        }
        else { //zbliza sie
            kierunek_ruchu = -1.0;
        }
        wejscieobiekt_x = (obiekt1 -> wspolrzedne.x - (szer / 2)) / (szer/2);
        wejscieobiekt_y = (obiekt1 -> wspolrzedne.y - (wys / 2)) / (wys/2);
        wejsciecel_x = ((obiekt1 -> cel -> wspolrzedne.x + obiekt1 -> cel -> kierunek.x) - (szer / 2)) / (szer/2);
        wejsciecel_y = ((obiekt1 -> cel -> wspolrzedne.y + obiekt1 -> cel -> kierunek.x) - (wys / 2)) / (wys/2);
        wejscie[0] = wejscieobiekt_x;
        wejscie[1] = wejscieobiekt_y;
        wejscie[2] = wejsciecel_x;
        wejscie[3] = wejsciecel_y;
        wejscie[4] = kierunek_ruchu;
        wejscie[5] = wiekszy;
        wyjscie = fann_run(ann, wejscie); //pobranie wynikow od sieci neuronowej
        if ((wyjscie[0] > 0.05 || wyjscie[0] < -0.05) || (wyjscie[1] > 0.05 || wyjscie[1] < -0.05)) {
            tworzenie_bialych(obiekt1, ilosc_bialych, szer/2 + szer/2 * wyjscie[0], wys/2 + wys/2 * wyjscie[1]);
            obiekt1 -> wystrzelenie = GetTickCount(); }
    }
}

void wyswietlanie_predkosci_gry(int *predkosc) { //funkcja odpowiedzialna za wyświetlanie prędkości na HUD-ie
    char predkosc_gry[20] = {"Predkosc gry: "};
    char predkosc_napis[3];
    if (*predkosc == 0) {
        strcat(predkosc_gry, "1x");
        setcolor(WHITE);
        outtextxy(szer - szer * 0.20, wys * 0.02, predkosc_gry);
    }
    else if (*predkosc == 25) {
        strcat(predkosc_gry, "0.5x");
        setcolor(WHITE);
        outtextxy(szer - szer * 0.20, wys * 0.02, predkosc_gry);
    }
    else {
        strcat(predkosc_gry, "0.25x");
        setcolor(WHITE);
        outtextxy(szer - szer * 0.20, wys * 0.02, predkosc_gry);
    }
}

void zapisz_do_pliku(time_t *przerwy, time_t *start, time_t *wstrzymanie, time_t *zapisy) { //funkcja zapisujaca gre w pliku
    FILE *status;
    status = fopen("zapis.dat", "w");
    if (status == NULL) {
        int wylaczanie[12] {
        srodek.x - 300, srodek.y,
        srodek.x - 150, srodek.y - 150,
        srodek.x + 150, srodek.y - 150,
        srodek.x + 300, srodek.y,
        srodek.x + 150, srodek.y + 150,
        srodek.x - 150, srodek.y + 150 };
        int myszkax, myszkay;
        bool kontynuuj = false;
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, RED);
        fillpoly(6, wylaczanie);
        setcolor(WHITE);
        outtextxy(srodek.x - 55, srodek.y - 125,"JAMES BLAD 404");
        readimagefile("err.is", srodek.x - 150, srodek.y - 100, srodek.x + 150, srodek.y + 25);
        outtextxy(srodek.x - 120, srodek.y + 35,"Nie udalo sie zapisac gry do pliku.");
        setcolor(BLACK);
        rectangle(srodek.x - 45, srodek.y + 75, srodek.x + 45, srodek.y + 125);
        setfillstyle(SOLID_FILL, BLACK);
        floodfill(srodek.x - 44, srodek.y + 76, BLACK);
        setcolor(WHITE);
        outtextxy(srodek.x - 29, srodek.y + 95, "Kontynuuj");
        do {
            czyszczenie_bufora();
            if (ismouseclick(WM_LBUTTONDOWN)) {
                getmouseclick(WM_LBUTTONDOWN, myszkax, myszkay);
                if (myszkax >= srodek.x - 45 && myszkax <= srodek.x + 50 && myszkay >= srodek.y + 75 && myszkay <= srodek.y + 125) {
                    kontynuuj = true; }
        }} while (kontynuuj == false);
    }
    else {
        time_t przerwy_tmp, zapisy_tmp, aktualny_czas;
        time(&aktualny_czas);
        przerwy_tmp = aktualny_czas - *wstrzymanie;
        zapisy_tmp = aktualny_czas - *start + *zapisy - przerwy_tmp - *przerwy;
        int wysokosc = getmaxheight();
        fprintf(status, "3 %f %d %d\n", poczatkowe_pole, wysokosc, zapisy_tmp);
        for (int i = 0; i < ilosc_bialych; i++) {
            fprintf(status, "0 %f %f %f %f %f %f\n", biali[i] -> wspolrzedne.x, biali[i] -> wspolrzedne.y, biali[i] -> kierunek.x, biali[i] -> kierunek.y, biali[i] -> promien, biali[i] -> pole);
        }
        for (int i = 0; i < ilosc_niebieskich; i++) {
            fprintf(status, "2 %f %f %f %f %f %f\n", niebiescy[i] -> wspolrzedne.x, niebiescy[i] -> wspolrzedne.y, niebiescy[i] -> kierunek.x, niebiescy[i] -> kierunek.y, niebiescy[i] -> promien, niebiescy[i] -> pole);
        }
        fprintf(status, "1 %f %f %f %f %f %f\n", gracz.wspolrzedne.x, gracz.wspolrzedne.y, gracz.kierunek.x, gracz.kierunek.y, gracz.promien, gracz.pole);
        fclose(status);
        int wylaczanie[12] {
        srodek.x - 300, srodek.y,
        srodek.x - 150, srodek.y - 150,
        srodek.x + 150, srodek.y - 150,
        srodek.x + 300, srodek.y,
        srodek.x + 150, srodek.y + 150,
        srodek.x - 150, srodek.y + 150 };
        int myszkax, myszkay;
        bool kontynuuj = false;
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, RED);
        fillpoly(6, wylaczanie);
        setcolor(WHITE);
        outtextxy(srodek.x - 65, srodek.y - 75,"Zapisano gre w pliku.");
        setcolor(BLACK);
        rectangle(srodek.x - 45, srodek.y + 50, srodek.x + 45, srodek.y + 100);
        setfillstyle(SOLID_FILL, BLACK);
        floodfill(srodek.x - 44, srodek.y + 51, BLACK);
        setcolor(WHITE);
        outtextxy(srodek.x - 30, srodek.y + 70, "Kontynuuj");
        do {
            czyszczenie_bufora();
            if (ismouseclick(WM_LBUTTONDOWN)) {
                getmouseclick(WM_LBUTTONDOWN, myszkax, myszkay);
                if (myszkax >= srodek.x - 45 && myszkax <= srodek.x + 50 && myszkay >= srodek.y + 50 && myszkay <= srodek.y + 100) {
                    kontynuuj = true; }
        }} while (kontynuuj == false);
    }

}

void zapis(time_t *przerwy, time_t *start, time_t *wstrzymanie, time_t *zapisy, char *argv[]) { //funkcja zapisujaca gre w bazie danych
    PGconn *stan;
    PGresult *status_tmp;
    char bufor[100];
    time_t aktualny_czas;
    char nazwa_zapisu[30];
    char *ip = argv[5];
    char *login = argv[6];
    char *haslo = argv[7];
    char *port = argv[9];
    char *dbname = argv[8];
    char polecenie[1000];
    strcpy(polecenie, "hostaddr = ");
    strcat(polecenie, ip);
    strcat(polecenie, " port = ");
    strcat(polecenie, port);
    strcat(polecenie, " dbname = ");
    strcat(polecenie, dbname);
    strcat(polecenie, " user = ");
    strcat(polecenie, login);
    strcat(polecenie, " password = ");
    strcat(polecenie, haslo);
    stan = PQconnectdb(polecenie);
    if (PQstatus(stan) != CONNECTION_OK) {
        int wylaczanie[12] {
        srodek.x - 300, srodek.y,
        srodek.x - 150, srodek.y - 150,
        srodek.x + 150, srodek.y - 150,
        srodek.x + 300, srodek.y,
        srodek.x + 150, srodek.y + 150,
        srodek.x - 150, srodek.y + 150 };
        int myszkax, myszkay;
        bool kontynuuj = false;
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, RED);
        fillpoly(6, wylaczanie);
        setcolor(WHITE);
        outtextxy(srodek.x - 55, srodek.y - 125,"JAMES BLAD 404");
        readimagefile("err.is", srodek.x - 150, srodek.y - 100, srodek.x + 150, srodek.y + 25);
        outtextxy(srodek.x - 130, srodek.y + 35,"Nie udalo sie zapisac gry do bazy danych.");
        setcolor(BLACK);
        rectangle(srodek.x - 45, srodek.y + 75, srodek.x + 45, srodek.y + 125);
        setfillstyle(SOLID_FILL, BLACK);
        floodfill(srodek.x - 44, srodek.y + 76, BLACK);
        setcolor(WHITE);
        outtextxy(srodek.x - 29, srodek.y + 95, "Kontynuuj");
        do {
            czyszczenie_bufora();
            if (ismouseclick(WM_LBUTTONDOWN)) {
                getmouseclick(WM_LBUTTONDOWN, myszkax, myszkay);
                if (myszkax >= srodek.x - 45 && myszkax <= srodek.x + 50 && myszkay >= srodek.y + 75 && myszkay <= srodek.y + 125) {
                    kontynuuj = true; }
        }} while (kontynuuj == false);
    }
    else {
        char polecenie_SQL[1000] = "CREATE TABLE ";
        float wysokosc = getmaxheight();
        time(&aktualny_czas);
        tm czasTM = *localtime(&aktualny_czas);
        strftime(nazwa_zapisu, 30, "G%d%m%y_%H%M%S", &czasTM);
        strcat(polecenie_SQL, nazwa_zapisu);
        strcat(polecenie_SQL, " (wspolrzedne_x FLOAT, wspolrzedne_y FLOAT, predkosc_x FLOAT, predkosc_y FLOAT, promien FLOAT, pole FLOAT, rodzaj INT);");
        status_tmp = PQexec(stan, polecenie_SQL);
        for (int i = 0; i < ilosc_bialych; i++) {
            strcpy(polecenie_SQL, "INSERT INTO ");
            strcat(polecenie_SQL, nazwa_zapisu);
            strcat(polecenie_SQL, " (wspolrzedne_x, wspolrzedne_y, predkosc_x, predkosc_y, promien, pole, rodzaj) VALUES ('");
            sprintf(bufor, "%f", biali[i] -> wspolrzedne.x);
            strcat(polecenie_SQL, bufor);
            strcat(polecenie_SQL, "', '");
            sprintf(bufor, "%f", biali[i] -> wspolrzedne.y);
            strcat(polecenie_SQL, bufor);
            strcat(polecenie_SQL, "', '");
            sprintf(bufor, "%f", biali[i] -> kierunek.x);
            strcat(polecenie_SQL, bufor);
            strcat(polecenie_SQL, "', '");
            sprintf(bufor, "%f", biali[i] -> kierunek.y);
            strcat(polecenie_SQL, bufor);
            strcat(polecenie_SQL, "', '");
            sprintf(bufor, "%f", biali[i] -> promien);
            strcat(polecenie_SQL, bufor);
            strcat(polecenie_SQL, "', '");
            sprintf(bufor, "%f", biali[i] -> pole);
            strcat(polecenie_SQL, bufor);
            strcat(polecenie_SQL, "', '");
            sprintf(bufor, "%d", biali[i] -> rodzaj);
            strcat(polecenie_SQL, bufor);
            strcat(polecenie_SQL, "');");
            status_tmp = PQexec(stan, polecenie_SQL);
            //cout << PQresultErrorMessage(status_tmp);
        }
        for (int i = 0; i < ilosc_niebieskich; i++) {
            strcpy(polecenie_SQL, "INSERT INTO ");
            strcat(polecenie_SQL, nazwa_zapisu);
            strcat(polecenie_SQL, " (wspolrzedne_x, wspolrzedne_y, predkosc_x, predkosc_y, promien, pole, rodzaj) VALUES ('");
            sprintf(bufor, "%f", niebiescy[i] -> wspolrzedne.x);
            strcat(polecenie_SQL, bufor);
            strcat(polecenie_SQL, "', '");
            sprintf(bufor, "%f", niebiescy[i] -> wspolrzedne.y);
            strcat(polecenie_SQL, bufor);
            strcat(polecenie_SQL, "', '");
            sprintf(bufor, "%f", niebiescy[i] -> kierunek.x);
            strcat(polecenie_SQL, bufor);
            strcat(polecenie_SQL, "', '");
            sprintf(bufor, "%f", niebiescy[i] -> kierunek.y);
            strcat(polecenie_SQL, bufor);
            strcat(polecenie_SQL, "', '");
            sprintf(bufor, "%f", niebiescy[i] -> promien);
            strcat(polecenie_SQL, bufor);
            strcat(polecenie_SQL, "', '");
            sprintf(bufor, "%f", niebiescy[i] -> pole);
            strcat(polecenie_SQL, bufor);
            strcat(polecenie_SQL, "', '");
            sprintf(bufor, "%d", niebiescy[i] -> rodzaj);
            strcat(polecenie_SQL, bufor);
            strcat(polecenie_SQL, "');");
            status_tmp = PQexec(stan, polecenie_SQL);
            //cout << PQresultErrorMessage(status_tmp);
        }
        strcpy(polecenie_SQL, "INSERT INTO ");
        strcat(polecenie_SQL, nazwa_zapisu);
        strcat(polecenie_SQL, " (wspolrzedne_x, wspolrzedne_y, predkosc_x, predkosc_y, promien, pole, rodzaj) VALUES ('");
        sprintf(bufor, "%f", gracz.wspolrzedne.x);
        strcat(polecenie_SQL, bufor);
        strcat(polecenie_SQL, "', '");
        sprintf(bufor, "%f", gracz.wspolrzedne.y);
        strcat(polecenie_SQL, bufor);
        strcat(polecenie_SQL, "', '");
        sprintf(bufor, "%f", gracz.kierunek.x);
        strcat(polecenie_SQL, bufor);
        strcat(polecenie_SQL, "', '");
        sprintf(bufor, "%f", gracz.kierunek.y);
        strcat(polecenie_SQL, bufor);
        strcat(polecenie_SQL, "', '");
        sprintf(bufor, "%f", gracz.promien);
        strcat(polecenie_SQL, bufor);
        strcat(polecenie_SQL, "', '");
        sprintf(bufor, "%f", gracz.pole);
        strcat(polecenie_SQL, bufor);
        strcat(polecenie_SQL, "', '");
        sprintf(bufor, "%d", gracz.rodzaj);
        strcat(polecenie_SQL, bufor);
        strcat(polecenie_SQL, "');");
        PQexec(stan, polecenie_SQL);
        strcpy(polecenie_SQL, "INSERT INTO ");
        strcat(polecenie_SQL, nazwa_zapisu);
        strcat(polecenie_SQL, " (wspolrzedne_x, wspolrzedne_y, predkosc_x, predkosc_y, promien, pole, rodzaj) VALUES ('");
        sprintf(bufor, "%f", poczatkowe_pole);
        strcat(polecenie_SQL, bufor);
        strcat(polecenie_SQL, "', '");
        sprintf(bufor, "%f", wysokosc);
        strcat(polecenie_SQL, bufor);
        strcat(polecenie_SQL, "', '0', '0', '0', '");
        time(&aktualny_czas);
        time_t przerwy_tmp, zapisy_tmp;
        przerwy_tmp = aktualny_czas - *wstrzymanie;
        zapisy_tmp = aktualny_czas - *start + *zapisy - przerwy_tmp - *przerwy;
        sprintf(bufor, "%d", zapisy_tmp);
        strcat(polecenie_SQL, bufor);
        strcat(polecenie_SQL, "', '3');");
        status_tmp = PQexec(stan, polecenie_SQL);
        //cout << PQresultErrorMessage(status_tmp);
        PQfinish(stan);
        PQclear(status_tmp);
        int wylaczanie[12] {
        srodek.x - 300, srodek.y,
        srodek.x - 150, srodek.y - 150,
        srodek.x + 150, srodek.y - 150,
        srodek.x + 300, srodek.y,
        srodek.x + 150, srodek.y + 150,
        srodek.x - 150, srodek.y + 150 };
        int myszkax, myszkay;
        bool kontynuuj = false;
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, RED);
        fillpoly(6, wylaczanie);
        setcolor(WHITE);
        outtextxy(srodek.x - 100, srodek.y - 75,"Zapisano gre w bazie danych.");
        setcolor(BLACK);
        rectangle(srodek.x - 45, srodek.y + 50, srodek.x + 45, srodek.y + 100);
        setfillstyle(SOLID_FILL, BLACK);
        floodfill(srodek.x - 44, srodek.y + 51, BLACK);
        setcolor(WHITE);
        outtextxy(srodek.x - 30, srodek.y + 70, "Kontynuuj");
        do {
            czyszczenie_bufora();
            if (ismouseclick(WM_LBUTTONDOWN)) {
                getmouseclick(WM_LBUTTONDOWN, myszkax, myszkay);
                if (myszkax >= srodek.x - 45 && myszkax <= srodek.x + 50 && myszkay >= srodek.y + 50 && myszkay <= srodek.y + 100) {
                    kontynuuj = true; }
        }} while (kontynuuj == false);
    }
}

void wyjscie_menu (time_t *przerwy, bool *wyjscie, time_t *start, time_t *zapisy, char *argv[], int argc) { //funkcja wyświetlająca menu wyjścia
    int wylaczanie[12] {
        srodek.x - 300, srodek.y,
        srodek.x - 150, srodek.y - 150,
        srodek.x + 150, srodek.y - 150,
        srodek.x + 300, srodek.y,
        srodek.x + 150, srodek.y + 150,
        srodek.x - 150, srodek.y + 150 };
    time_t aktualny, wstrzymanie;
    int myszkax, myszkay;
    bool kontynuuj = false;
    if (getactivepage() == 0)
        setactivepage(1);
    else
        setactivepage(0);
    time(&wstrzymanie);
    setcolor(WHITE);
    setfillstyle(SOLID_FILL, RED);
    fillpoly(6, wylaczanie);
    setcolor(WHITE);
    outtextxy(srodek.x - 100, srodek.y - 75,"Czy na pewno chcesz zakonczyc?");
    setcolor(BLACK);
    rectangle(srodek.x - 100, srodek.y + 50, srodek.x - 50, srodek.y + 100);
    rectangle(srodek.x + 50, srodek.y + 50, srodek.x + 100, srodek.y + 100);
    rectangle(srodek.x - 45, srodek.y + 50, srodek.x + 45, srodek.y + 100);
    setfillstyle(SOLID_FILL, BLACK);
    floodfill(srodek.x - 99, srodek.y + 51, BLACK);
    floodfill(srodek.x + 51, srodek.y + 51, BLACK);
    floodfill(srodek.x - 44, srodek.y + 51, BLACK);
    setcolor(WHITE);
    outtextxy(srodek.x - 88, srodek.y + 70, "TAK");
    outtextxy(srodek.x + 63, srodek.y + 70, "NIE");
    outtextxy(srodek.x - 22, srodek.y + 70, "Zapisz");
    do {
        czyszczenie_bufora();
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, myszkax, myszkay);
            if (myszkax >= srodek.x - 100 && myszkax <= srodek.x - 50 && myszkay >= srodek.y + 50 && myszkay <= srodek.y + 100)
                *wyjscie = true;
            else if (myszkax >= srodek.x + 50 && myszkax <= srodek.x + 100 && myszkay >= srodek.y + 50 && myszkay <= srodek.y + 100)
                kontynuuj = true;
            else if (myszkax >= srodek.x - 45 && myszkax <= srodek.x + 50 && myszkay >= srodek.y + 50 && myszkay <= srodek.y + 100) {
                if (argc > 5 && strcmp(argv[4], "1") == 0) {
                    zapis(przerwy, start, &wstrzymanie, zapisy, argv);
                }
                else {
                    zapisz_do_pliku(przerwy, start, &wstrzymanie, zapisy);
                }
                kontynuuj = true; }
    }} while (*wyjscie == false && kontynuuj == false);
    time(&aktualny);
    *przerwy += aktualny - wstrzymanie;
}

void pomoc(time_t *przerwy) { //funkcja wyświetlająca pomoc do gry
    if (getch() == 59) {
        int okienko[12] {
            srodek.x - 400, srodek.y,
            srodek.x - 250, srodek.y - 200,
            srodek.x + 250, srodek.y - 200,
            srodek.x + 400, srodek.y,
            srodek.x + 250, srodek.y + 200,
            srodek.x - 250, srodek.y + 200 };
        bool kontynuuj;
        time_t aktualny, wstrzymanie;
        int myszkax, myszkay;
        if (getactivepage() == 0)
            setactivepage(1);
        else
            setactivepage(0);
        time(&wstrzymanie);
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, RED);
        fillpoly(6, okienko);
        setcolor(WHITE);
        outtextxy(srodek.x - 25, srodek.y - 100,"POMOC");
        outtextxy(srodek.x - 250, srodek.y - 75,"Celem gry jest zdobycie 85% masy wszystkich kulek w jak najkrotszym czasie.");
        outtextxy(srodek.x - 100, srodek.y - 50,"Do sterowania nalezy uzywac LPM.");
        outtextxy(srodek.x - 325, srodek.y - 25,"Dodatkowym ulatwieniem dla gracza jest zmiana predkosci gry z wykorzystaniem klawiszy \"1\", \"2\" i \"3\".");
        setcolor(BLACK);
        rectangle(srodek.x - 50, srodek.y + 50, srodek.x + 50, srodek.y + 100);
        setfillstyle(SOLID_FILL, BLACK);
        floodfill(srodek.x - 49, srodek.y + 51, BLACK);
        setcolor(WHITE);
        outtextxy(srodek.x - 33, srodek.y + 70, "Kontynuuj");
        do {
            czyszczenie_bufora();
            if (ismouseclick(WM_LBUTTONDOWN)) {
                getmouseclick(WM_LBUTTONDOWN, myszkax, myszkay);
                if (myszkax >= srodek.x - 50 && myszkax <= srodek.x + 50 && myszkay >= srodek.y + 50 && myszkay <= srodek.y + 100)
                    kontynuuj = true;
        }} while (kontynuuj == false);
        time(&aktualny);
        *przerwy += aktualny - wstrzymanie;
    }
}

void poprawa_bledow(obiekt *obiekt1) { //szukanie błędów w pozycjach obiektów i obliczanie poprawek
    if (obiekt1 -> wspolrzedne.x <= szer * 0.05) {
        obiekt1 -> wspolrzedne.x += (szer * 0.05 - obiekt1 -> wspolrzedne.x) + 1;
    }
    if (obiekt1 -> wspolrzedne.x >= szer * 0.95) {
        obiekt1 -> wspolrzedne.x -= (obiekt1 -> wspolrzedne.x - 0.95 * szer) - 1;
    }
    if (obiekt1 -> wspolrzedne.y <= wys * 0.05) {
        obiekt1 -> wspolrzedne.y += (wys * 0.05 - obiekt1 -> wspolrzedne.y) + 1;
    }
    if (obiekt1 -> wspolrzedne.y >= wys * 0.95) {
        obiekt1 -> wspolrzedne.y -= (obiekt1 -> wspolrzedne.y - 0.95 * wys) - 1;
    }
}

void wprowadzenie_poprawek() { //wprowadzanie poprawek
    for (int i=0; i<ilosc_bialych; i++) {
        poprawa_bledow(biali[i]);
    }
    for (int i=0; i<ilosc_niebieskich; i++) {
        poprawa_bledow(niebiescy[i]);
    }
}

void przegral(bool wyjscie, char aktualny_czas[]) { //informacja o przegraniu gry
        int myszkax, myszkay;
        char zakonczenie[50];
        int okienko[12] {
            srodek.x - 400, srodek.y,
            srodek.x - 250, srodek.y - 200,
            srodek.x + 250, srodek.y - 200,
            srodek.x + 400, srodek.y,
            srodek.x + 250, srodek.y + 200,
            srodek.x - 250, srodek.y + 200 };
        bool kontynuuj;
        if (wyjscie != true) {
            if (getactivepage() == 0)
                setactivepage(1);
            else
                setactivepage(0); }
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, RED);
        fillpoly(6, okienko);
        setcolor(WHITE);
        strcpy(zakonczenie, "Na planszy udalo Ci sie przetrwac: ");
        strcat(zakonczenie, aktualny_czas);
        outtextxy(srodek.x - 35, srodek.y - 100,"SMUTECZEK");
        outtextxy(srodek.x - 60, srodek.y - 75,"Niestety przegrales.");
        outtextxy(srodek.x - 125, srodek.y - 50, zakonczenie);
        outtextxy(srodek.x - 110, srodek.y - 25,"Moze uda sie nastepnym razem! :)");
        setcolor(BLACK);
        rectangle(srodek.x - 50, srodek.y + 50, srodek.x + 50, srodek.y + 100);
        setfillstyle(SOLID_FILL, BLACK);
        floodfill(srodek.x - 49, srodek.y + 51, BLACK);
        setcolor(WHITE);
        outtextxy(srodek.x - 28, srodek.y + 70, "Zakoncz");
        do {
            if (ismouseclick(WM_LBUTTONDOWN)) {
                getmouseclick(WM_LBUTTONDOWN, myszkax, myszkay);
                if (myszkax >= srodek.x - 50 && myszkax <= srodek.x + 50 && myszkay >= srodek.y + 50 && myszkay <= srodek.y + 100)
                    kontynuuj = true;
        }} while (kontynuuj == false);
}

void wygral(char aktualny_czas[]) { //informacja o wygraniu gry
        char zakonczenie[50];
        int myszkax, myszkay;
        int okienko[12] {
            srodek.x - 400, srodek.y,
            srodek.x - 250, srodek.y - 200,
            srodek.x + 250, srodek.y - 200,
            srodek.x + 400, srodek.y,
            srodek.x + 250, srodek.y + 200,
            srodek.x - 250, srodek.y + 200 };
        bool kontynuuj;
        if (getactivepage() == 0)
            setactivepage(1);
        else
            setactivepage(0);
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, RED);
        fillpoly(6, okienko);
        setcolor(WHITE);
        strcpy(zakonczenie, "Potrzebowales na to: ");
        strcat(zakonczenie, aktualny_czas);
        outtextxy(srodek.x - 50, srodek.y - 100,"GRATULACJE!!!");
        outtextxy(srodek.x - 35, srodek.y - 75,"Wygrales.");
        outtextxy(srodek.x - 80, srodek.y - 50, zakonczenie);
        outtextxy(srodek.x - 175, srodek.y - 25,"Sprobuj jeszcze raz na wyzszym poziomie trudnosci! :)");
        setcolor(BLACK);
        rectangle(srodek.x - 50, srodek.y + 50, srodek.x + 50, srodek.y + 100);
        setfillstyle(SOLID_FILL, BLACK);
        floodfill(srodek.x - 49, srodek.y + 51, BLACK);
        setcolor(WHITE);
        outtextxy(srodek.x - 28, srodek.y + 70, "Zakoncz");
        do {
            if (ismouseclick(WM_LBUTTONDOWN)) {
                getmouseclick(WM_LBUTTONDOWN, myszkax, myszkay);
                if (myszkax >= srodek.x - 50 && myszkax <= srodek.x + 50 && myszkay >= srodek.y + 50 && myszkay <= srodek.y + 100)
                    kontynuuj = true;
        }} while (kontynuuj == false);
}

int wczytaj_z_pliku(time_t *zapisy) { //funkcja wczytujaca gre z pliku
    FILE *status;
    status = fopen("zapis.dat", "r");
    if (status == NULL) {
        int wylaczanie[12] {
        srodek.x - 300, srodek.y,
        srodek.x - 150, srodek.y - 150,
        srodek.x + 150, srodek.y - 150,
        srodek.x + 300, srodek.y,
        srodek.x + 150, srodek.y + 150,
        srodek.x - 150, srodek.y + 150 };
        int myszkax, myszkay;
        bool kontynuuj = false;
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, RED);
        fillpoly(6, wylaczanie);
        setcolor(WHITE);
        outtextxy(srodek.x - 55, srodek.y - 125,"JAMES BLAD 404");
        readimagefile("err.is", srodek.x - 150, srodek.y - 100, srodek.x + 150, srodek.y + 25);
        outtextxy(srodek.x - 100, srodek.y + 35,"Nie udalo sie wczytac gry z pliku.");
        setcolor(BLACK);
        rectangle(srodek.x - 45, srodek.y + 75, srodek.x + 45, srodek.y + 125);
        setfillstyle(SOLID_FILL, BLACK);
        floodfill(srodek.x - 44, srodek.y + 76, BLACK);
        setcolor(WHITE);
        outtextxy(srodek.x - 29, srodek.y + 95, "Kontynuuj");
        do {
            czyszczenie_bufora();
            if (ismouseclick(WM_LBUTTONDOWN)) {
                getmouseclick(WM_LBUTTONDOWN, myszkax, myszkay);
                if (myszkax >= srodek.x - 45 && myszkax <= srodek.x + 50 && myszkay >= srodek.y + 75 && myszkay <= srodek.y + 125) {
                    kontynuuj = true; }
        }} while (kontynuuj == false);
        return -1;
    }
    else {
        char wiersz[100];
        char *odczyt, *przerwa1, *przerwa2;
        float skalowanie;
        float wysokosc = getmaxheight();
        odczyt = fgets(wiersz, 100, status);
        while (odczyt != NULL) {
            if(wiersz[0] == '3') {
                poczatkowe_pole = strtof(&wiersz[2], &przerwa1);
                skalowanie = wysokosc / strtol(przerwa1, &przerwa2, 10);
                poczatkowe_pole *= skalowanie;
                *zapisy = strtol(przerwa2, NULL, 10);
                odczyt = fgets(wiersz, 100, status);
            }
            else if (wiersz[0] == '0') {
                biali[ilosc_bialych] = new obiekt(0);
                brak_pamieci(biali[ilosc_bialych]);
                biali[ilosc_bialych] -> wspolrzedne.x = strtof(&wiersz[2], &przerwa1) * skalowanie;
                biali[ilosc_bialych] -> wspolrzedne.y = strtof(przerwa1, &przerwa2) * skalowanie;
                biali[ilosc_bialych] -> kierunek.x = strtof(przerwa2, &przerwa1) * skalowanie;
                biali[ilosc_bialych] -> kierunek.y = strtof(przerwa1, &przerwa2) * skalowanie;
                biali[ilosc_bialych] -> promien = strtof(przerwa2, &przerwa1) * skalowanie;
                biali[ilosc_bialych] -> pole = strtof(przerwa1, NULL) * skalowanie;
                ilosc_bialych++;
                odczyt = fgets(wiersz, 100, status);
            }
            else if (wiersz[0] == '2') {
                niebiescy[ilosc_niebieskich] = new obiekt_niebieski;
                brak_pamieci(niebiescy[ilosc_niebieskich]);
                niebiescy[ilosc_niebieskich] -> wspolrzedne.x = strtof(&wiersz[2], &przerwa1) * skalowanie;
                niebiescy[ilosc_niebieskich] -> wspolrzedne.y = strtof(przerwa1, &przerwa2) * skalowanie;
                niebiescy[ilosc_niebieskich] -> kierunek.x = strtof(przerwa2, &przerwa1) * skalowanie;
                niebiescy[ilosc_niebieskich] -> kierunek.y = strtof(przerwa1, &przerwa2) * skalowanie;
                niebiescy[ilosc_niebieskich] -> promien = strtof(przerwa2, &przerwa1) * skalowanie;
                niebiescy[ilosc_niebieskich] -> pole = strtof(przerwa1, NULL) * skalowanie;
                ilosc_niebieskich++;
                odczyt = fgets(wiersz, 100, status);
            }
            else if (wiersz[0] == '1') {
                gracz.wspolrzedne.x = strtof(&wiersz[2], &przerwa1) * skalowanie;
                gracz.wspolrzedne.y = strtof(przerwa1, &przerwa2) * skalowanie;
                gracz.kierunek.x = strtof(przerwa2, &przerwa1) * skalowanie;
                gracz.kierunek.y = strtof(przerwa1, &przerwa2) * skalowanie;
                gracz.promien = strtof(przerwa2, &przerwa1) * skalowanie;
                gracz.pole = strtof(przerwa1, NULL) * skalowanie;
                odczyt = fgets(wiersz, 100, status);
            }
        }
        cout << "Wczytano" << endl;
        return 0;
    }
}

int wczytaj(time_t *zapisy, char *argv[], int argc) { //funkcja wczytujaca gre z bazy danych
    PGconn *stan;
    PGresult *status_tmp;
    char bufor[100];
    time_t aktualny_czas;
    char *nazwa_zapisu = argv[10];
    char *ip = argv[5];
    char *login = argv[6];
    char *haslo = argv[7];
    char *port = argv[9];
    char *dbname = argv[8];
    char polecenie[1000];
    strcpy(polecenie, "hostaddr = ");
    strcat(polecenie, ip);
    strcat(polecenie, " port = ");
    strcat(polecenie, port);
    strcat(polecenie, " dbname = ");
    strcat(polecenie, dbname);
    strcat(polecenie, " user = ");
    strcat(polecenie, login);
    strcat(polecenie, " password = ");
    strcat(polecenie, haslo);
    stan = PQconnectdb(polecenie);
    if (PQstatus(stan) != CONNECTION_OK || argc == 10) {
        int wylaczanie[12] {
        srodek.x - 300, srodek.y,
        srodek.x - 150, srodek.y - 150,
        srodek.x + 150, srodek.y - 150,
        srodek.x + 300, srodek.y,
        srodek.x + 150, srodek.y + 150,
        srodek.x - 150, srodek.y + 150 };
        int myszkax, myszkay;
        bool kontynuuj = false;
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, RED);
        fillpoly(6, wylaczanie);
        setcolor(WHITE);
        outtextxy(srodek.x - 55, srodek.y - 125,"JAMES BLAD 404");
        readimagefile("err.is", srodek.x - 150, srodek.y - 100, srodek.x + 150, srodek.y + 25);
        outtextxy(srodek.x - 130, srodek.y + 35,"Nie udalo sie wczytac gry z bazy danych.");
        setcolor(BLACK);
        rectangle(srodek.x - 45, srodek.y + 75, srodek.x + 45, srodek.y + 125);
        setfillstyle(SOLID_FILL, BLACK);
        floodfill(srodek.x - 44, srodek.y + 76, BLACK);
        setcolor(WHITE);
        outtextxy(srodek.x - 29, srodek.y + 95, "Kontynuuj");
        do {
            czyszczenie_bufora();
            if (ismouseclick(WM_LBUTTONDOWN)) {
                getmouseclick(WM_LBUTTONDOWN, myszkax, myszkay);
                if (myszkax >= srodek.x - 45 && myszkax <= srodek.x + 50 && myszkay >= srodek.y + 75 && myszkay <= srodek.y + 125) {
                    kontynuuj = true; }
        }} while (kontynuuj == false);
        return -1;
    }
    else {
        int biali_i = 0;
        int niebiescy_i = 0;
        float skalowanie;
        float wysokosc = getmaxheight();
        char polecenie_SQL[1000] = "SELECT * FROM ";
        //char *nazwa_zapisu = "g011115_010432";
        strcat(polecenie_SQL, nazwa_zapisu);
        strcat(polecenie_SQL, ";");
        //cout << polecenie_SQL << endl;
        status_tmp = PQexec(stan, polecenie_SQL);
        //cout << PQresultErrorMessage(status_tmp);
        skalowanie = wysokosc / strtof(PQgetvalue(status_tmp, PQntuples(status_tmp) - 1, 1), NULL);
        for (int i = 0; i < PQntuples(status_tmp); i++) {
            if (strcmp(PQgetvalue(status_tmp, i, 6), "3") == 0) {
                //cout << "Wczytuje 3 " << i << endl;
                *zapisy = strtol(PQgetvalue(status_tmp, i, 5), NULL, 10);
                poczatkowe_pole = strtof(PQgetvalue(status_tmp, i, 0), NULL) * skalowanie;
            }
            else if (strcmp(PQgetvalue(status_tmp, i, 6), "2") == 0) {
                //cout << "Wczytuje 2 " << i << endl;
                niebiescy[ilosc_niebieskich] = new obiekt_niebieski();
                brak_pamieci(niebiescy[ilosc_niebieskich]);
                niebiescy[ilosc_niebieskich] -> wspolrzedne.x = strtof(PQgetvalue(status_tmp, i, 0), NULL) * skalowanie;
                niebiescy[ilosc_niebieskich] -> wspolrzedne.y = strtof(PQgetvalue(status_tmp, i, 1), NULL) * skalowanie;
                niebiescy[ilosc_niebieskich] -> kierunek.x = strtof(PQgetvalue(status_tmp, i, 2), NULL) * skalowanie;
                niebiescy[ilosc_niebieskich] -> kierunek.y = strtof(PQgetvalue(status_tmp, i, 3), NULL) * skalowanie;
                niebiescy[ilosc_niebieskich] -> promien = strtof(PQgetvalue(status_tmp, i, 4), NULL) * skalowanie;
                niebiescy[ilosc_niebieskich] -> pole = strtof(PQgetvalue(status_tmp, i, 5), NULL) * skalowanie;
                niebiescy[ilosc_niebieskich] -> rodzaj = 2;
                ilosc_niebieskich++;
            }
            else if (strcmp(PQgetvalue(status_tmp, i, 6), "0") == 0) {
                //cout << "Wczytuje 0 " << i << endl;
                biali[ilosc_bialych] = new obiekt(0);
                brak_pamieci(biali[ilosc_bialych]);
                biali[ilosc_bialych] -> wspolrzedne.x = strtof(PQgetvalue(status_tmp, i, 0), NULL) * skalowanie;
                biali[ilosc_bialych] -> wspolrzedne.y = strtof(PQgetvalue(status_tmp, i, 1), NULL) * skalowanie;
                biali[ilosc_bialych] -> kierunek.x = strtof(PQgetvalue(status_tmp, i, 2), NULL) * skalowanie;
                biali[ilosc_bialych] -> kierunek.y = strtof(PQgetvalue(status_tmp, i, 3), NULL) * skalowanie;
                biali[ilosc_bialych] -> promien = strtof(PQgetvalue(status_tmp, i, 4), NULL) * skalowanie;
                biali[ilosc_bialych] -> pole = strtof(PQgetvalue(status_tmp, i, 5), NULL) * skalowanie;
                biali[ilosc_bialych] -> rodzaj = 0;
                ilosc_bialych++;
            }
            else if (strcmp(PQgetvalue(status_tmp, i, 6), "1") == 0){
                //cout << "Wczytuje 1 " << i << endl;
                gracz.wspolrzedne.x = strtof(PQgetvalue(status_tmp, i, 0), NULL) * skalowanie;
                gracz.wspolrzedne.y = strtof(PQgetvalue(status_tmp, i, 1), NULL) * skalowanie;
                gracz.kierunek.x = strtof(PQgetvalue(status_tmp, i, 2), NULL) * skalowanie;
                gracz.kierunek.y = strtof(PQgetvalue(status_tmp, i, 3), NULL) * skalowanie;
                gracz.promien = strtof(PQgetvalue(status_tmp, i, 4), NULL) * skalowanie;
                gracz.pole = strtof(PQgetvalue(status_tmp, i, 5), NULL) * skalowanie;
                gracz.rodzaj = 1;
            }
        }
        cout << "Wczytano" << endl;
        PQclear(status_tmp);
        return 0;
    }
}

void za_mala_rozdzielczosc() { //funkcja informujaca o zbyt malej rozdzielczosci ekranu
        int wylaczanie[12] {
        srodek.x - 300, srodek.y,
        srodek.x - 150, srodek.y - 150,
        srodek.x + 150, srodek.y - 150,
        srodek.x + 300, srodek.y,
        srodek.x + 150, srodek.y + 150,
        srodek.x - 150, srodek.y + 150 };
        int myszkax, myszkay;
        bool kontynuuj = false;
        setcolor(WHITE);
        setfillstyle(SOLID_FILL, RED);
        fillpoly(6, wylaczanie);
        setcolor(WHITE);
        outtextxy(srodek.x - 55, srodek.y - 125,"JAMES BLAD 404");
        readimagefile("err.is", srodek.x - 150, srodek.y - 100, srodek.x + 150, srodek.y + 25);
        outtextxy(srodek.x - 100, srodek.y + 35,"Za mala rozdzielczosc ekranu!!!");
        outtextxy(srodek.x - 97, srodek.y + 50,"Wymagane 1024x786 pikseli!");
        setcolor(BLACK);
        rectangle(srodek.x - 45, srodek.y + 75, srodek.x + 45, srodek.y + 125);
        setfillstyle(SOLID_FILL, BLACK);
        floodfill(srodek.x - 44, srodek.y + 76, BLACK);
        setcolor(WHITE);
        outtextxy(srodek.x - 29, srodek.y + 95, "Kontynuuj");
        do {
            czyszczenie_bufora();
            if (ismouseclick(WM_LBUTTONDOWN)) {
                getmouseclick(WM_LBUTTONDOWN, myszkax, myszkay);
                if (myszkax >= srodek.x - 45 && myszkax <= srodek.x + 50 && myszkay >= srodek.y + 75 && myszkay <= srodek.y + 125) {
                    kontynuuj = true; }
        }} while (kontynuuj == false);
}

void rysuj(int *ilosc_bialych, int *ilosc_niebieskich) { //funkcja rysujaca wszystkie obiekty
    for (int i=0; i < *ilosc_bialych; i++) {
        rysowanie(biali[i]);
    }
    for (int i=0; i < *ilosc_niebieskich; i++) {
        rysowanie(niebiescy[i]);
    }
    rysowanie(&gracz);
}

int main(int argc, char *argv[]) {
    if (argc <= 4) {
        ShellExecuteA(NULL, "open", "GUI.exe", NULL, NULL, SW_NORMAL);
        return 0;
    }
    do_zakresu_projektu wykres[2] {
        {szer * 0.05, wys * 0.96},
        {szer * 0.95, wys * 0.99}};
    bool wyjscie = false;
    bool kontynuuj = false;
    int predkosc_gry = 0;
    int stan_wczytywania = 0;
    int wybrana_ilosc_bialych = strtol(argv[1], NULL, 10);
    int wybrana_ilosc_niebieskich = strtol(argv[2], NULL, 10);
    int klawisz;
    time_t start, aktualny, wstrzymanie, przerwy = 0, zapisy = 0;
    char aktualny_czas[15];
    int czerwona_linia = wykres[0].x + wykres[1].x * 0.85;
    for (int i=0; i<tab_wsk_biali; i++) {
        biali[i] = NULL;
    }
    for (int i=0; i<tab_wsk_niebiescy; i++) {
        niebiescy[i] = NULL;
    }
    uruchamianie_grafiki();
    //cout << getmaxwidth() << " " << getmaxheight() << endl;
    if (getmaxwidth() < 1000 || getmaxheight() < 700) {
        za_mala_rozdzielczosc();
        return -1;
    }
    setactivepage(0);
    int myszkax, myszkay;
    if(strcmp(argv[3], "1") == 0) {
        if (strcmp(argv[4], "1") == 0 && argc > 5) {
            stan_wczytywania = wczytaj(&zapisy, argv, argc);
        }
        else {
            stan_wczytywania = wczytaj_z_pliku(&zapisy);
        }
    }
    else {
        generator_niebieskich(wybrana_ilosc_niebieskich);
        generator_bialych(wybrana_ilosc_bialych, 25 * wys / 902.4);
        wprowadzenie_poprawek();
        poczatkowe_pole -= pole_do_rozdzialu;
        zapisy = 0; }
    if (stan_wczytywania == -1) {
        return -1;
    }
    setcolor(WHITE);
    rectangle(0.05 * szer, 0.05 * wys, szer - 0.05 * szer, wys - 0.05 * wys);
    outtextxy(szer * 0.35, wys - 0.035 * wys, "Aby rozpoczac nacisnij dowolny klawisz");
    rysuj(&ilosc_bialych, &ilosc_niebieskich);
    getch();
    cout << "Gramy!!!" << endl;
    time(&start);
    while(wyjscie != true && (gracz.pole > 0.0 && gracz.pole < 0.85 * poczatkowe_pole)) {
        zmiana_strony();
        delay(predkosc_gry);
        wyswietlanie_predkosci_gry(&predkosc_gry);
        time(&aktualny);
        aktualny = aktualny - start - przerwy + zapisy;
        sprintf(aktualny_czas, "%d", aktualny);
        strcat(aktualny_czas, " s");
        setcolor(RED);
        line(czerwona_linia, wykres[0].y, czerwona_linia, wykres[1].y);
        setcolor(WHITE);
        rectangle(wykres[0].x, wykres[0].y, wykres[0].x + ((wykres[1].x - wykres[0].x) * (gracz.pole / poczatkowe_pole)), wykres[1].y);
        rectangle(0.05 * szer, 0.05 * wys, szer - 0.05 * szer, wys - 0.05 * wys);
        outtextxy(szer * 0.35, wys - 0.035 * wys, "Aby wylaczyc gre nacisnij przycisk ESC");
        outtextxy(szer * 0.43, wys * 0.02, "Pomoc -> Klawisz F1");
        outtextxy(szer * 0.05, wys * 0.02, aktualny_czas);
        if (ismouseclick(WM_LBUTTONDOWN)) {
            getmouseclick(WM_LBUTTONDOWN, myszkax, myszkay);
            tworzenie_bialych(&gracz, &ilosc_bialych, myszkax, myszkay);
        }
        ruchy(&ilosc_bialych, &ilosc_niebieskich);
        odbicia(&ilosc_bialych, &ilosc_niebieskich);
        walki(&ilosc_bialych, &ilosc_niebieskich);
        sprzatanie_bialych(&ilosc_bialych);
        sprzatanie_niebieskich(&ilosc_niebieskich);
        for (int i=0; i<ilosc_niebieskich; i++) {
            AI(niebiescy[i], &ilosc_bialych, &ilosc_niebieskich);
        }
        if (kbhit()) {
            klawisz = getch();
            if (klawisz == '1') {
                predkosc_gry = 50;
            }
            else if (klawisz == '2') {
                predkosc_gry = 25;
            }
            else if (klawisz == '3') {
                predkosc_gry = 0;
            }
            else if (klawisz == 0) {
                pomoc(&przerwy);
            }
            else if (klawisz == 27) { //Klawisz ESC
                wyjscie_menu(&przerwy, &wyjscie, &start, &zapisy, argv, argc); }
    }}
    if (gracz.pole <= 0.0 || wyjscie == true) {
        przegral(wyjscie, aktualny_czas);
    }
    else {
        wygral(aktualny_czas);
    }
}
