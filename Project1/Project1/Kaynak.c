#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include<dirent.h>

DIR* dirp;
struct dirent* directory;

struct noktalar {
    double x, y;
}nokta[100];

struct Koordinatlar
{
    double x, y;
} KoordinatBilgisi[100];

struct cember {
    double x, y;
    double r;
};


void ciz(float x, float y, float r, int a[][2], int b)//Alegro ile ilgili bütün iþlemler
{
    
    al_init_primitives_addon();
    al_init_ttf_addon();
    al_init_font_addon();
    ALLEGRO_DISPLAY* display = NULL;

    al_init();

    display = al_create_display(1000, 1000);//Konsol
    al_draw_filled_rectangle(0, 0, 1000, 1000, al_map_rgb(255, 255, 255));//Arka plan
    ALLEGRO_FONT* font24 = al_load_ttf_font("Regular.ttf", 10, 0);

    al_draw_circle(500 + (x * 20), 500 + (-y * 20), r * 20, al_map_rgb(0, 0, 0), 2);//Çember
    al_draw_line(0, 500, 1000, 500, al_map_rgb(0, 0, 0), 2);//X koordinatý
    al_draw_line(500, 0, 500, 1000, al_map_rgb(0, 0, 0), 2);//Y koordinatý


    for (int i = 0; i < 1000; i += 20)//X koordinatý noktalarý
    {
        al_draw_filled_circle(i, 500, 2.35, al_map_rgb(0, 0, 0));
        al_draw_textf(font24, al_map_rgb(0, 0, 0), i, 500, 0, "%i", -25 + i / 20);
    }

    for (int i = 0; i < 1000; i += 20)//Y koordinatý noktalarý
    {
        al_draw_filled_circle(500, i, 2.35, al_map_rgb(0, 0, 0));
        al_draw_textf(font24, al_map_rgb(0, 0, 0), 500, i, 0, "%i", -i / 20 + 25);
    }

    for (int i = 0; i < b; i++)//Dosyadaki noktalar
    {
        al_draw_filled_circle(500 + a[i][0] * 20, 500 + a[i][1] * -20, 4, al_map_rgb(255, 0, 0));
    }
    
    al_draw_filled_circle(500 + x*20, 500 + y * -20, 5, al_map_rgb(0, 0, 255), 1);//Merkez noktasý

    al_flip_display();

    system("pause");
}

double uzaklik(struct noktalar a, struct noktalar b) {//Ýki nokta arasýndaki uzaklýðý bulan fonksiyon

    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

bool cember_gecerli_mi(struct cember c, int p[][2], int n) {//Geçici çemberin geçerli olup olmadýðýný kontrol eden fonksiyon
    struct noktalar gecici2;
    struct noktalar gecici3;
    gecici3.x = c.x;
    gecici3.y = c.y;
    for (int i = 0; i < n; i++) {
        gecici2.x = p[i][0];
        gecici2.y = p[i][1];
        if (uzaklik(gecici3, gecici2) > c.r)
            return 0;
    }
    return 1;
}

struct cember iki_noktadan_cember(struct noktalar O, struct noktalar F) {
    struct cember d;
    d.x = (O.x + F.x) / 2.0;
    d.y = (O.y + F.y) / 2.0;
    d.r = uzaklik(O, F) / 2.0;
    return d;
};


struct cember uc_noktadan_cember(struct noktalar a, struct noktalar b, struct noktalar c) {
    struct cember gecici4;

    double A = a.x * (b.y - c.y) - a.y * (b.x - c.x) + b.x * c.y - c.x * b.y;
    double B = (a.x * a.x + a.y * a.y) * (c.y - b.y) + (b.x * b.x + b.y * b.y) * (a.y - c.y) + (c.x * c.x + c.y * c.y) * (b.y - a.y);
    double C = (a.x * a.x + a.y * a.y) * (b.x - c.x) + (b.x * b.x + b.y * b.y) * (c.x - a.x) + (c.x * c.x + c.y * c.y) * (a.x - b.x);
    double D = (a.x * a.x + a.y * a.y) * (c.x * b.y - b.x * c.y) + (b.x * b.x + b.y * b.y) * (a.x * c.y - c.x * a.y) + (c.x * c.x + c.y * c.y) * (b.x * a.y - a.x * b.y);
    gecici4.x = -(B / (2 * A));
    gecici4.y = -(C / (2 * A));
    gecici4.r = sqrt((B * B + C * C - 4 * A * D) / (4 * A * A));

    return gecici4;
};

struct cember cevreleyen_en_kucuk_cember(int koordinatlar[][2], int n) {

    struct cember ekc;
    ekc.x = 0;
    ekc.y = 0;
    ekc.r = 100;

    struct cember c1;
 
    if (n == 0) {
        c1.x = 0;
        c1.y = 0;
        c1.r = 0;
        return c1;
    }
    else if (n == 1) {
        c1.x = koordinatlar[0][0];
        c1.y = koordinatlar[0][1];
        c1.r = 0;
        return c1;
    }

    for (int i = 0; i < n; i++) {
        nokta[i].x = koordinatlar[i][0];
        nokta[i].y = koordinatlar[i][1];
    }

    struct cember gecici;

    for (int O = 0; O < n; O++) {
        for (int F = O + 1; F < n; F++) {
            gecici = iki_noktadan_cember(nokta[O], nokta[F]);

            if (gecici.r < ekc.r && cember_gecerli_mi(gecici, koordinatlar, n)) {
                ekc = gecici;
            }
        }
    }
    
    for (int H = 0; H < n; H++) {
        for (int E = H + 1; E < n; E++) {
            for (int P = E + 1; P < n; P++) {

                gecici = uc_noktadan_cember(nokta[H], nokta[E], nokta[P]);

                if (gecici.r < ekc.r && cember_gecerli_mi(gecici, koordinatlar, n)) {
                    ekc = gecici;
                }

            }
        }
    }
    return ekc;
};

int main()
{
    dirp = opendir(".");
    int a = 0, i;
    char dizi[200], dosya_adi[20];
    FILE* dosya;
    char* ptr;
    int nokta = 0, koordinat_adeti = 0;

    if (dirp != NULL)
    {
        while ((directory = readdir(dirp)) != NULL)
        {
            int len = strlen(directory->d_name);
            if (strncmp(directory->d_name + len - 4, ".txt", 4) == 0)
            {
                i = 0;
                strcpy(dosya_adi, directory->d_name);
                dosya = fopen(directory->d_name, "r");


                while (!feof(dosya))
                {
                    if (fgets(dizi, 200, dosya))
                    {
                        ptr = strtok(dizi, " ");

                        int t = 0;
                        while (ptr != NULL)
                        {
                            if (t == 0)
                            {
                                KoordinatBilgisi[nokta].x = atof(ptr);
                                koordinat_adeti++;
                            }
                            if (t == 1)
                            {
                                KoordinatBilgisi[nokta].y = atof(ptr);
                            }
                            t++;
                            ptr = strtok(NULL, " ");
                        }
                        nokta++;

                    }
                }

                nokta = 0;
            }
        }
    }
    int Koordinatlar[100][2];

    for (int i = 0; i < koordinat_adeti; i++) {
        Koordinatlar[i][0] =(int)KoordinatBilgisi[i].x;
        Koordinatlar[i][1] =(int)KoordinatBilgisi[i].y;
    }
    struct cember ekc = cevreleyen_en_kucuk_cember(Koordinatlar, koordinat_adeti);
    printf("Nokta Sayisi : %d\n", koordinat_adeti);
    printf("Merkezin koordinati : (%.3f,%.3f)\nYaricap : %.3f\n", ekc.x, ekc.y, ekc.r);
 
      ciz(ekc.x, ekc.y, ekc.r, Koordinatlar, koordinat_adeti);

    return 0;
}

