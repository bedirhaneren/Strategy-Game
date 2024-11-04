#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#define GRID_COLS 20
#define GRID_ROWS 20
#define SQUARE_SIZE 42

struct BellekYapisi
{
    char *veri;
    size_t boyut;
};

typedef struct
{
    char bonus_turu[20];
    char etkiledigi_birim[20];
    float bonus_degeri;
} Kahraman;

typedef struct
{
    float birlik_sayisi=0;
    float saldiri=0;
    float savunma=0;
    float saglik=0;
    float kritik_sans=0;
} Birlik;

typedef struct
{
    float etki_deger;
    char etki_turu[50];
    char etkiledigi_birim[50];
} Creatures;

typedef struct
{
    float deger;
    char etkiledigi_birim[20];
} seviyeler_degerleri;
typedef struct
{
    seviyeler_degerleri seviye_1;
    seviyeler_degerleri seviye_2;
    seviyeler_degerleri seviye_3;

} savunma_ustaligi;

typedef struct
{
    seviyeler_degerleri seviye_1;
    seviyeler_degerleri seviye_2;
    seviyeler_degerleri seviye_3;

} saldiri_gelistirmesi;

typedef struct
{
    seviyeler_degerleri seviye_1;
    seviyeler_degerleri seviye_2;
    seviyeler_degerleri seviye_3;

} elit_egitim;

typedef struct
{
    seviyeler_degerleri seviye_1;
    seviyeler_degerleri seviye_2;
    seviyeler_degerleri seviye_3;

} kusatma_ustaligi;

typedef struct
{
    Birlik piyadeler;
    Birlik okcular;
    Birlik suvariler;
    Birlik kusatma_makineleri;
    Kahraman Alparslan;
    Kahraman Fatih_Sultan_Mehmet;
    Kahraman Mete_Han;
    Kahraman Yavuz_Sultan_Selim;
    Kahraman Tugrul_Bey;
    Creatures Ejderha;
    Creatures Agri_Dagi_Devleri;
    Creatures Tepegoz;
    Creatures Karakurt;
    Creatures Samur;
} InsanImparatorlugu;

typedef struct
{
    Birlik ork_dovusculeri;
    Birlik mizrakcilar;
    Birlik varg_binicileri;
    Birlik troller;
    Kahraman Goruk_Vahsi;
    Kahraman Thruk_Kemikkiran;
    Kahraman Vrog_Kafakiran;
    Kahraman Ugar_Zalim;
    Creatures Kara_Troll;
    Creatures Golge_Kurtlari;
    Creatures Camur_Devleri;
    Creatures Ates_Iblisi;
    Creatures Makrog_Savas_Beyi;
    Creatures Buz_Devleri;
} OrkLegi;


int JsonAyristir(const char *birimAdresi, const char *ArananOzellik, float *deger)
{
    char *sonuc = strstr(birimAdresi, ArananOzellik);

    if (sonuc == NULL)
    {
        printf("Aranan ozellik bulunamadi: %s!\n", ArananOzellik);
        return 0;
    }

    sonuc = strchr(sonuc, ':');
    if (sonuc == NULL)
    {
        printf("Aranan ozellik bulundu fakat degeri bulunamadi: %s!\n", ArananOzellik);
        return 0;
    }

    *deger = atoi(sonuc + 1);
    return 1;
}

int BirlikleriAyristir(const char *jsonVerisi, Birlik *birlikTipi, const char *birlikIsmi)
{
    char *birimAdresi = strstr(jsonVerisi, birlikIsmi);
    if (birimAdresi == NULL)
    {
        printf("Birlik tipi JSON dosyaniz icinde bulunamadi: %s!\n", birlikIsmi);
        return 0;
    }
    else
    {
        JsonAyristir(birimAdresi, "\"saldiri\"", &birlikTipi->saldiri);
        JsonAyristir(birimAdresi, "\"savunma\"", &birlikTipi->savunma);
        JsonAyristir(birimAdresi, "\"saglik\"", &birlikTipi->saglik);
        JsonAyristir(birimAdresi, "\"kritik_sans\"", &birlikTipi->kritik_sans);
        return 1;
    }
}

void birlikBilgiBastir(const char *birlikIsmi, Birlik birlikTipi)
{
    printf("%s: Saldiri: %f, Savunma: %f, Saglik: %f, Kritik Sans: %f\n",
           birlikIsmi, birlikTipi.saldiri, birlikTipi.savunma,
           birlikTipi.saglik, birlikTipi.kritik_sans);
}

char* dosyaOku(char *dosyaIsmi)
{
    FILE *dosya = fopen(dosyaIsmi, "r");

    if (dosya == NULL)
    {
        printf("Dosya icerigi acilamadi! ");
        return NULL;
    }
    fseek(dosya, 0, SEEK_END);
    long dosyaUzunlugu = ftell(dosya);
    rewind(dosya);

    char *dosyaicerigi = (char*)malloc(dosyaUzunlugu + 1);

    if (dosyaicerigi)
    {
        fread(dosyaicerigi, 1, dosyaUzunlugu, dosya);
        dosyaicerigi[dosyaUzunlugu] = '\0';
    }
    else
    {
        printf("Dosya icin malloc ile yer ayirmada bir sorun olustu.");
        return NULL;
    }

    fclose(dosya);
    return dosyaicerigi;
}

int KahramanlariAyristir(const char *HeroJsonVerisi, const char *KahramanIsmi, Kahraman *kahramanTipi)
{
    char *KahramanAdresi = strstr(HeroJsonVerisi, KahramanIsmi);

    if (KahramanAdresi == NULL)
    {
        printf("Kahraman tipi JSON dosyaniz bulunamadi: %s\n", KahramanIsmi);
        return 0;
    }
    char *sonuc = strstr(KahramanAdresi, "\"bonus_turu\"");

    if (sonuc == NULL)
    {
        printf("Aranan ozellik bulunamadi: bonus_turu\n");
        return 0;
    }

    sonuc = strchr(sonuc, ':');
    if (sonuc == NULL)
    {
        printf("Aranan ozellik bulundu fakat degeri bulunamadi: bonus_turu\n");
        return 0;
    }

    sscanf(sonuc + 2, "\"%[^\"]\"", kahramanTipi->bonus_turu);

    char *aciklamaAdres = strstr(KahramanAdresi, "\"aciklama\"");
    if (aciklamaAdres == NULL)
    {
        strcpy(kahramanTipi->etkiledigi_birim, "bilinmiyor");
    }
    else
    {
        char aciklama[256];
        sscanf(aciklamaAdres, "\"aciklama\": \"%[^\"]\"", aciklama);

        if (strstr(aciklama, "piyadeler") != NULL)
        {
            strcpy(kahramanTipi->etkiledigi_birim, "piyadeler");
        }
        else if (strstr(aciklama, "suvariler") != NULL)
        {
            strcpy(kahramanTipi->etkiledigi_birim, "suvariler");
        }
        else if (strstr(aciklama, "okcular") != NULL)
        {
            strcpy(kahramanTipi->etkiledigi_birim, "okcular");
        }
        else if (strstr(aciklama, "kusatma_makineleri") != NULL)
        {
            strcpy(kahramanTipi->etkiledigi_birim, "kusatma_makineleri");
        }
        else if (strstr(aciklama, "troller")!=NULL)
        {
            strcpy(kahramanTipi->etkiledigi_birim, "troller");
        }
        else if (strstr(aciklama, "varg_binicileri")!=NULL)
        {
            strcpy(kahramanTipi->etkiledigi_birim, "varg_binicileri");
        }
        else if (strstr(aciklama, "ork_dovusculeri")!=NULL)
        {
            strcpy(kahramanTipi->etkiledigi_birim, "ork_dovusculeri");
        }
        else if (strstr(aciklama, "mizrakcilar")!=NULL)
        {
            strcpy(kahramanTipi->etkiledigi_birim, "mizrakcilar");
        }
        else if (strstr(aciklama, "tum_birimler")!=NULL)
        {
            strcpy(kahramanTipi->etkiledigi_birim, "tum_birimler");
        }
        else
        {
            strcpy(kahramanTipi->etkiledigi_birim, "bilinmiyor");
        }
    }
    char *bonusAdresi = strstr(KahramanAdresi, "\"bonus_degeri\"");
    if (bonusAdresi)
    {
        sscanf(bonusAdresi, "\"bonus_degeri\": \"%f\"", &(kahramanTipi->bonus_degeri));
    }
    else
    {
        printf("Bonus degeri bulunamadi: %s\n", KahramanIsmi);
        return 0;
    }
    return 1;
}

void KahramanBilgiBastir(const char *kahramanIsmi, Kahraman kahramanTipi)
{
    printf("%s: Bonus Turu: %s, Etkiledigi Birim: %s, Bonus Degeri: %f\n",kahramanIsmi, kahramanTipi.bonus_turu, kahramanTipi.etkiledigi_birim, kahramanTipi.bonus_degeri);
}
int CreaturesAyristir(const char *AdresBilgisi, const char *creaturesAd, Creatures *creaturesTipi)
{
    char *creatureStart = strstr(AdresBilgisi, creaturesAd);
    if (creatureStart == NULL)
    {
        printf("Creatures tipi JSON dosyasinda bulunamadi.\n");
        return 0;
    }
    char *etki_degerAdres = strstr(creatureStart, "\"etki_degeri\"");
    if (etki_degerAdres == NULL)
    {
        printf("Etki degeri bulunamadi.\n");
        return 0;
    }
    else
    {
        sscanf(etki_degerAdres, "\"etki_degeri\": \"%f\"", &(creaturesTipi->etki_deger));
    }
    char *etki_turuAdres = strstr(creatureStart, "\"etki_turu\"");
    if (etki_turuAdres == NULL)
    {
        printf("Etki turu bulunamadi.\n");
        return 0;
    }
    else
    {
        sscanf(etki_turuAdres, "\"etki_turu\": \"%[^\"]\"", creaturesTipi->etki_turu);
    }

    char *aciklamaAdres = strstr(creatureStart, "\"aciklama\"");
    if (aciklamaAdres == NULL)
    {
        printf("Creatures Aciklamasi Bulunamadi!\n");
        strcpy(creaturesTipi->etkiledigi_birim, "bilinmiyor");
    }
    else
    {
        char aciklama[256];
        sscanf(aciklamaAdres, "\"aciklama\": \"%[^\"]\"", aciklama);

        if (strstr(aciklama, "piyadeler") != NULL)
        {
            strcpy(creaturesTipi->etkiledigi_birim, "piyade");
        }
        else if (strstr(aciklama, "suvariler") != NULL)
        {
            strcpy(creaturesTipi->etkiledigi_birim, "suvari");
        }
        else if (strstr(aciklama, "okcular") != NULL)
        {
            strcpy(creaturesTipi->etkiledigi_birim, "okcu");
        }
        else if (strstr(aciklama, "kusatma_makineleri") != NULL)
        {
            strcpy(creaturesTipi->etkiledigi_birim, "kusatma_makineleri");
        }
        else if (strstr(aciklama, "troller")!=NULL)
        {
            strcpy(creaturesTipi->etkiledigi_birim, "troller");
        }
        else if (strstr(aciklama, "varg_binicileri")!=NULL)
        {
            strcpy(creaturesTipi->etkiledigi_birim, "varg_binicileri");
        }
        else if (strstr(aciklama, "ork_dovusculeri")!=NULL)
        {
            strcpy(creaturesTipi->etkiledigi_birim, "ork_dovusculeri");
        }
        else if (strstr(aciklama, "mizrakcilar")!=NULL)
        {
            strcpy(creaturesTipi->etkiledigi_birim, "mizrakcilar");
        }
        else if (strstr(aciklama, "tum_birimler")!=NULL)
        {
            strcpy(creaturesTipi->etkiledigi_birim, "tum_birimler");
        }

        else
        {
            strcpy(creaturesTipi->etkiledigi_birim, "bilinmiyor");
        }
    }
    return 1;
}

void CreatureBilgiBastir(const char *CreaturesIsim, Creatures CreaturesTipi)
{
    printf("%s: Etki turu: %s, Etki degeri %f\n etkiledigi birim :%s\n",CreaturesIsim, CreaturesTipi.etki_turu, CreaturesTipi.etki_deger, CreaturesTipi.etkiledigi_birim);
}

int ResearchAyristir(const char *AdresBilgisi, const char *SeviyeIsmi, seviyeler_degerleri *seviyeTipi, const char *SeviyeSayisi)
{
    char *Seviye_konumu = strstr(AdresBilgisi, SeviyeIsmi);
    if(Seviye_konumu == NULL)
    {
        printf("Aranan seviyede seviye bulunamamaktadir.");
        return 0 ;
    }


    char *Seviye_sayisi_konumu = strstr(Seviye_konumu, SeviyeSayisi);
    if(Seviye_sayisi_konumu == NULL)
    {
        printf("Bu seviye sayisi bulunamamktadir.");
        return 0 ;
    }

    char *aciklamaAdres = strstr(Seviye_sayisi_konumu, "\"aciklama\"");
    if (aciklamaAdres == NULL)
    {
        strcpy(seviyeTipi->etkiledigi_birim, "bilinmiyor");
    }
    else
    {
        char aciklama[256];
        sscanf(aciklamaAdres, "\"aciklama\": \"%[^\"]\"", aciklama);

        if (strstr(aciklama, "piyadeler") != NULL)
        {
            strcpy(seviyeTipi->etkiledigi_birim, "piyade");
        }
        else if (strstr(aciklama, "suvariler") != NULL)
        {
            strcpy(seviyeTipi->etkiledigi_birim, "suvari");
        }
        else if (strstr(aciklama, "okcular") != NULL)
        {
            strcpy(seviyeTipi->etkiledigi_birim, "okcu");
        }
        else if (strstr(aciklama, "kusatma_makineleri") != NULL)
        {
            strcpy(seviyeTipi->etkiledigi_birim, "kusatma_makineleri");
        }
        else if (strstr(aciklama, "troller")!=NULL)
        {
            strcpy(seviyeTipi->etkiledigi_birim, "troller");
        }
        else if (strstr(aciklama, "varg_binicileri")!=NULL)
        {
            strcpy(seviyeTipi->etkiledigi_birim, "varg_binicileri");
        }
        else if (strstr(aciklama, "ork_dovusculeri")!=NULL)
        {
            strcpy(seviyeTipi->etkiledigi_birim, "ork_dovusculeri");
        }
        else if (strstr(aciklama, "mizrakcilar")!=NULL)
        {
            strcpy(seviyeTipi->etkiledigi_birim, "mizrakcilar");
        }
        else if (strstr(aciklama, "tum_birimler")!=NULL)
        {
            strcpy(seviyeTipi->etkiledigi_birim, "tum_birimler");
        }

        else
        {
            strcpy(seviyeTipi->etkiledigi_birim, "bilinmiyor");
        }
    }

    char *deger_adres = strstr(Seviye_sayisi_konumu, "\"deger\"");
    if(deger_adres == NULL)
    {
        printf("Aranan seviyede deger bulunamamaktadir.");
        return 0 ;
    }
    else
    {
        sscanf(deger_adres, "\"deger\": \"%f\"", &(seviyeTipi->deger));
    }

}

void ResearchBilgiBastir(const char *SeviyeTipi, seviyeler_degerleri SeviyeDegiskenleri)
{
    printf("%s: degeri %f, etkiledigi birim: %s\n", SeviyeTipi, SeviyeDegiskenleri.deger, SeviyeDegiskenleri.etkiledigi_birim);
}

int baseSaldiriGucu(void *birlikismi, float birimSaldiriGucu)
{
    Birlik *birlik = (Birlik *)birlikismi;

    if(birlik->birlik_sayisi<=0)
    {
        return 0;
    }

    float saldiri_gucu = birlik->birlik_sayisi * birimSaldiriGucu;
    return saldiri_gucu;
}
int baseSavunmaGucu(void *birlikismi2, float birimSavunmaGucu)
{
    Birlik *birlik2 = (Birlik *)birlikismi2;

    if(birlik2->birlik_sayisi<=0)
    {
        return 0;
    }

    float toplamSavunmaGucu = birlik2->birlik_sayisi * birimSavunmaGucu;
    return toplamSavunmaGucu;
}
int baseSaglikaGucu(void *birlikismi3, float birimSaglikGucu)
{
    Birlik *birlik3 = (Birlik *)birlikismi3;

    if(birlik3->birlik_sayisi<=0)
    {
        return 0;
    }

    float toplamSaglikGucu = birlik3->birlik_sayisi * birimSaglikGucu;
    return toplamSaglikGucu;
}
static size_t BellekYazmaGeriCagirma(void *icerik, size_t boyut, size_t adet, void *kullanici_verisi)
{
    size_t gercek_boyut = boyut * adet;
    struct BellekYapisi *bellek = (struct BellekYapisi *)kullanici_verisi;

    char *yeniadres = (char *)realloc(bellek->veri, bellek->boyut + gercek_boyut + 1);
    if(yeniadres == NULL)
    {
        printf("Yeterli bellek yok\n");
        return 0;
    }

    bellek->veri = yeniadres;
    memcpy(&(bellek->veri[bellek->boyut]), icerik, gercek_boyut);
    bellek->boyut += gercek_boyut;
    bellek->veri[bellek->boyut] = 0;

    return gercek_boyut;
}
void KareCiz(SDL_Renderer *renderer, int x, int y, int boyut, SDL_Color renk)
{
    SDL_SetRenderDrawColor(renderer, renk.r, renk.g, renk.b, renk.a);
    SDL_Rect konum = {x, y, boyut, boyut};
    SDL_RenderDrawRect(renderer, &konum);
}
void KareleriDoldur(SDL_Renderer *renderer, int x, int y, int boyut, SDL_Color renk)
{
    SDL_SetRenderDrawColor(renderer, renk.r, renk.g, renk.b, renk.a);
    SDL_Rect konum = {x, y, boyut, boyut};
    SDL_RenderFillRect(renderer, &konum);
}

int main(int argc, char *argv[])
{
    char *unitJsonVerisi = dosyaOku("files/unit_types.json");

    if (unitJsonVerisi == NULL)
    {
        printf("Dosya icerigi acilamadi veya bulunamadi!\n");
        return 1;
    }

    InsanImparatorlugu insanImparatorlugu;
    OrkLegi orkLegi;

    // printf("Insan Imparatorlugu:\n");
    if (BirlikleriAyristir(unitJsonVerisi, &insanImparatorlugu.piyadeler, "piyadeler"))
    {
        // birlikBilgiBastir("Piyadeler", insanImparatorlugu.piyadeler);
    }

    if (BirlikleriAyristir(unitJsonVerisi, &insanImparatorlugu.okcular, "okcular"))
    {
        //birlikBilgiBastir("Okcular", insanImparatorlugu.okcular);
    }

    if (BirlikleriAyristir(unitJsonVerisi, &insanImparatorlugu.suvariler, "suvariler"))
    {
        // birlikBilgiBastir("Suvariler", insanImparatorlugu.suvariler);
    }

    if (BirlikleriAyristir(unitJsonVerisi, &insanImparatorlugu.kusatma_makineleri, "kusatma_makineleri"))
    {
        //birlikBilgiBastir("Kusatma Makineleri", insanImparatorlugu.kusatma_makineleri);
    }

    //  printf("\nOrk Legi:\n");
    if (BirlikleriAyristir(unitJsonVerisi, &orkLegi.ork_dovusculeri, "ork_dovusculeri"))
    {
        //birlikBilgiBastir("Ork Dovusculeri", orkLegi.ork_dovusculeri);
    }

    if (BirlikleriAyristir(unitJsonVerisi, &orkLegi.mizrakcilar, "mizrakcilar"))
    {
        //birlikBilgiBastir("Mizrakcilar", orkLegi.mizrakcilar);
    }

    if (BirlikleriAyristir(unitJsonVerisi, &orkLegi.varg_binicileri, "varg_binicileri"))
    {
        //birlikBilgiBastir("Varg Binicileri", orkLegi.varg_binicileri);
    }

    if (BirlikleriAyristir(unitJsonVerisi, &orkLegi.troller, "troller"))
    {
        //birlikBilgiBastir("Troller", orkLegi.troller);
    }


    char *HeroJsonVerisi = dosyaOku("files/heroes.json");

    if (HeroJsonVerisi == NULL)
    {
        printf("Heroes.json acilamadi!\n");
        return 1;
    }
    // printf("\n\n");
    //printf("Insanoglu imparatorlugu kahramanlari:\n");

    if (KahramanlariAyristir(HeroJsonVerisi, "Alparslan", &insanImparatorlugu.Alparslan))
    {

        // KahramanBilgiBastir("Alparslan", insanImparatorlugu.Alparslan);
    }

    if (KahramanlariAyristir(HeroJsonVerisi, "Fatih_Sultan_Mehmet", &insanImparatorlugu.Fatih_Sultan_Mehmet))
    {
        //KahramanBilgiBastir("Fatih_Sultan_Mehmet", insanImparatorlugu.Fatih_Sultan_Mehmet);
    }

    if (KahramanlariAyristir(HeroJsonVerisi, "Mete_Han", &insanImparatorlugu.Mete_Han))
    {
        //KahramanBilgiBastir("Mete-Han", insanImparatorlugu.Mete_Han);
    }

    if (KahramanlariAyristir(HeroJsonVerisi, "Yavuz_Sultan_Selim", &insanImparatorlugu.Yavuz_Sultan_Selim))
    {
        //KahramanBilgiBastir("Yavuz_Sultan_Selim", insanImparatorlugu.Yavuz_Sultan_Selim);
    }

    if (KahramanlariAyristir(HeroJsonVerisi, "Tugrul_Bey", &insanImparatorlugu.Tugrul_Bey))
    {
        //KahramanBilgiBastir("Tugrul_Bey", insanImparatorlugu.Tugrul_Bey);
    }
    //printf("\n\n");
    //printf("Org legi kahramanlari:\n");

    if (KahramanlariAyristir(HeroJsonVerisi, "Goruk_Vahsi", &orkLegi.Goruk_Vahsi))
    {

//        KahramanBilgiBastir("Goruk_Vahsi", orkLegi.Goruk_Vahsi);
    }
    if (KahramanlariAyristir(HeroJsonVerisi, "Thruk_Kemikkiran", &orkLegi.Thruk_Kemikkiran))
    {

        //      KahramanBilgiBastir("Thruk_Kemikkiran", orkLegi.Thruk_Kemikkiran);
    }
    if (KahramanlariAyristir(HeroJsonVerisi, "Vrog_Kafakiran", &orkLegi.Vrog_Kafakiran))
    {

        //    KahramanBilgiBastir("Vrog_Kafakiran", orkLegi.Vrog_Kafakiran);
    }
    if (KahramanlariAyristir(HeroJsonVerisi, "Ugar_Zalim", &orkLegi.Ugar_Zalim))
    {

        //  KahramanBilgiBastir("Ugar_Zalim", orkLegi.Ugar_Zalim);
    }

    char *CreaturesJsonVerisi = dosyaOku("files/creatures.json");

    if (CreaturesJsonVerisi == NULL)
    {
        printf("Dosya icerigi acilamadi veya bulunamadi!\n");
        return 1;
    }

    //printf("\n\n");
    //printf("Insanoglu imparatorlugu kahramanlari:\n");


    if (CreaturesAyristir(CreaturesJsonVerisi, "Ejderha", &insanImparatorlugu.Ejderha))
    {
        //  CreatureBilgiBastir("Ejderha", insanImparatorlugu.Ejderha);
    }
    if (CreaturesAyristir(CreaturesJsonVerisi, "Agri_Dagi_Devleri", &insanImparatorlugu.Agri_Dagi_Devleri))
    {
        //CreatureBilgiBastir("Agri_Dagi_Devleri", insanImparatorlugu.Agri_Dagi_Devleri);
    }
    if (CreaturesAyristir(CreaturesJsonVerisi, "Tepegoz", &insanImparatorlugu.Tepegoz))
    {
        //CreatureBilgiBastir("Tepegoz", insanImparatorlugu.Tepegoz);
    }
    if (CreaturesAyristir(CreaturesJsonVerisi, "Karakurt", &insanImparatorlugu.Karakurt))
    {
        //    CreatureBilgiBastir("Karakurt", insanImparatorlugu.Karakurt);
    }
    if (CreaturesAyristir(CreaturesJsonVerisi, "Samur", &insanImparatorlugu.Samur))
    {
        //  CreatureBilgiBastir("Samur", insanImparatorlugu.Samur);
    }

    //printf("\n\n");
    //printf("Org legi kahramanlari:\n");

    if (CreaturesAyristir(CreaturesJsonVerisi, "Kara_Troll", &orkLegi.Kara_Troll))
    {
        //  CreatureBilgiBastir("Kara_Troll", orkLegi.Kara_Troll);
    }
    if (CreaturesAyristir(CreaturesJsonVerisi, "Golge_Kurtlari", &orkLegi.Golge_Kurtlari))
    {
        //CreatureBilgiBastir("Golge_Kurtlari", orkLegi.Golge_Kurtlari);
    }
    if (CreaturesAyristir(CreaturesJsonVerisi, "Camur_Devleri", &orkLegi.Camur_Devleri))
    {
        //CreatureBilgiBastir("Camur_Devleri", orkLegi.Camur_Devleri);
    }
    if (CreaturesAyristir(CreaturesJsonVerisi, "Ates_Iblisi", &orkLegi.Ates_Iblisi))
    {
        //CreatureBilgiBastir("Ates_Iblisi", orkLegi.Ates_Iblisi);
    }
    if (CreaturesAyristir(CreaturesJsonVerisi, "Makrog_Savas_Beyi", &orkLegi.Makrog_Savas_Beyi))
    {
        //CreatureBilgiBastir("Makrog_Savas_Beyi", orkLegi.Makrog_Savas_Beyi);
    }
    if (CreaturesAyristir(CreaturesJsonVerisi, "Buz_Devleri", &orkLegi.Buz_Devleri))
    {
        //CreatureBilgiBastir("Buz_Devleri", orkLegi.Buz_Devleri);
    }


    char *ResearchJsonVerisi = dosyaOku("files/research.json");

    if (ResearchJsonVerisi == NULL)
    {
        printf("Dosya icerigi acilamadi veya bulunamadi!\n");
        return 1;
    }
    savunma_ustaligi savunma_ustaligi; // veya yapýnýn tipini belirtmelisiniz.

    //printf("\n\n");
    //printf("savunma_ustaligi\n");

    if(ResearchAyristir(ResearchJsonVerisi, "savunma_ustaligi", &savunma_ustaligi.seviye_1, "seviye_1"))
    {
        //  ResearchBilgiBastir("seviye_1", savunma_ustaligi.seviye_1);
    }
    if(ResearchAyristir(ResearchJsonVerisi, "savunma_ustaligi", &savunma_ustaligi.seviye_2, "seviye_2"))
    {
        //ResearchBilgiBastir("seviye_2", savunma_ustaligi.seviye_2);
    }
    if(ResearchAyristir(ResearchJsonVerisi, "savunma_ustaligi", &savunma_ustaligi.seviye_3, "seviye_3"))
    {
        //ResearchBilgiBastir("seviye_3", savunma_ustaligi.seviye_3);
    }
    saldiri_gelistirmesi saldiri_gelistirmesi;

    //printf("\n\n");
    //printf("saldiri_gelistirmesi\n");

    if(ResearchAyristir(ResearchJsonVerisi, "saldiri_gelistirmesi", &saldiri_gelistirmesi.seviye_1, "seviye_1"))
    {
        //  ResearchBilgiBastir("seviye_1", saldiri_gelistirmesi.seviye_1);
    }
    if(ResearchAyristir(ResearchJsonVerisi, "saldiri_gelistirmesi", &saldiri_gelistirmesi.seviye_2, "seviye_2"))
    {
        //ResearchBilgiBastir("seviye_2", saldiri_gelistirmesi.seviye_2);
    }
    if(ResearchAyristir(ResearchJsonVerisi, "saldiri_gelistirmesi", &saldiri_gelistirmesi.seviye_3, "seviye_3"))
    {
        //ResearchBilgiBastir("seviye_3", saldiri_gelistirmesi.seviye_3);
    }

    elit_egitim elit_egitim;
    //printf("\n\n");
    //printf("elit_egitim\n");

    if(ResearchAyristir(ResearchJsonVerisi, "elit_egitim", &elit_egitim.seviye_1, "seviye_1"))
    {
        //  ResearchBilgiBastir("seviye_1", elit_egitim.seviye_1);
    }
    if(ResearchAyristir(ResearchJsonVerisi, "elit_egitim", &elit_egitim.seviye_2, "seviye_2"))
    {
        //ResearchBilgiBastir("seviye_2", elit_egitim.seviye_2);
    }
    if(ResearchAyristir(ResearchJsonVerisi, "elit_egitim", &elit_egitim.seviye_3, "seviye_3"))
    {
        //ResearchBilgiBastir("seviye_3", elit_egitim.seviye_3);
    }

    kusatma_ustaligi kusatma_ustaligi;
    //printf("\n\n");
    //printf("kusatma_ustaligi\n");

    if(ResearchAyristir(ResearchJsonVerisi, "kusatma_ustaligi", &kusatma_ustaligi.seviye_1, "seviye_1"))
    {
        //   ResearchBilgiBastir("seviye_1", kusatma_ustaligi.seviye_1);
    }
    if(ResearchAyristir(ResearchJsonVerisi, "kusatma_ustaligi", &kusatma_ustaligi.seviye_2, "seviye_2"))
    {
        // ResearchBilgiBastir("seviye_2", kusatma_ustaligi.seviye_2);
    }
    if(ResearchAyristir(ResearchJsonVerisi, "kusatma_ustaligi", &kusatma_ustaligi.seviye_3, "seviye_3"))
    {
        //ResearchBilgiBastir("seviye_3", kusatma_ustaligi.seviye_3);
    }

    CURL *curl_tutucu;
    CURLcode sonuc;

    const char *url_adresleri[10] =
    {
        "https://yapbenzet.org.tr/1.json",
        "https://yapbenzet.org.tr/2.json",
        "https://yapbenzet.org.tr/3.json",
        "https://yapbenzet.org.tr/4.json",
        "https://yapbenzet.org.tr/5.json",
        "https://yapbenzet.org.tr/6.json",
        "https://yapbenzet.org.tr/7.json",
        "https://yapbenzet.org.tr/8.json",
        "https://yapbenzet.org.tr/9.json",
        "https://yapbenzet.org.tr/10.json"
    };

    int secim;
    printf("10 farkli URL arasindan birini secin (1-10): ");
    scanf("%d", &secim);

    if(secim < 1 || secim > 10)
    {
        printf("Gecersiz secim!\n");
        return 1;
    }

    const char *secilen_url = url_adresleri[secim - 1];
    printf("Secilen URL: %s\n", secilen_url);

    struct BellekYapisi parcacik;
    parcacik.veri = (char *) malloc(1);
    parcacik.boyut = 0;

    curl_global_init(CURL_GLOBAL_ALL);

    curl_tutucu = curl_easy_init();
    if(curl_tutucu)
    {
        curl_easy_setopt(curl_tutucu, CURLOPT_URL, secilen_url);
        curl_easy_setopt(curl_tutucu, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl_tutucu, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl_tutucu, CURLOPT_WRITEFUNCTION, BellekYazmaGeriCagirma);
        curl_easy_setopt(curl_tutucu, CURLOPT_WRITEDATA, (void *)&parcacik);
        sonuc = curl_easy_perform(curl_tutucu);

        if(sonuc != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() basarisiz: %s\n", curl_easy_strerror(sonuc));
        }
        else
        {
            printf("içerik (%zu bayt):\n%s\n", parcacik.boyut, parcacik.veri);

            FILE *fp = fopen("cikti.json", "w");
            if(fp == NULL)
            {
                perror("Dosya acma hatasi");
                free(parcacik.veri);
                curl_easy_cleanup(curl_tutucu);
                curl_global_cleanup();
                return 1;
            }

            size_t yazilan = fwrite(parcacik.veri, sizeof(char), parcacik.boyut, fp);
            if(yazilan != parcacik.boyut)
            {
                perror("Dosyaya yazma hatasi");
            }
            else
            {
                printf("Veri basariyla 'cikti.json' dosyasina yazildi.\n");
            }

            fclose(fp);
        }
        curl_easy_cleanup(curl_tutucu);
    }
    curl_global_cleanup();
    if(parcacik.veri)
        free(parcacik.veri);

    char *SenaryoJsonVerisi = dosyaOku("cikti.json");

    if (SenaryoJsonVerisi == NULL)
    {
        printf("Dosya icerigi acilamadi veya bulunamadi!\n");
        return 1;
    }

    printf("\nOrk Legi:\n");
    char *SenaryoAdres_ork = strstr(SenaryoJsonVerisi, "\"ork_legi\"");

    orkLegi.troller.birlik_sayisi=0;
    orkLegi.ork_dovusculeri.birlik_sayisi=0;
    orkLegi.varg_binicileri.birlik_sayisi=0;
    orkLegi.mizrakcilar.birlik_sayisi=0;
    char kahramanismiOrk[50];
    char CanavarismiOrk[50] ;

    if (SenaryoAdres_ork != NULL)
    {

        char *BirimAdres_ork = strstr(SenaryoAdres_ork, "\"birimler\"");
        if (BirimAdres_ork != NULL)
        {
            char *Birim1_ork = strstr(BirimAdres_ork, "\"ork_dovusculeri\"");
            if (Birim1_ork != NULL)
            {
                sscanf(Birim1_ork, "\"ork_dovusculeri\": %f", & orkLegi.ork_dovusculeri.birlik_sayisi );
                printf("ork_dovusculeri : ");
                printf("%f", orkLegi.ork_dovusculeri.birlik_sayisi);
                printf("\n");
            }
            char *Birim2_ork = strstr(BirimAdres_ork, "\"mizrakcilar\"");
            if (Birim2_ork != NULL)
            {
                sscanf(Birim2_ork, "\"mizrakcilar\": %f", &orkLegi.mizrakcilar.birlik_sayisi );
                printf("mizrakcilar : ");
                printf("%f", orkLegi.mizrakcilar.birlik_sayisi);
                printf("\n");
            }
            char *Birim3_ork= strstr(BirimAdres_ork, "\"varg_binicileri\"");
            if (Birim3_ork!= NULL)
            {
                sscanf(Birim3_ork, "\"varg_binicileri\": %f", & orkLegi.varg_binicileri.birlik_sayisi );
                printf("varg_binicileri : ");
                printf("%d", orkLegi.varg_binicileri.birlik_sayisi);
                printf("\n");
            }
            char *Birim4_ork= strstr(BirimAdres_ork, "\"troller\"");
            if (Birim4_ork != NULL)
            {
                sscanf(Birim4_ork, "\"troller\": %f", &  orkLegi.troller.birlik_sayisi);
                printf("troller : ");
                printf("%f", orkLegi.troller.birlik_sayisi);
                printf("\n");
            }


            char *kahramanAdresOrk = strstr(SenaryoAdres_ork, "\"kahramanlar\"");
            if (kahramanAdresOrk != NULL)
            {
                sscanf(kahramanAdresOrk, "\"kahramanlar\": [\"%[^\"]\"]", kahramanismiOrk);
                printf("Kahraman: %s\n", kahramanismiOrk);
            }
        }
        char *CanavarAdresOrk =strstr(SenaryoAdres_ork, "\"canavarlar\"");
        if(CanavarAdresOrk!=NULL)
        {
            sscanf(CanavarAdresOrk, "\"canavarlar\": [\"%[^\"]\"]",CanavarismiOrk) ;
            printf ("Canavar : %s\n",CanavarismiOrk) ;
        }
    }

    printf("\nInsanoglu imparatorlugu :\n");

    char *SenaryoAdres_insan = strstr(SenaryoJsonVerisi, "\"insan_imparatorlugu\"");
    insanImparatorlugu.suvariler.birlik_sayisi=0;
    insanImparatorlugu.okcular.birlik_sayisi=0;
    insanImparatorlugu.kusatma_makineleri.birlik_sayisi=0;
    insanImparatorlugu.piyadeler.birlik_sayisi=0;
    char kahramanismiinsan[50];
    char kahramanismiinsan2[50];
    char CanavarismiInsan[50] ;
    if(SenaryoAdres_insan != NULL)
    {
        char *BirimAdres_insan = strstr(SenaryoAdres_insan, "\"birimler\"");
        if (BirimAdres_insan != NULL)
        {
            char *Birim1_insan = strstr(BirimAdres_insan, "\"piyadeler\"");
            if (Birim1_insan != NULL)
            {
                sscanf(Birim1_insan, "\"piyadeler\": %f", &insanImparatorlugu.piyadeler.birlik_sayisi );
                printf("piyadeler : ");
                printf("%f", insanImparatorlugu.piyadeler.birlik_sayisi);
                printf("\n");
            }
            char *Birim2_insan= strstr(BirimAdres_insan, "\"suvariler\"");
            if (Birim2_insan != NULL)
            {
                sscanf(Birim2_insan, "\"suvariler\": %f", &insanImparatorlugu.suvariler.birlik_sayisi );
                printf("suvariler : ");
                printf("%f", insanImparatorlugu.suvariler.birlik_sayisi);
                printf("\n");
            }
            char *Birim3_insan = strstr(BirimAdres_insan, "\"okcular\"");
            if (Birim3_insan != NULL)
            {
                sscanf(Birim3_insan, "\"okcular\": %f", &insanImparatorlugu.okcular.birlik_sayisi);
                printf("okcular : ");
                printf("%f", insanImparatorlugu.okcular.birlik_sayisi);
                printf("\n");
            }
            char *Birim4_insan = strstr(BirimAdres_insan, "\"kusatma_makineleri\"");
            if (Birim4_insan != NULL)
            {
                sscanf(Birim4_insan, "\"kusatma_makineleri\": %f", & insanImparatorlugu.kusatma_makineleri.birlik_sayisi);
                printf("kusatma_makineleri : ");
                printf("%f", insanImparatorlugu.kusatma_makineleri.birlik_sayisi);
                printf("\n");
            }


            char *kahramanAdresinsan = strstr(SenaryoAdres_insan, "\"kahramanlar\"");
            if (kahramanAdresinsan != NULL)
            {

                sscanf(kahramanAdresinsan, "\"kahramanlar\": [\"%[^\"]\", \"%[^\"]\"]", kahramanismiinsan, kahramanismiinsan2);
                printf("Kahraman: %s\n", kahramanismiinsan);
                printf("Kahraman: %s\n", kahramanismiinsan2);
            }
        }

        char *CanavarAdresInsan =strstr(SenaryoAdres_insan, "\"canavarlar\"");
        if(CanavarAdresInsan!=NULL)
        {

            sscanf(CanavarAdresInsan, "\"canavarlar\": [\"%[^\"]\"]",CanavarismiInsan) ;
            printf ("Canavar : %s\n",CanavarismiInsan) ;
        }
    }

    FILE *file = fopen("cikti.json", "r");
    if (file == NULL)
    {
        perror("Dosya açılamadı");
        return 1;
    }

    char line[256];
    float savunma_ustaligi_insan = 0, saldiri_gelistirmesi_insan = 0, elit_egitim_insan = 0, kusatma_ustaligi_insan = 0;
    float savunma_ustaligi_ork = 0, saldiri_gelistirmesi_ork = 0, elit_egitim_ork = 0, kusatma_ustaligi_ork = 0;
    float insan_bulundu = 0, ork_bulundu = 0, arastirma_bulundu = 0;

    while (fgets(line, sizeof(line), file))
    {
        if (strstr(line, "\"insan_imparatorlugu\""))
        {
            insan_bulundu = 1;
            ork_bulundu = 0;
            arastirma_bulundu = 0;
            continue;
        }

        if (strstr(line, "\"ork_legi\""))
        {
            ork_bulundu = 1;
            insan_bulundu = 0;
            arastirma_bulundu = 0;
            continue;
        }

        if (insan_bulundu || ork_bulundu)
        {
            if (strstr(line, "\"arastirma_seviyesi\""))
            {
                arastirma_bulundu = 1;
                continue;
            }

            if (arastirma_bulundu)
            {
                if (strstr(line, "\"savunma_ustaligi\""))
                {
                    if (insan_bulundu)
                        sscanf(line, " \"savunma_ustaligi\": %f", &savunma_ustaligi_insan);
                    else if (ork_bulundu)
                        sscanf(line, " \"savunma_ustaligi\": %f", &savunma_ustaligi_ork);
                }
                else if (strstr(line, "\"saldiri_gelistirmesi\""))
                {
                    if (insan_bulundu)
                        sscanf(line, " \"saldiri_gelistirmesi\": %f", &saldiri_gelistirmesi_insan);
                    else if (ork_bulundu)
                        sscanf(line, " \"saldiri_gelistirmesi\": %f", &saldiri_gelistirmesi_ork);
                }
                else if (strstr(line, "\"elit_egitim\""))
                {
                    if (insan_bulundu)
                        sscanf(line, " \"elit_egitim\": %f", &elit_egitim_insan);
                    else if (ork_bulundu)
                        sscanf(line, " \"elit_egitim\": %f", &elit_egitim_ork);
                }
                else if (strstr(line, "\"kusatma_ustaligi\""))
                {
                    if (insan_bulundu)
                        sscanf(line, " \"kusatma_ustaligi\": %f", &kusatma_ustaligi_insan);
                    else if (ork_bulundu)
                        sscanf(line, " \"kusatma_ustaligi\": %f", &kusatma_ustaligi_ork);
                }
            }
        }
    }
    printf("Insan Savunma Ustaligi LVL: %f\n", savunma_ustaligi_insan);
    printf("Insan Saldiri Gelistirmesi LVL: %f\n", saldiri_gelistirmesi_insan);
    printf("Insan Elit Egitim LVL: %f\n", elit_egitim_insan);
    printf("Insan Kusatma Ustaligi LVL: %f\n\n\n", kusatma_ustaligi_insan);

    printf("Ork Savunma Ustaligi LVL: %f\n", savunma_ustaligi_ork);
    printf("Ork Saldiri Gelistirmesi LVL: %f\n", saldiri_gelistirmesi_ork);
    printf("Ork Elit Egitim LVL: %f\n", elit_egitim_ork);
    printf("Ork Kusatma Ustaligi LVL: %f\n", kusatma_ustaligi_ork);

    fclose(file);
    printf("----------INSAN IMPARATORLUGU SALDIRI GUCU HESAPLAMA KISMI (TEMEL)----------\n\n");

    float okcuSaldiriGucu = baseSaldiriGucu(&insanImparatorlugu.okcular, insanImparatorlugu.okcular.saldiri);
    printf("\nOkcu sayiniz: %f\n", insanImparatorlugu.okcular.birlik_sayisi);
    printf("Okculardan gelen temel saldiri gucu: %f\n\n\n", okcuSaldiriGucu);

    float piyadeSaldiriGucu= baseSaldiriGucu(&insanImparatorlugu.piyadeler, insanImparatorlugu.piyadeler.saldiri);
    printf ("Piyade sayiniz : %f\n", insanImparatorlugu.piyadeler.birlik_sayisi);
    printf ("Piyadelerinizden gelen temel saldiri gucu : %f\n\n",piyadeSaldiriGucu) ;

    float suvarilerSaldiriGucu= baseSaldiriGucu(&insanImparatorlugu.suvariler, insanImparatorlugu.suvariler.saldiri);
    printf ("Suvari sayiniz : %f\n", insanImparatorlugu.suvariler.birlik_sayisi) ;
    printf ("Suvarilerinizdne gelen temel saldiri gucu : %f\n\n", suvarilerSaldiriGucu) ;

    float kusatmaMakineSaldiriGucu= baseSaldiriGucu(&insanImparatorlugu.kusatma_makineleri, insanImparatorlugu.kusatma_makineleri.saldiri) ;
    printf ("Kusatma Makinesi Sayiniz : %f\n", insanImparatorlugu.kusatma_makineleri.birlik_sayisi) ;
    printf ("Kusatma Makinelerinizden gelen temel saldiri gucu: %f\n\n", kusatmaMakineSaldiriGucu) ;

    float insanToplamTemelSaldiriGucu= okcuSaldiriGucu+piyadeSaldiriGucu+suvarilerSaldiriGucu+kusatmaMakineSaldiriGucu ;
    printf ("Insan Imparatorlugu Total Temel saldiri gucu: %f\n\n", insanToplamTemelSaldiriGucu) ;

    printf("----------ORK LEGI SALDIRI GUCU HESAPLAMA KISMI (TEMEL)----------\n\n");
    float orkDovuscuSaldiriGucu= baseSaldiriGucu(&orkLegi.ork_dovusculeri, orkLegi.ork_dovusculeri.saldiri);
    printf ("Ork Dovuscu sayiniz : %f\n",orkLegi.ork_dovusculeri.birlik_sayisi) ;
    printf ("Ork Dovusculerinizden gelen temel saldiri gucu : %f\n\n\n", orkDovuscuSaldiriGucu) ;

    float mizrakcilarSaldiriGucu= baseSaldiriGucu(&orkLegi.mizrakcilar, orkLegi.mizrakcilar.saldiri) ;
    printf ("Mizrakci sayiniz : %f\n",orkLegi.mizrakcilar.birlik_sayisi) ;
    printf ("Mizrakcilarinizdan gelen temel saldiri gucu : %f\n\n\n", mizrakcilarSaldiriGucu) ;

    float vargBinicileriSaldiriGucu= baseSaldiriGucu(&orkLegi.varg_binicileri, orkLegi.varg_binicileri.saldiri) ;
    printf ("Varg Binici sayiniz : %f\n",orkLegi.varg_binicileri.birlik_sayisi) ;
    printf ("Varg Binici gelen temel saldiri gucu : %f\n\n\n", vargBinicileriSaldiriGucu) ;

    float trollerSaldiriGucu= baseSaldiriGucu(&orkLegi.troller, orkLegi.troller.saldiri) ;
    printf ("Troll sayiniz : %f\n",orkLegi.troller.birlik_sayisi) ;
    printf ("Trollerden gelen temel saldiri gucu : %f\n\n\n", trollerSaldiriGucu) ;

    float OrklegiToplamTemelSaldiriGucu= orkDovuscuSaldiriGucu+mizrakcilarSaldiriGucu+vargBinicileriSaldiriGucu+trollerSaldiriGucu ;
    printf ("ORK LEGI TOPLAM TEMEL SALDIRI GUCUNUZ : %f\n\n", OrklegiToplamTemelSaldiriGucu) ;


    printf ("---------------INSAN IMPARATORLUGU SAVUNMA GUCU HESAPLAMA KISMI (TEMEL)----------------------");
    float okcuSavunmaGucu= baseSavunmaGucu(&insanImparatorlugu.okcular, insanImparatorlugu.okcular.savunma) ;
    printf("\nOkcu sayiniz: %f\n", insanImparatorlugu.okcular.birlik_sayisi);
    printf("Okculardan gelen temel savunma gucu: %f\n\n\n", okcuSavunmaGucu);

    float piyadeSavunmaGucu=baseSavunmaGucu(&insanImparatorlugu.piyadeler, insanImparatorlugu.piyadeler.savunma);
    printf ("\nPiyade sayiniz :%f\n", insanImparatorlugu.piyadeler.birlik_sayisi) ;
    printf("Piyadelerinizden gelen temel savunma gucu: %f\n\n\n", piyadeSavunmaGucu);

    float suvariSavunmaGucu=baseSavunmaGucu(&insanImparatorlugu.suvariler, insanImparatorlugu.suvariler.savunma);
    printf ("Suvari sayiniz : %f\n", insanImparatorlugu.suvariler.birlik_sayisi) ;
    printf ("Suvarilerden gelen temel savunma gucu : %f\n\n", suvariSavunmaGucu) ;

    float kusatmaMakineSavunmaGucu= baseSavunmaGucu(&insanImparatorlugu.kusatma_makineleri, insanImparatorlugu.kusatma_makineleri.savunma) ;
    printf ("Kusatma Makine sayiniz : %f\n", insanImparatorlugu.kusatma_makineleri.birlik_sayisi) ;
    printf ("Kusatma Makinelerinden gelen temel savunma gucu : %f\n\n", kusatmaMakineSavunmaGucu) ;

    float InsanToplamTemelSavunmaGucu= okcuSavunmaGucu + piyadeSavunmaGucu + suvariSavunmaGucu + kusatmaMakineSavunmaGucu;
    printf("Insanlardan gelen toplam savunma gucu :%f\n",InsanToplamTemelSavunmaGucu);

    printf ("------------------------ORK LEGI SAVUNMA GUCU HESAPLAMA KISMI(TEMEL)---------------------\n") ;
    float orkdovuscuSavunmaGucu=baseSavunmaGucu(&orkLegi.ork_dovusculeri, orkLegi.ork_dovusculeri.savunma) ;
    printf ("Ork Dovuscu sayiniz : %f\n",orkLegi.ork_dovusculeri.birlik_sayisi) ;
    printf ("Ork Dovusculerinizden gelen temel savunma gucu : %f\n\n\n", orkdovuscuSavunmaGucu) ;

    float mizrakciSavunmaGucu=baseSavunmaGucu(&orkLegi.mizrakcilar, orkLegi.mizrakcilar.savunma) ;
    printf ("Mizrakci sayiniz : %f\n",orkLegi.mizrakcilar.birlik_sayisi) ;
    printf ("Mizrakcilarinizdan gelen temel savunma gucu : %f\n\n\n", mizrakciSavunmaGucu) ;

    float vargBinicileriSavunmaGucu = baseSavunmaGucu(&orkLegi.varg_binicileri, orkLegi.varg_binicileri.savunma)  ;
    printf ("Varg Binici sayiniz : %f\n",orkLegi.varg_binicileri.birlik_sayisi) ;
    printf ("Varg Binici gelen temel savunma gucu : %f\n\n\n", vargBinicileriSavunmaGucu) ;

    float trollerSavunmaGucu= baseSavunmaGucu(&orkLegi.troller, orkLegi.troller.savunma) ;
    printf ("Troll sayiniz : %f\n",orkLegi.troller.birlik_sayisi) ;
    printf ("Trollerden gelen temel savunma gucu : %f\n\n", trollerSavunmaGucu) ;

    float OrklegiToplamTemelSavunmaGucu = orkdovuscuSavunmaGucu + mizrakciSavunmaGucu + vargBinicileriSavunmaGucu + trollerSavunmaGucu;
    printf("Ork leginden gelen toplam savunma gucu : %f\n",OrklegiToplamTemelSavunmaGucu);


    printf ("---------------INSAN IMPARATORLUGU SAGLIK HESAPLAMA KISMI (TEMEL)----------------------");
    float okcuSaglikGucu= baseSaglikaGucu(&insanImparatorlugu.okcular, insanImparatorlugu.okcular.saglik) ;
    printf("\nOkcu sayiniz: %f\n", insanImparatorlugu.okcular.birlik_sayisi);
    printf("Okculardan gelen temel saglik : %f\n\n\n", okcuSavunmaGucu);

    float piyadeSaglikGucu=baseSaglikaGucu(&insanImparatorlugu.piyadeler, insanImparatorlugu.piyadeler.saglik);
    printf ("\nPiyade sayiniz :%f\n", insanImparatorlugu.piyadeler.birlik_sayisi) ;
    printf("Piyadelerinizden gelen temel saglik : %f\n\n\n", piyadeSaglikGucu);

    float suvariSaglikGucu=baseSaglikaGucu(&insanImparatorlugu.suvariler, insanImparatorlugu.suvariler.saglik);
    printf ("Suvari sayiniz : %f\n", insanImparatorlugu.suvariler.birlik_sayisi) ;
    printf ("Suvarilerden gelen temel saglik : %f\n\n", suvariSaglikGucu) ;

    float kusatmaMakineSaglikGucu= baseSaglikaGucu(&insanImparatorlugu.kusatma_makineleri, insanImparatorlugu.kusatma_makineleri.saglik) ;
    printf ("Kusatma Makine sayiniz : %f\n", insanImparatorlugu.kusatma_makineleri.birlik_sayisi) ;
    printf ("Kusatma Makinelerinden gelen temel saglik : %f\n\n", kusatmaMakineSaglikGucu) ;

    float InsanToplamTemelSaglikGucu= okcuSaglikGucu + piyadeSaglikGucu + suvariSaglikGucu + kusatmaMakineSaglikGucu;
    printf("Insanlardan gelen toplam saglik : %f\n",InsanToplamTemelSaglikGucu);


    printf ("------------------------ORK LEGI SAGLIK HESAPLAMA KISMI(TEMEL)---------------------\n") ;
    float orkdovuscuSaglikGucu=baseSaglikaGucu(&orkLegi.ork_dovusculeri, orkLegi.ork_dovusculeri.saglik) ;
    printf ("Ork Dovuscu sayiniz : %f\n",orkLegi.ork_dovusculeri.birlik_sayisi) ;
    printf ("Ork Dovusculerinizden gelen temel saglik : %f\n\n\n", orkdovuscuSaglikGucu) ;

    float mizrakciSaglikGucu=baseSaglikaGucu(&orkLegi.mizrakcilar, orkLegi.mizrakcilar.saglik) ;
    printf ("Mizrakci sayiniz : %f\n",orkLegi.mizrakcilar.birlik_sayisi) ;
    printf ("Mizrakcilarinizdan gelen temel saglik: %f\n\n\n", mizrakciSaglikGucu) ;

    float vargBinicileriSaglikGucu = baseSaglikaGucu(&orkLegi.varg_binicileri, orkLegi.varg_binicileri.saglik)  ;
    printf ("Varg Binici sayiniz : %f\n",orkLegi.varg_binicileri.birlik_sayisi) ;
    printf ("Varg Binici gelen temel saglik : %f\n\n\n", vargBinicileriSaglikGucu) ;

    float trollerSaglikGucu= baseSaglikaGucu(&orkLegi.troller, orkLegi.troller.saglik) ;
    printf ("Troll sayiniz : %f\n",orkLegi.troller.birlik_sayisi) ;
    printf ("Trollerden gelen temel saglik : %f\n\n", trollerSaglikGucu) ;

    float OrklegiToplamTemelSaglikGucu = orkdovuscuSaglikGucu + mizrakciSaglikGucu + vargBinicileriSaglikGucu + trollerSaglikGucu;
    printf("Ork leginden gelen toplam saglik :%f \n\n",OrklegiToplamTemelSaglikGucu);

    int sayac=0;

    double insanBoostluSaldiriGucu= (insanImparatorlugu.okcular.birlik_sayisi*insanImparatorlugu.okcular.saldiri) +(insanImparatorlugu.suvariler.birlik_sayisi*insanImparatorlugu.suvariler.saldiri) + (insanImparatorlugu.kusatma_makineleri.birlik_sayisi*insanImparatorlugu.kusatma_makineleri.saldiri)+(insanImparatorlugu.piyadeler.birlik_sayisi*insanImparatorlugu.piyadeler.saldiri);
    double insanBoostluSavunmaGucu= (insanImparatorlugu.okcular.birlik_sayisi*insanImparatorlugu.okcular.savunma) +(insanImparatorlugu.suvariler.birlik_sayisi*insanImparatorlugu.suvariler.savunma) + (insanImparatorlugu.kusatma_makineleri.birlik_sayisi*insanImparatorlugu.kusatma_makineleri.savunma)+(insanImparatorlugu.piyadeler.birlik_sayisi*insanImparatorlugu.piyadeler.savunma);
    double orkLegiBoostluSavunmaGucu=(orkLegi.ork_dovusculeri.birlik_sayisi*orkLegi.ork_dovusculeri.savunma) + (orkLegi.mizrakcilar.birlik_sayisi*orkLegi.mizrakcilar.savunma)+ (orkLegi.varg_binicileri.birlik_sayisi*orkLegi.varg_binicileri.savunma) +(orkLegi.troller.birlik_sayisi*orkLegi.troller.savunma);
    double orkLegiBoostluSaldiriGucu= (orkLegi.ork_dovusculeri.birlik_sayisi*orkLegi.ork_dovusculeri.saldiri) + (orkLegi.mizrakcilar.birlik_sayisi*orkLegi.mizrakcilar.saldiri)+ (orkLegi.varg_binicileri.birlik_sayisi*orkLegi.varg_binicileri.saldiri) +(orkLegi.troller.birlik_sayisi*orkLegi.troller.saldiri);
    double netHasarInsan = insanBoostluSaldiriGucu*(1-(orkLegiBoostluSavunmaGucu/insanBoostluSaldiriGucu)) ;
    double netHasarOrk= orkLegiBoostluSaldiriGucu*(1-(insanBoostluSavunmaGucu/orkLegiBoostluSaldiriGucu)) ;

    printf ("\n------------------------------------------------------------------------\n") ;
    if ((strcmp(kahramanismiinsan,"Yavuz_Sultan_Selim"))==0 || (strcmp(kahramanismiinsan2,"Yavuz_Sultan_Selim"))==0)
    {
        if(strcmp(insanImparatorlugu.Yavuz_Sultan_Selim.bonus_turu,"kritik_sans")==0)
        {
            if ((strcmp(insanImparatorlugu.Yavuz_Sultan_Selim.etkiledigi_birim,"suvariler"))==0)
            {
                insanImparatorlugu.suvariler.kritik_sans =insanImparatorlugu.suvariler.kritik_sans+((insanImparatorlugu.suvariler.kritik_sans*insanImparatorlugu.Yavuz_Sultan_Selim.bonus_degeri)/100);
                printf ("YAVUZ SULTAN SELIM BOOSTLU KRITIK GUCU SUVARI : %f ", insanImparatorlugu.suvariler.kritik_sans);
            }
            if ((strcmp(insanImparatorlugu.Yavuz_Sultan_Selim.etkiledigi_birim,"piyadeler"))==0)
            {
                insanImparatorlugu.piyadeler.kritik_sans =insanImparatorlugu.piyadeler.kritik_sans+((insanImparatorlugu.piyadeler.kritik_sans*insanImparatorlugu.Yavuz_Sultan_Selim.bonus_degeri)/100);
                printf ("YAVUZ SULTAN SELIM BOOSTLU KRITIK GUCU SUVARI : %f ", insanImparatorlugu.piyadeler.kritik_sans);

            }
            if ((strcmp(insanImparatorlugu.Yavuz_Sultan_Selim.etkiledigi_birim,"okcular"))==0)
            {
                insanImparatorlugu.okcular.kritik_sans =insanImparatorlugu.okcular.kritik_sans+((insanImparatorlugu.okcular.kritik_sans*insanImparatorlugu.Yavuz_Sultan_Selim.bonus_degeri)/100);
                printf ("YAVUZ SULTAN SELIM BOOSTLU KRITIK GUCU SUVARI : %f ", insanImparatorlugu.okcular.kritik_sans);

            }
            if ((strcmp(insanImparatorlugu.Yavuz_Sultan_Selim.etkiledigi_birim,"kusatma_makineleri"))==0)
            {
                insanImparatorlugu.kusatma_makineleri.kritik_sans =insanImparatorlugu.kusatma_makineleri.kritik_sans+((insanImparatorlugu.kusatma_makineleri.kritik_sans*insanImparatorlugu.Yavuz_Sultan_Selim.bonus_degeri)/100);
                printf ("YAVUZ SULTAN SELIM BOOSTLU KRITIK GUCU SUVARI : %f ", insanImparatorlugu.kusatma_makineleri.kritik_sans);
            }
        }
        if(strcmp(insanImparatorlugu.Yavuz_Sultan_Selim.bonus_turu,"savunma")==0)
        {
            if ((strcmp(insanImparatorlugu.Yavuz_Sultan_Selim.etkiledigi_birim,"suvariler"))==0)
            {
                insanImparatorlugu.suvariler.savunma =insanImparatorlugu.suvariler.savunma+((insanImparatorlugu.suvariler.savunma*insanImparatorlugu.Yavuz_Sultan_Selim.bonus_degeri)/100);
                printf ("YAVUZ SULTAN SELIM BOOSTLU SAVUNMA GUCU SUVARI : %f ", insanImparatorlugu.suvariler.savunma);
            }

            if ((strcmp(insanImparatorlugu.Yavuz_Sultan_Selim.etkiledigi_birim,"piyadeler"))==0)
            {
                insanImparatorlugu.piyadeler.savunma=insanImparatorlugu.piyadeler.savunma+((insanImparatorlugu.piyadeler.savunma*insanImparatorlugu.Yavuz_Sultan_Selim.bonus_degeri)/100);
                printf ("YAVUZ SULTAN SELIM BOOSTLU SAVUNMA GUCU SUVARI : %f ", insanImparatorlugu.piyadeler.savunma);
            }
            if ((strcmp(insanImparatorlugu.Yavuz_Sultan_Selim.etkiledigi_birim,"okcular"))==0)
            {
                insanImparatorlugu.okcular.savunma =insanImparatorlugu.okcular.savunma+((insanImparatorlugu.okcular.savunma*insanImparatorlugu.Yavuz_Sultan_Selim.bonus_degeri)/100);
                printf ("YAVUZ SULTAN SELIM BOOSTLU SAVUNMA GUCU SUVARI : %f ", insanImparatorlugu.okcular.savunma);
            }
            if ((strcmp(insanImparatorlugu.Yavuz_Sultan_Selim.etkiledigi_birim,"kusatma_makineleri"))==0)
            {
                insanImparatorlugu.kusatma_makineleri.savunma =insanImparatorlugu.kusatma_makineleri.savunma+((insanImparatorlugu.kusatma_makineleri.savunma*insanImparatorlugu.Yavuz_Sultan_Selim.bonus_degeri)/100);
                printf ("YAVUZ SULTAN SELIM BOOSTLU SAVUNMA GUCU SUVARI : %f ", insanImparatorlugu.kusatma_makineleri.savunma);
            }
        }
        if(strcmp(insanImparatorlugu.Yavuz_Sultan_Selim.bonus_turu,"saldiri")==0)
        {
            if ((strcmp(insanImparatorlugu.Yavuz_Sultan_Selim.etkiledigi_birim,"suvariler"))==0)
            {
                insanImparatorlugu.suvariler.saldiri =insanImparatorlugu.suvariler.saldiri+((insanImparatorlugu.suvariler.saldiri*insanImparatorlugu.Yavuz_Sultan_Selim.bonus_degeri)/100);
                printf ("YAVUZ SULTAN SELIM BOOSTLU SALDIRI GUCU SUVARI : %f ", insanImparatorlugu.suvariler.saldiri);
            }
            if ((strcmp(insanImparatorlugu.Yavuz_Sultan_Selim.etkiledigi_birim,"piyadeler"))==0)
            {
                insanImparatorlugu.piyadeler.saldiri =insanImparatorlugu.piyadeler.saldiri+((insanImparatorlugu.piyadeler.saldiri*insanImparatorlugu.Yavuz_Sultan_Selim.bonus_degeri)/100);
                printf ("YAVUZ SULTAN SELIM BOOSTLU SALDIRI GUCU SUVARI : %f ", insanImparatorlugu.piyadeler.saldiri);
            }
            if ((strcmp(insanImparatorlugu.Yavuz_Sultan_Selim.etkiledigi_birim,"okcular"))==0)
            {
                insanImparatorlugu.okcular.saldiri =insanImparatorlugu.okcular.saldiri+((insanImparatorlugu.okcular.saldiri*insanImparatorlugu.Yavuz_Sultan_Selim.bonus_degeri)/100);
                printf ("YAVUZ SULTAN SELIM BOOSTLU SALDIRI GUCU SUVARI : %f ", insanImparatorlugu.okcular.saldiri);
            }
            if ((strcmp(insanImparatorlugu.Yavuz_Sultan_Selim.etkiledigi_birim,"kusatma_makineleri"))==0)
            {
                insanImparatorlugu.kusatma_makineleri.saldiri =insanImparatorlugu.kusatma_makineleri.saldiri+((insanImparatorlugu.kusatma_makineleri.saldiri*insanImparatorlugu.Yavuz_Sultan_Selim.bonus_degeri)/100);
                printf ("YAVUZ SULTAN SELIM BOOSTLU SALDIRI GUCU SUVARI : %f ", insanImparatorlugu.kusatma_makineleri.saldiri);
            }
        }
    }
    if ((strcmp(kahramanismiinsan,"Fatih_Sultan_Mehmet"))==0 || (strcmp(kahramanismiinsan2,"Fatih_Sultan_Mehmet"))==0)
    {
        if(strcmp(insanImparatorlugu.Fatih_Sultan_Mehmet.bonus_turu,"kritik_sans")==0)
        {
            if ((strcmp(insanImparatorlugu.Fatih_Sultan_Mehmet.etkiledigi_birim,"suvariler"))==0)
            {
                insanImparatorlugu.suvariler.kritik_sans =insanImparatorlugu.suvariler.kritik_sans+((insanImparatorlugu.suvariler.kritik_sans*insanImparatorlugu.Fatih_Sultan_Mehmet.bonus_degeri)/100);
                printf ("FARIH SULTAN MEHMET BOOSTLU KRITIK GUCU SUVARI : %f ", insanImparatorlugu.suvariler.kritik_sans);
            }
            if ((strcmp(insanImparatorlugu.Fatih_Sultan_Mehmet.etkiledigi_birim,"piyadeler"))==0)
            {
                insanImparatorlugu.piyadeler.kritik_sans =insanImparatorlugu.piyadeler.kritik_sans+((insanImparatorlugu.piyadeler.kritik_sans*insanImparatorlugu.Fatih_Sultan_Mehmet.bonus_degeri)/100);
                printf ("FARIH SULTAN MEHMET BOOSTLU KRITIK GUCU SUVARI : %f ", insanImparatorlugu.piyadeler.kritik_sans);

            }
            if ((strcmp(insanImparatorlugu.Fatih_Sultan_Mehmet.etkiledigi_birim,"okcular"))==0)
            {
                insanImparatorlugu.okcular.kritik_sans =insanImparatorlugu.okcular.kritik_sans+((insanImparatorlugu.okcular.kritik_sans*insanImparatorlugu.Fatih_Sultan_Mehmet.bonus_degeri)/100);
                printf ("FARIH SULTAN MEHMET KRITIK GUCU SUVARI : %f ", insanImparatorlugu.okcular.kritik_sans);

            }
            if ((strcmp(insanImparatorlugu.Fatih_Sultan_Mehmet.etkiledigi_birim,"kusatma_makineleri"))==0)
            {
                insanImparatorlugu.kusatma_makineleri.kritik_sans =insanImparatorlugu.kusatma_makineleri.kritik_sans+((insanImparatorlugu.kusatma_makineleri.kritik_sans*insanImparatorlugu.Fatih_Sultan_Mehmet.bonus_degeri)/100);
                printf ("FARIH SULTAN MEHMET BOOSTLU KRITIK GUCU SUVARI : %f ", insanImparatorlugu.kusatma_makineleri.kritik_sans);
            }
        }
        if(strcmp(insanImparatorlugu.Fatih_Sultan_Mehmet.bonus_turu,"savunma")==0)
        {
            if ((strcmp(insanImparatorlugu.Fatih_Sultan_Mehmet.etkiledigi_birim,"suvariler"))==0)
            {
                insanImparatorlugu.suvariler.savunma =insanImparatorlugu.suvariler.savunma+((insanImparatorlugu.suvariler.savunma*insanImparatorlugu.Fatih_Sultan_Mehmet.bonus_degeri)/100);
                printf ("FARIH SULTAN MEHMET SAVUNMA GUCU SUVARI : %f ", insanImparatorlugu.suvariler.savunma);
            }
            if ((strcmp(insanImparatorlugu.Fatih_Sultan_Mehmet.etkiledigi_birim,"piyadeler"))==0)
            {
                insanImparatorlugu.piyadeler.savunma=insanImparatorlugu.piyadeler.savunma+((insanImparatorlugu.piyadeler.savunma*insanImparatorlugu.Fatih_Sultan_Mehmet.bonus_degeri)/100);
                printf ("FARIH SULTAN MEHMET BOOSTLU SAVUNMA GUCU SUVARI : %f ", insanImparatorlugu.piyadeler.savunma);
            }
            if ((strcmp(insanImparatorlugu.Fatih_Sultan_Mehmet.etkiledigi_birim,"okcular"))==0)
            {
                insanImparatorlugu.okcular.savunma =insanImparatorlugu.okcular.savunma+((insanImparatorlugu.okcular.savunma*insanImparatorlugu.Fatih_Sultan_Mehmet.bonus_degeri)/100);
                printf ("FARIH SULTAN MEHMET BOOSTLU SAVUNMA GUCU SUVARI : %f ", insanImparatorlugu.okcular.savunma);
            }
            if ((strcmp(insanImparatorlugu.Fatih_Sultan_Mehmet.etkiledigi_birim,"kusatma_makineleri"))==0)
            {
                insanImparatorlugu.kusatma_makineleri.savunma =insanImparatorlugu.kusatma_makineleri.savunma+((insanImparatorlugu.kusatma_makineleri.savunma*insanImparatorlugu.Fatih_Sultan_Mehmet.bonus_degeri)/100);
                printf ("FARIH SULTAN MEHMET BOOSTLU SAVUNMA GUCU SUVARI : %f ", insanImparatorlugu.kusatma_makineleri.savunma);
            }
        }
        if(strcmp(insanImparatorlugu.Fatih_Sultan_Mehmet.bonus_turu,"saldiri")==0)
        {
            if ((strcmp(insanImparatorlugu.Fatih_Sultan_Mehmet.etkiledigi_birim,"suvariler"))==0)
            {
                insanImparatorlugu.suvariler.saldiri =insanImparatorlugu.suvariler.saldiri+((insanImparatorlugu.suvariler.saldiri*insanImparatorlugu.Fatih_Sultan_Mehmet.bonus_degeri)/100);
                printf ("FARIH SULTAN MEHMET BOOSTLU SALDIRI GUCU SUVARI : %f ", insanImparatorlugu.suvariler.saldiri);
            }
            if ((strcmp(insanImparatorlugu.Fatih_Sultan_Mehmet.etkiledigi_birim,"piyadeler"))==0)
            {
                insanImparatorlugu.piyadeler.saldiri =insanImparatorlugu.piyadeler.saldiri+((insanImparatorlugu.piyadeler.saldiri*insanImparatorlugu.Fatih_Sultan_Mehmet.bonus_degeri)/100);
                printf ("FARIH SULTAN MEHMET BOOSTLU SALDIRI GUCU SUVARI : %f ", insanImparatorlugu.piyadeler.saldiri);
            }
            if ((strcmp(insanImparatorlugu.Fatih_Sultan_Mehmet.etkiledigi_birim,"okcular"))==0)
            {
                insanImparatorlugu.okcular.saldiri =insanImparatorlugu.okcular.saldiri+((insanImparatorlugu.okcular.saldiri*insanImparatorlugu.Fatih_Sultan_Mehmet.bonus_degeri)/100);
                printf ("FARIH SULTAN MEHMET BOOSTLU SALDIRI GUCU SUVARI : %f ", insanImparatorlugu.okcular.saldiri);
            }
            if ((strcmp(insanImparatorlugu.Fatih_Sultan_Mehmet.etkiledigi_birim,"kusatma_makineleri"))==0)
            {
                insanImparatorlugu.kusatma_makineleri.saldiri =insanImparatorlugu.kusatma_makineleri.saldiri+((insanImparatorlugu.kusatma_makineleri.saldiri*insanImparatorlugu.Fatih_Sultan_Mehmet.bonus_degeri)/100);
                printf ("FARIH SULTAN MEHMET BOOSTLU SALDIRI GUCU KUSATMA MAKINELERI : %f", insanImparatorlugu.kusatma_makineleri.saldiri);
            }
        }
    }
    if ((strcmp(kahramanismiinsan,"Mete_Han"))==0 || (strcmp(kahramanismiinsan2,"Mete_Han"))==0)
    {

        if(strcmp(insanImparatorlugu.Mete_Han.bonus_turu,"kritik_sans")==0)
        {
            if ((strcmp(insanImparatorlugu.Mete_Han.etkiledigi_birim,"suvariler"))==0)
            {
                insanImparatorlugu.suvariler.kritik_sans =insanImparatorlugu.suvariler.kritik_sans+((insanImparatorlugu.suvariler.kritik_sans*insanImparatorlugu.Mete_Han.bonus_degeri)/100);
                printf ("METE HAN BOOSTLU KRITIK GUCU SUVARI : %f ", insanImparatorlugu.suvariler.kritik_sans);
            }
            if ((strcmp(insanImparatorlugu.Mete_Han.etkiledigi_birim,"piyadeler"))==0)
            {
                insanImparatorlugu.piyadeler.kritik_sans =insanImparatorlugu.piyadeler.kritik_sans+((insanImparatorlugu.piyadeler.kritik_sans*insanImparatorlugu.Mete_Han.bonus_degeri)/100);
                printf ("METE HAN BOOSTLU KRITIK GUCU SUVARI : %f ", insanImparatorlugu.piyadeler.kritik_sans);

            }
            if ((strcmp(insanImparatorlugu.Mete_Han.etkiledigi_birim,"okcular"))==0)
            {
                insanImparatorlugu.okcular.kritik_sans =insanImparatorlugu.okcular.kritik_sans+((insanImparatorlugu.okcular.kritik_sans*insanImparatorlugu.Mete_Han.bonus_degeri)/100);
                printf ("METE HAN KRITIK GUCU SUVARI : %f ", insanImparatorlugu.okcular.kritik_sans);

            }
            if ((strcmp(insanImparatorlugu.Mete_Han.etkiledigi_birim,"kusatma_makineleri"))==0)
            {
                insanImparatorlugu.kusatma_makineleri.kritik_sans =insanImparatorlugu.kusatma_makineleri.kritik_sans+((insanImparatorlugu.kusatma_makineleri.kritik_sans*insanImparatorlugu.Mete_Han.bonus_degeri)/100);
                printf ("METE HAN BOOSTLU KRITIK GUCU SUVARI : %f ", insanImparatorlugu.kusatma_makineleri.kritik_sans);
            }
        }
        if(strcmp(insanImparatorlugu.Mete_Han.bonus_turu,"savunma")==0)
        {
            if ((strcmp(insanImparatorlugu.Mete_Han.etkiledigi_birim,"suvariler"))==0)
            {
                insanImparatorlugu.suvariler.savunma =insanImparatorlugu.suvariler.savunma+((insanImparatorlugu.suvariler.savunma*insanImparatorlugu.Mete_Han.bonus_degeri)/100);
                printf ("METE HAN SAVUNMA GUCU SUVARI : %f ", insanImparatorlugu.suvariler.savunma);
            }
            if ((strcmp(insanImparatorlugu.Mete_Han.etkiledigi_birim,"piyadeler"))==0)
            {
                insanImparatorlugu.piyadeler.savunma=insanImparatorlugu.piyadeler.savunma+((insanImparatorlugu.piyadeler.savunma*insanImparatorlugu.Mete_Han.bonus_degeri)/100);
                printf ("METE HAN BOOSTLU SAVUNMA GUCU SUVARI : %f ", insanImparatorlugu.piyadeler.savunma);
            }
            if ((strcmp(insanImparatorlugu.Mete_Han.etkiledigi_birim,"okcular"))==0)
            {
                insanImparatorlugu.okcular.savunma =insanImparatorlugu.okcular.savunma+((insanImparatorlugu.okcular.savunma*insanImparatorlugu.Mete_Han.bonus_degeri)/100);
                printf ("METE HAN BOOSTLU SAVUNMA GUCU SUVARI : %f ", insanImparatorlugu.okcular.savunma);
            }
            if ((strcmp(insanImparatorlugu.Mete_Han.etkiledigi_birim,"kusatma_makineleri"))==0)
            {
                insanImparatorlugu.kusatma_makineleri.savunma =insanImparatorlugu.kusatma_makineleri.savunma+((insanImparatorlugu.kusatma_makineleri.savunma*insanImparatorlugu.Mete_Han.bonus_degeri)/100);
                printf ("METE HAN BOOSTLU SAVUNMA GUCU SUVARI : %f ", insanImparatorlugu.kusatma_makineleri.savunma);
            }
        }
        if(strcmp(insanImparatorlugu.Mete_Han.bonus_turu,"saldiri")==0)
        {
            if ((strcmp(insanImparatorlugu.Mete_Han.etkiledigi_birim,"suvariler"))==0)
            {
                insanImparatorlugu.suvariler.saldiri =insanImparatorlugu.suvariler.saldiri+((insanImparatorlugu.suvariler.saldiri*insanImparatorlugu.Mete_Han.bonus_degeri)/100);
                printf ("METE HAN BOOSTLU SALDIRI GUCU SUVARI : %f ", insanImparatorlugu.suvariler.saldiri);
            }
            if ((strcmp(insanImparatorlugu.Mete_Han.etkiledigi_birim,"piyadeler"))==0)
            {
                insanImparatorlugu.piyadeler.saldiri =insanImparatorlugu.piyadeler.saldiri+((insanImparatorlugu.piyadeler.saldiri*insanImparatorlugu.Mete_Han.bonus_degeri)/100);
                printf ("METE HAN BOOSTLU SALDIRI GUCU SUVARI : %f ", insanImparatorlugu.piyadeler.saldiri);
            }
            if ((strcmp(insanImparatorlugu.Mete_Han.etkiledigi_birim,"okcular"))==0)
            {
                insanImparatorlugu.okcular.saldiri =insanImparatorlugu.okcular.saldiri+((insanImparatorlugu.okcular.saldiri*insanImparatorlugu.Mete_Han.bonus_degeri)/100);
                printf ("METE HAN BOOSTLU SALDIRI GUCU SUVARI : %f ", insanImparatorlugu.okcular.saldiri);
            }
            if ((strcmp(insanImparatorlugu.Mete_Han.etkiledigi_birim,"kusatma_makineleri"))==0)
            {
                insanImparatorlugu.kusatma_makineleri.saldiri =insanImparatorlugu.kusatma_makineleri.saldiri+((insanImparatorlugu.kusatma_makineleri.saldiri*insanImparatorlugu.Mete_Han.bonus_degeri)/100);
                printf ("METE HAN BOOSTLU SALDIRI GUCU KUSATMA MAKINELERI : %f ", insanImparatorlugu.kusatma_makineleri.saldiri);
            }
        }
    }
    if ((strcmp(kahramanismiinsan,"Tugrul_Bey"))==0 || (strcmp(kahramanismiinsan2,"Tugrul_Bey"))==0)
    {
        if(strcmp(insanImparatorlugu.Tugrul_Bey.bonus_turu,"kritik_sans")==0)
        {
            if ((strcmp(insanImparatorlugu.Tugrul_Bey.etkiledigi_birim,"suvariler"))==0)
            {
                insanImparatorlugu.suvariler.kritik_sans =insanImparatorlugu.suvariler.kritik_sans+((insanImparatorlugu.suvariler.kritik_sans*insanImparatorlugu.Tugrul_Bey.bonus_degeri)/100);
                printf ("TUGRUL BEY BOOSTLU KRITIK GUCU SUVARI : %f ", insanImparatorlugu.suvariler.kritik_sans);
            }
            if ((strcmp(insanImparatorlugu.Tugrul_Bey.etkiledigi_birim,"piyadeler"))==0)
            {
                insanImparatorlugu.piyadeler.kritik_sans =insanImparatorlugu.piyadeler.kritik_sans+((insanImparatorlugu.piyadeler.kritik_sans*insanImparatorlugu.Tugrul_Bey.bonus_degeri)/100);
                printf ("TUGRUL BEY BOOSTLU KRITIK GUCU PIYADE : %f ", insanImparatorlugu.piyadeler.kritik_sans);
            }
            if ((strcmp(insanImparatorlugu.Tugrul_Bey.etkiledigi_birim,"okcular"))==0)
            {
                insanImparatorlugu.okcular.kritik_sans =insanImparatorlugu.okcular.kritik_sans+((insanImparatorlugu.okcular.kritik_sans*insanImparatorlugu.Tugrul_Bey.bonus_degeri)/100);
                printf ("TUGRUL BEY BOOSTLU KRITIK GUCU OKCULAR : %f ", insanImparatorlugu.okcular.kritik_sans);
            }
            if ((strcmp(insanImparatorlugu.Tugrul_Bey.etkiledigi_birim,"kusatma_makineleri"))==0)
            {
                insanImparatorlugu.kusatma_makineleri.kritik_sans =insanImparatorlugu.kusatma_makineleri.kritik_sans+((insanImparatorlugu.kusatma_makineleri.kritik_sans*insanImparatorlugu.Tugrul_Bey.bonus_degeri)/100);
                printf ("TUGRUL BEY BOOSTLU KRITIK GUCU KUSATMA MAKINELRI : %f ", insanImparatorlugu.kusatma_makineleri.kritik_sans);
            }
        }
        if(strcmp(insanImparatorlugu.Tugrul_Bey.bonus_turu,"savunma")==0)
        {
            if ((strcmp(insanImparatorlugu.Tugrul_Bey.etkiledigi_birim,"suvariler"))==0)
            {
                insanImparatorlugu.suvariler.savunma =insanImparatorlugu.suvariler.savunma+((insanImparatorlugu.suvariler.savunma*insanImparatorlugu.Tugrul_Bey.bonus_degeri)/100);
                printf ("TUGRUL BEY BOOSTLU SAVUNMA GUCU SUVARI : %f ", insanImparatorlugu.suvariler.savunma);
            }

            if ((strcmp(insanImparatorlugu.Tugrul_Bey.etkiledigi_birim,"piyadeler"))==0)
            {
                insanImparatorlugu.piyadeler.savunma=insanImparatorlugu.piyadeler.savunma+((insanImparatorlugu.piyadeler.savunma*insanImparatorlugu.Tugrul_Bey.bonus_degeri)/100);
                printf ("TUGRUL BEY BOOSTLU SAVUNMA GUCU PIYADE : %f ", insanImparatorlugu.piyadeler.savunma);

            }
            if ((strcmp(insanImparatorlugu.Tugrul_Bey.etkiledigi_birim,"okcular"))==0)
            {
                insanImparatorlugu.okcular.savunma =insanImparatorlugu.okcular.savunma+((insanImparatorlugu.okcular.savunma*insanImparatorlugu.Tugrul_Bey.bonus_degeri)/100);
                printf ("TUGRUL BEY BOOSTLU SAVUNMA GUCU OKCULAR : %f ", insanImparatorlugu.okcular.savunma);

            }
            if ((strcmp(insanImparatorlugu.Tugrul_Bey.etkiledigi_birim,"kusatma_makineleri"))==0)
            {
                insanImparatorlugu.kusatma_makineleri.savunma =insanImparatorlugu.kusatma_makineleri.savunma+((insanImparatorlugu.kusatma_makineleri.savunma*insanImparatorlugu.Tugrul_Bey.bonus_degeri)/100);
                printf ("TUGRUL BEY BOOSTLU SAVUNMA GUCU KUSATMA MAKINELERI : %f ", insanImparatorlugu.kusatma_makineleri.savunma);
            }
        }
        if(strcmp(insanImparatorlugu.Tugrul_Bey.bonus_turu,"saldiri")==0)
        {
            if ((strcmp(insanImparatorlugu.Tugrul_Bey.etkiledigi_birim,"suvariler"))==0)
            {
                insanImparatorlugu.suvariler.saldiri =insanImparatorlugu.suvariler.saldiri+((insanImparatorlugu.suvariler.saldiri*insanImparatorlugu.Tugrul_Bey.bonus_degeri)/100);
                printf ("TUGRUL BEY BOOSTLU SALDIRI GUCU SUVARI : %f ", insanImparatorlugu.suvariler.saldiri);
            }
            if ((strcmp(insanImparatorlugu.Tugrul_Bey.etkiledigi_birim,"piyadeler"))==0)
            {
                insanImparatorlugu.piyadeler.saldiri =insanImparatorlugu.piyadeler.saldiri+((insanImparatorlugu.piyadeler.saldiri*insanImparatorlugu.Tugrul_Bey.bonus_degeri)/100);
                printf ("TUGRUL BEY BOOSTLU SALDIRI GUCU PIYADE : %f ", insanImparatorlugu.piyadeler.saldiri);
            }
            if ((strcmp(insanImparatorlugu.Tugrul_Bey.etkiledigi_birim,"okcular"))==0)
            {
                insanImparatorlugu.okcular.saldiri =insanImparatorlugu.okcular.saldiri+((insanImparatorlugu.okcular.saldiri*insanImparatorlugu.Tugrul_Bey.bonus_degeri)/100);
                printf ("TUGRUL BEY BOOSTLU SALDIRI GUCU OKCULAR : %f ", insanImparatorlugu.okcular.saldiri);
            }
            if ((strcmp(insanImparatorlugu.Tugrul_Bey.etkiledigi_birim,"kusatma_makineleri"))==0)
            {
                insanImparatorlugu.kusatma_makineleri.saldiri =insanImparatorlugu.kusatma_makineleri.saldiri+((insanImparatorlugu.kusatma_makineleri.saldiri*insanImparatorlugu.Fatih_Sultan_Mehmet.bonus_degeri)/100);
                printf ("TUGRUL BEY BOOSTLU SALDIRI GUCU KUSATMA MAKINELERI : %f ", insanImparatorlugu.kusatma_makineleri.saldiri);
            }
        }
    }
    if ((strcmp(kahramanismiinsan,"Alparslan"))==0 || (strcmp(kahramanismiinsan2,"Alparslan"))==0)
    {
        if(strcmp(insanImparatorlugu.Alparslan.bonus_turu,"kritik_sans")==0)
        {
            if ((strcmp(insanImparatorlugu.Alparslan.etkiledigi_birim,"suvariler"))==0)
            {
                insanImparatorlugu.suvariler.kritik_sans =insanImparatorlugu.suvariler.kritik_sans+((insanImparatorlugu.suvariler.kritik_sans*insanImparatorlugu.Alparslan.bonus_degeri)/100);
                printf ("ALPARSLAN BOOSTLU KRITIK GUCU SUVARI : %f ", insanImparatorlugu.suvariler.kritik_sans);
            }
            if ((strcmp(insanImparatorlugu.Alparslan.etkiledigi_birim,"piyadeler"))==0)
            {
                insanImparatorlugu.piyadeler.kritik_sans =insanImparatorlugu.piyadeler.kritik_sans+((insanImparatorlugu.piyadeler.kritik_sans*insanImparatorlugu.Alparslan.bonus_degeri)/100);
                printf ("ALPARSLAN BOOSTLU KRITIK GUCU PIYADE : %f ", insanImparatorlugu.piyadeler.kritik_sans);
            }
            if ((strcmp(insanImparatorlugu.Alparslan.etkiledigi_birim,"okcular"))==0)
            {
                insanImparatorlugu.okcular.kritik_sans =insanImparatorlugu.okcular.kritik_sans+((insanImparatorlugu.okcular.kritik_sans*insanImparatorlugu.Alparslan.bonus_degeri)/100);
                printf ("ALPARSLAN BOOSTLU KRITIK GUCU OKCULAR : %f ", insanImparatorlugu.okcular.kritik_sans);
            }
            if ((strcmp(insanImparatorlugu.Alparslan.etkiledigi_birim,"kusatma_makineleri"))==0)
            {
                insanImparatorlugu.kusatma_makineleri.kritik_sans =insanImparatorlugu.kusatma_makineleri.kritik_sans+((insanImparatorlugu.kusatma_makineleri.kritik_sans*insanImparatorlugu.Alparslan.bonus_degeri)/100);
                printf ("ALPARSLAN BOOSTLU KRITIK GUCU KUSATMA MAKINELRI : %f ", insanImparatorlugu.kusatma_makineleri.kritik_sans);
            }
        }
        if(strcmp(insanImparatorlugu.Alparslan.bonus_turu,"savunma")==0)
        {
            if ((strcmp(insanImparatorlugu.Alparslan.etkiledigi_birim,"suvariler"))==0)
            {
                insanImparatorlugu.suvariler.savunma =insanImparatorlugu.suvariler.savunma+((insanImparatorlugu.suvariler.savunma*insanImparatorlugu.Alparslan.bonus_degeri)/100);
                printf ("ALPARSLAN BOOSTLU SAVUNMA GUCU SUVARI : %f ", insanImparatorlugu.suvariler.savunma);
            }

            if ((strcmp(insanImparatorlugu.Alparslan.etkiledigi_birim,"piyadeler"))==0)
            {
                insanImparatorlugu.piyadeler.savunma=insanImparatorlugu.piyadeler.savunma+((insanImparatorlugu.piyadeler.savunma*insanImparatorlugu.Alparslan.bonus_degeri)/100);
                printf ("ALPASLAN BOOSTLU SAVUNMA GUCU PIYADE : %f ", insanImparatorlugu.piyadeler.savunma);
            }
            if ((strcmp(insanImparatorlugu.Alparslan.etkiledigi_birim,"okcular"))==0)
            {
                printf("%lf",insanImparatorlugu.okcular.savunma);
                insanImparatorlugu.okcular.savunma =insanImparatorlugu.okcular.savunma+((insanImparatorlugu.okcular.savunma*insanImparatorlugu.Alparslan.bonus_degeri)/100);
                printf ("ALPASLAN BOOSTLU SAVUNMA GUCU OKCULAR : %f ", insanImparatorlugu.okcular.savunma);
            }
            if ((strcmp(insanImparatorlugu.Alparslan.etkiledigi_birim,"kusatma_makineleri"))==0)
            {
                insanImparatorlugu.kusatma_makineleri.savunma =insanImparatorlugu.kusatma_makineleri.savunma+((insanImparatorlugu.kusatma_makineleri.savunma*insanImparatorlugu.Alparslan.bonus_degeri)/100);
                printf ("ALPASLAN BOOSTLU SAVUNMA GUCU KUSATMA MAKINELERI : %f ", insanImparatorlugu.kusatma_makineleri.savunma);
            }
        }
        if(strcmp(insanImparatorlugu.Alparslan.bonus_turu,"saldiri")==0)
        {
            if ((strcmp(insanImparatorlugu.Alparslan.etkiledigi_birim,"suvariler"))==0)
            {
                insanImparatorlugu.suvariler.saldiri =insanImparatorlugu.suvariler.saldiri+((insanImparatorlugu.suvariler.saldiri*insanImparatorlugu.Alparslan.bonus_degeri)/100);
                printf ("ALPASLAN BOOSTLU SALDIRI GUCU SUVARI : %f ", insanImparatorlugu.suvariler.saldiri);
            }
            if ((strcmp(insanImparatorlugu.Alparslan.etkiledigi_birim,"piyadeler"))==0)
            {
                insanImparatorlugu.piyadeler.saldiri =insanImparatorlugu.piyadeler.saldiri+((insanImparatorlugu.piyadeler.saldiri*insanImparatorlugu.Alparslan.bonus_degeri)/100);
                printf ("ALPASLAN BOOSTLU SALDIRI GUCU PIYADE : %f ", insanImparatorlugu.piyadeler.saldiri);
            }
            if ((strcmp(insanImparatorlugu.Alparslan.etkiledigi_birim,"okcular"))==0)
            {
                insanImparatorlugu.okcular.saldiri =insanImparatorlugu.okcular.saldiri+((insanImparatorlugu.okcular.saldiri*insanImparatorlugu.Alparslan.bonus_degeri)/100);
                printf ("ALPASLAN BOOSTLU SALDIRI GUCU OKCULAR : %f ", insanImparatorlugu.okcular.saldiri);
            }
            if ((strcmp(insanImparatorlugu.Alparslan.etkiledigi_birim,"kusatma_makineleri"))==0)
            {
                insanImparatorlugu.kusatma_makineleri.saldiri =insanImparatorlugu.kusatma_makineleri.saldiri+((insanImparatorlugu.kusatma_makineleri.saldiri*insanImparatorlugu.Alparslan.bonus_degeri)/100);
                printf ("ALPASLAN BOOSTLU SALDIRI GUCU KUSATMA MAKINELERI : %f ", insanImparatorlugu.kusatma_makineleri.saldiri);
            }
        }
    }
    if ((strcmp(kahramanismiOrk,"Vrog_Kafakiran"))==0)
    {
        printf("\n");
        if(strcmp(orkLegi.Vrog_Kafakiran.bonus_turu,"kritik_sans")==0)
        {
            if ((strcmp(orkLegi.Vrog_Kafakiran.etkiledigi_birim,"varg_binicileri"))==0)
            {
                orkLegi.varg_binicileri.kritik_sans =orkLegi.varg_binicileri.kritik_sans+((orkLegi.varg_binicileri.kritik_sans*orkLegi.Vrog_Kafakiran.bonus_degeri)/100);
                printf ("VROG KAFAKIRAN BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.varg_binicileri.kritik_sans);
            }
            if ((strcmp(orkLegi.Vrog_Kafakiran.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.kritik_sans =orkLegi.ork_dovusculeri.kritik_sans+((orkLegi.ork_dovusculeri.kritik_sans*orkLegi.Vrog_Kafakiran.bonus_degeri)/100);
                printf ("VROG KAFAKIRAN BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.kritik_sans);

            }
            if ((strcmp(orkLegi.Vrog_Kafakiran.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.kritik_sans =orkLegi.mizrakcilar.kritik_sans+((orkLegi.mizrakcilar.kritik_sans*orkLegi.Vrog_Kafakiran.bonus_degeri)/100);
                printf ("VROG KAFAKIRAN BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.mizrakcilar.kritik_sans);

            }
            if ((strcmp(orkLegi.Vrog_Kafakiran.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.kritik_sans =orkLegi.troller.kritik_sans+((orkLegi.troller.kritik_sans*orkLegi.Vrog_Kafakiran.bonus_degeri)/100);
                printf ("VROG KAFAKIRAN BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.troller.kritik_sans);
            }
        }
        if(strcmp(orkLegi.Vrog_Kafakiran.bonus_turu,"savunma")==0)
        {
            if ((strcmp(orkLegi.Vrog_Kafakiran.etkiledigi_birim,"varg_binicilerine"))==0)
            {
                orkLegi.varg_binicileri.savunma =orkLegi.varg_binicileri.savunma+((orkLegi.varg_binicileri.savunma*orkLegi.Vrog_Kafakiran.bonus_degeri)/100);
                printf ("VROG KAFAKIRAN BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.varg_binicileri.savunma);
            }
            if ((strcmp(orkLegi.Vrog_Kafakiran.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.savunma=orkLegi.ork_dovusculeri.savunma+((orkLegi.ork_dovusculeri.savunma*orkLegi.Vrog_Kafakiran.bonus_degeri)/100);
                printf ("VROG KAFAKIRAN BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.savunma);
            }
            if ((strcmp(orkLegi.Vrog_Kafakiran.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.savunma =orkLegi.mizrakcilar.savunma+((orkLegi.mizrakcilar.savunma*orkLegi.Vrog_Kafakiran.bonus_degeri)/100);
                printf ("VROG KAFAKIRAN BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.mizrakcilar.savunma);
            }
            if ((strcmp(orkLegi.Vrog_Kafakiran.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.savunma =orkLegi.troller.savunma+((orkLegi.troller.savunma*orkLegi.Vrog_Kafakiran.bonus_degeri)/100);
                printf ("VROG KAFAKIRAN BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.troller.savunma);
            }
        }
        if(strcmp(orkLegi.Vrog_Kafakiran.bonus_turu,"saldiri")==0)
        {
            if ((strcmp(orkLegi.Vrog_Kafakiran.etkiledigi_birim,"varg_binicilerine"))==0)
            {
                orkLegi.varg_binicileri.saldiri =orkLegi.varg_binicileri.saldiri+((orkLegi.varg_binicileri.saldiri*orkLegi.Vrog_Kafakiran.bonus_degeri)/100);
                printf ("VROG KAFAKIRAN BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.varg_binicileri.saldiri);
            }
            if ((strcmp(orkLegi.Vrog_Kafakiran.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.saldiri =orkLegi.ork_dovusculeri.saldiri+((orkLegi.ork_dovusculeri.saldiri*orkLegi.Vrog_Kafakiran.bonus_degeri)/100);
                printf ("VROG KAFAKIRAN BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.saldiri);
            }
            if ((strcmp(orkLegi.Vrog_Kafakiran.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.saldiri =orkLegi.mizrakcilar.saldiri+((orkLegi.mizrakcilar.saldiri*orkLegi.Vrog_Kafakiran.bonus_degeri)/100);
                printf ("VROG KAFAKIRAN BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.mizrakcilar.saldiri);
            }
            if ((strcmp(orkLegi.Vrog_Kafakiran.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.saldiri =orkLegi.troller.saldiri+((orkLegi.troller.saldiri*orkLegi.Vrog_Kafakiran.bonus_degeri)/100);
                printf ("VROG KAFAKİIRAN BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.troller.saldiri);
            }
        }
    }
    if ((strcmp(kahramanismiOrk,"Goruk_Vahsi"))==0)
    {
        if(strcmp(orkLegi.Goruk_Vahsi.bonus_turu,"kritik_sans")==0)
        {
            if ((strcmp(orkLegi.Goruk_Vahsi.etkiledigi_birim,"varg_binicileri"))==0)
            {
                orkLegi.varg_binicileri.kritik_sans =orkLegi.varg_binicileri.kritik_sans+((orkLegi.varg_binicileri.kritik_sans*orkLegi.Goruk_Vahsi.bonus_degeri)/100);
                printf ("GORUK VAHSI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.varg_binicileri.kritik_sans);
            }
            if ((strcmp(orkLegi.Goruk_Vahsi.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.kritik_sans =orkLegi.ork_dovusculeri.kritik_sans+((orkLegi.ork_dovusculeri.kritik_sans*orkLegi.Goruk_Vahsi.bonus_degeri)/100);
                printf ("GORUK VAHSI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.kritik_sans);
            }
            if ((strcmp(orkLegi.Goruk_Vahsi.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.kritik_sans =orkLegi.mizrakcilar.kritik_sans+((orkLegi.mizrakcilar.kritik_sans*orkLegi.Goruk_Vahsi.bonus_degeri)/100);
                printf ("GORUK VAHSI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.mizrakcilar.kritik_sans);
            }
            if ((strcmp(orkLegi.Goruk_Vahsi.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.kritik_sans =orkLegi.troller.kritik_sans+((orkLegi.troller.kritik_sans*orkLegi.Goruk_Vahsi.bonus_degeri)/100);
                printf ("GORUK VAHSI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.troller.kritik_sans);
            }
        }
        if(strcmp(orkLegi.Goruk_Vahsi.bonus_turu,"savunma")==0)
        {
            if ((strcmp(orkLegi.Goruk_Vahsi.etkiledigi_birim,"varg_binicilerine"))==0)
            {
                orkLegi.varg_binicileri.savunma =orkLegi.varg_binicileri.savunma+((orkLegi.varg_binicileri.savunma*orkLegi.Goruk_Vahsi.bonus_degeri)/100);
                printf ("GORUK VAHSI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.varg_binicileri.savunma);
            }

            if ((strcmp(orkLegi.Goruk_Vahsi.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.savunma=orkLegi.ork_dovusculeri.savunma+((orkLegi.ork_dovusculeri.savunma*orkLegi.Goruk_Vahsi.bonus_degeri)/100);
                printf ("GORUK VAHSI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.savunma);

            }
            if ((strcmp(orkLegi.Goruk_Vahsi.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.savunma =orkLegi.mizrakcilar.savunma+((orkLegi.mizrakcilar.savunma*orkLegi.Goruk_Vahsi.bonus_degeri)/100);
                printf ("GORUK VAHSI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.mizrakcilar.savunma);

            }
            if ((strcmp(orkLegi.Goruk_Vahsi.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.savunma =orkLegi.troller.savunma+((orkLegi.troller.savunma*orkLegi.Goruk_Vahsi.bonus_degeri)/100);
                printf ("GORUK VAHSI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.troller.savunma);
            }
        }
        if(strcmp(orkLegi.Goruk_Vahsi.bonus_turu,"saldiri")==0)
        {
            if ((strcmp(orkLegi.Goruk_Vahsi.etkiledigi_birim,"varg_binicilerine"))==0)
            {
                orkLegi.varg_binicileri.saldiri =orkLegi.varg_binicileri.saldiri+((orkLegi.varg_binicileri.saldiri*orkLegi.Goruk_Vahsi.bonus_degeri)/100);
                printf ("GORUK VAHSI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.varg_binicileri.saldiri);
            }
            if ((strcmp(orkLegi.Goruk_Vahsi.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.saldiri =orkLegi.ork_dovusculeri.saldiri+((orkLegi.ork_dovusculeri.saldiri*orkLegi.Goruk_Vahsi.bonus_degeri)/100);
                printf ("GORUK VAHSI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.saldiri);
            }
            if ((strcmp(orkLegi.Goruk_Vahsi.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.saldiri =orkLegi.mizrakcilar.saldiri+((orkLegi.mizrakcilar.saldiri*orkLegi.Goruk_Vahsi.bonus_degeri)/100);
                printf ("GORUK VAHSI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.mizrakcilar.saldiri);
            }
            if ((strcmp(orkLegi.Goruk_Vahsi.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.saldiri =orkLegi.troller.saldiri+((orkLegi.troller.saldiri*orkLegi.Goruk_Vahsi.bonus_degeri)/100);
                printf ("GORUK VAHSI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.troller.saldiri);
            }
        }
    }
    if ((strcmp(kahramanismiOrk,"Ugar_Zalim"))==0)
    {
        if(strcmp(orkLegi.Ugar_Zalim.bonus_turu,"kritik_sans")==0)
        {
            if ((strcmp(orkLegi.Ugar_Zalim.etkiledigi_birim,"varg_binicileri"))==0)
            {
                orkLegi.varg_binicileri.kritik_sans =orkLegi.varg_binicileri.kritik_sans+((orkLegi.varg_binicileri.kritik_sans*orkLegi.Ugar_Zalim.bonus_degeri)/100);
                printf ("UGAR ZALIM BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.varg_binicileri.kritik_sans);
            }
            if ((strcmp(orkLegi.Ugar_Zalim.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.kritik_sans =orkLegi.ork_dovusculeri.kritik_sans+((orkLegi.ork_dovusculeri.kritik_sans*orkLegi.Ugar_Zalim.bonus_degeri)/100);
                printf ("UGAR ZALIM BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.kritik_sans);
            }
            if ((strcmp(orkLegi.Ugar_Zalim.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.kritik_sans =orkLegi.mizrakcilar.kritik_sans+((orkLegi.mizrakcilar.kritik_sans*orkLegi.Ugar_Zalim.bonus_degeri)/100);
                printf ("UGAR ZALIM BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.mizrakcilar.kritik_sans);
            }
            if ((strcmp(orkLegi.Ugar_Zalim.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.kritik_sans =orkLegi.troller.kritik_sans+((orkLegi.troller.kritik_sans*orkLegi.Ugar_Zalim.bonus_degeri)/100);
                printf ("UGAR ZALIM BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.troller.kritik_sans);
            }
            if ((strcmp(orkLegi.Ugar_Zalim.etkiledigi_birim,"tum_birimler"))==0)
            {
                orkLegi.troller.kritik_sans =orkLegi.troller.kritik_sans+((orkLegi.troller.kritik_sans*orkLegi.Ugar_Zalim.bonus_degeri)/100);
                printf ("UGAR ZALIM BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.troller.kritik_sans);
                orkLegi.mizrakcilar.kritik_sans =orkLegi.mizrakcilar.kritik_sans+((orkLegi.mizrakcilar.kritik_sans*orkLegi.Ugar_Zalim.bonus_degeri)/100);
                printf ("UGAR ZALIM BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.mizrakcilar.kritik_sans);
                orkLegi.ork_dovusculeri.kritik_sans =orkLegi.ork_dovusculeri.kritik_sans+((orkLegi.ork_dovusculeri.kritik_sans*orkLegi.Ugar_Zalim.bonus_degeri)/100);
                printf ("UGAR ZALIM BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.kritik_sans);
                orkLegi.varg_binicileri.kritik_sans =orkLegi.varg_binicileri.kritik_sans+((orkLegi.varg_binicileri.kritik_sans*orkLegi.Ugar_Zalim.bonus_degeri)/100);
                printf ("UGAR ZALIM BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.varg_binicileri.kritik_sans);
            }
        }
        if(strcmp(orkLegi.Ugar_Zalim.bonus_turu,"savunma")==0)
        {
            if ((strcmp(orkLegi.Ugar_Zalim.etkiledigi_birim,"varg_binicilerine"))==0)
            {
                orkLegi.varg_binicileri.savunma =orkLegi.varg_binicileri.savunma+((orkLegi.varg_binicileri.savunma*orkLegi.Ugar_Zalim.bonus_degeri)/100);
                printf ("UGAR ZALIM BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.varg_binicileri.savunma);
            }

            if ((strcmp(orkLegi.Ugar_Zalim.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.savunma=orkLegi.ork_dovusculeri.savunma+((orkLegi.ork_dovusculeri.savunma*orkLegi.Ugar_Zalim.bonus_degeri)/100);
                printf ("UGAR ZALIM BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.savunma);
            }
            if ((strcmp(orkLegi.Ugar_Zalim.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.savunma =orkLegi.mizrakcilar.savunma+((orkLegi.mizrakcilar.savunma*orkLegi.Ugar_Zalim.bonus_degeri)/100);
                printf ("UGAR ZALIM BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.mizrakcilar.savunma);
            }
            if ((strcmp(orkLegi.Ugar_Zalim.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.savunma =orkLegi.troller.savunma+((orkLegi.troller.savunma*orkLegi.Ugar_Zalim.bonus_degeri)/100);
                printf ("UGAR ZALIM BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.troller.savunma);
            }
            if ((strcmp(orkLegi.Ugar_Zalim.etkiledigi_birim,"tum_birimler"))==0)
            {
                orkLegi.troller.savunma =orkLegi.troller.savunma+((orkLegi.troller.savunma*orkLegi.Ugar_Zalim.bonus_degeri)/100);
                printf ("UGAR ZALIM BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.troller.savunma);
                orkLegi.mizrakcilar.savunma =orkLegi.mizrakcilar.savunma+((orkLegi.mizrakcilar.savunma*orkLegi.Ugar_Zalim.bonus_degeri)/100);
                printf ("UGAR ZALIM BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.mizrakcilar.savunma);
                orkLegi.ork_dovusculeri.savunma =orkLegi.ork_dovusculeri.savunma+((orkLegi.ork_dovusculeri.savunma*orkLegi.Ugar_Zalim.bonus_degeri)/100);
                printf ("UGAR ZALIM BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.savunma);
                orkLegi.varg_binicileri.savunma =orkLegi.varg_binicileri.savunma+((orkLegi.varg_binicileri.savunma*orkLegi.Ugar_Zalim.bonus_degeri)/100);
                printf ("UGAR ZALIM BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.varg_binicileri.savunma);
            }
        }
        if(strcmp(orkLegi.Ugar_Zalim.bonus_turu,"saldiri")==0)
        {
            if ((strcmp(orkLegi.Ugar_Zalim.etkiledigi_birim,"varg_binicilerine"))==0)
            {
                orkLegi.varg_binicileri.saldiri =orkLegi.varg_binicileri.saldiri+((orkLegi.varg_binicileri.saldiri*orkLegi.Ugar_Zalim.bonus_degeri)/100);
                printf ("UGAR ZALIM BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.varg_binicileri.saldiri);
            }
            if ((strcmp(orkLegi.Ugar_Zalim.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.saldiri =orkLegi.ork_dovusculeri.saldiri+((orkLegi.ork_dovusculeri.saldiri*orkLegi.Ugar_Zalim.bonus_degeri)/100);
                printf ("UGAR ZALIM BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.saldiri);
            }
            if ((strcmp(orkLegi.Ugar_Zalim.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.saldiri =orkLegi.mizrakcilar.saldiri+((orkLegi.mizrakcilar.saldiri*orkLegi.Ugar_Zalim.bonus_degeri)/100);
                printf ("UGAR ZALIM BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.mizrakcilar.saldiri);
            }
            if ((strcmp(orkLegi.Ugar_Zalim.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.saldiri =orkLegi.troller.saldiri+((orkLegi.troller.saldiri*orkLegi.Ugar_Zalim.bonus_degeri)/100);
                printf ("UGAR ZALIM BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.troller.saldiri);
            }
            if ((strcmp(orkLegi.Ugar_Zalim.etkiledigi_birim,"tum_birimler"))==0)
            {
                orkLegi.troller.saldiri =orkLegi.troller.saldiri+((orkLegi.troller.saldiri*orkLegi.Ugar_Zalim.bonus_degeri)/100);
                printf ("UGAR ZALIM BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.troller.saldiri);
                orkLegi.mizrakcilar.saldiri =orkLegi.mizrakcilar.saldiri+((orkLegi.mizrakcilar.saldiri*orkLegi.Ugar_Zalim.bonus_degeri)/100);
                printf ("UGAR ZALIM BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.mizrakcilar.saldiri);
                orkLegi.ork_dovusculeri.saldiri =orkLegi.ork_dovusculeri.saldiri+((orkLegi.ork_dovusculeri.saldiri*orkLegi.Ugar_Zalim.bonus_degeri)/100);
                printf ("UGAR ZALIM BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.saldiri);
                orkLegi.varg_binicileri.saldiri =orkLegi.varg_binicileri.saldiri+((orkLegi.varg_binicileri.saldiri*orkLegi.Ugar_Zalim.bonus_degeri)/100);
                printf ("UGAR ZALIM BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.varg_binicileri.saldiri);
            }
        }
    }
    if ((strcmp(kahramanismiOrk,"Goruk_Vahsi"))==0)
    {
        if(strcmp(orkLegi.Goruk_Vahsi.bonus_turu,"kritik_sans")==0)
        {
            if ((strcmp(orkLegi.Goruk_Vahsi.etkiledigi_birim,"varg_binicileri"))==0)
            {
                orkLegi.varg_binicileri.kritik_sans =orkLegi.varg_binicileri.kritik_sans+((orkLegi.varg_binicileri.kritik_sans*orkLegi.Goruk_Vahsi.bonus_degeri)/100);
                printf ("GORUK VAHSI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.varg_binicileri.kritik_sans);
            }
            if ((strcmp(orkLegi.Goruk_Vahsi.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.kritik_sans =orkLegi.ork_dovusculeri.kritik_sans+((orkLegi.ork_dovusculeri.kritik_sans*orkLegi.Goruk_Vahsi.bonus_degeri)/100);
                printf ("GORUK VAHSI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.kritik_sans);
            }
            if ((strcmp(orkLegi.Goruk_Vahsi.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.kritik_sans =orkLegi.mizrakcilar.kritik_sans+((orkLegi.mizrakcilar.kritik_sans*orkLegi.Goruk_Vahsi.bonus_degeri)/100);
                printf ("GORUK VAHSI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.mizrakcilar.kritik_sans);
            }
            if ((strcmp(orkLegi.Goruk_Vahsi.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.kritik_sans =orkLegi.troller.kritik_sans+((orkLegi.troller.kritik_sans*orkLegi.Goruk_Vahsi.bonus_degeri)/100);
                printf ("GORUK VAHSI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.troller.kritik_sans);
            }
        }
        if(strcmp(orkLegi.Goruk_Vahsi.bonus_turu,"savunma")==0)
        {
            if ((strcmp(orkLegi.Goruk_Vahsi.etkiledigi_birim,"varg_binicilerine"))==0)
            {
                orkLegi.varg_binicileri.savunma =orkLegi.varg_binicileri.savunma+((orkLegi.varg_binicileri.savunma*orkLegi.Goruk_Vahsi.bonus_degeri)/100);
                printf ("GORUK VAHSI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.varg_binicileri.savunma);
            }
            if ((strcmp(orkLegi.Goruk_Vahsi.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.savunma=orkLegi.ork_dovusculeri.savunma+((orkLegi.ork_dovusculeri.savunma*orkLegi.Goruk_Vahsi.bonus_degeri)/100);
                printf ("GORUK VAHSI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.savunma);

            }
            if ((strcmp(orkLegi.Goruk_Vahsi.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.savunma =orkLegi.mizrakcilar.savunma+((orkLegi.mizrakcilar.savunma*orkLegi.Goruk_Vahsi.bonus_degeri)/100);
                printf ("GORUK VAHSI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.mizrakcilar.savunma);

            }
            if ((strcmp(orkLegi.Goruk_Vahsi.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.savunma =orkLegi.troller.savunma+((orkLegi.troller.savunma*orkLegi.Goruk_Vahsi.bonus_degeri)/100);
                printf ("GORUK VAHSI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.troller.savunma);
            }
        }
        if(strcmp(orkLegi.Goruk_Vahsi.bonus_turu,"saldiri")==0)
        {
            if ((strcmp(orkLegi.Goruk_Vahsi.etkiledigi_birim,"varg_binicilerine"))==0)
            {
                orkLegi.varg_binicileri.saldiri =orkLegi.varg_binicileri.saldiri+((orkLegi.varg_binicileri.saldiri*orkLegi.Goruk_Vahsi.bonus_degeri)/100);
                printf ("GORUK VAHSI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.varg_binicileri.saldiri);
            }
            if ((strcmp(orkLegi.Goruk_Vahsi.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.saldiri =orkLegi.ork_dovusculeri.saldiri+((orkLegi.ork_dovusculeri.saldiri*orkLegi.Goruk_Vahsi.bonus_degeri)/100);
                printf ("GORUK VAHSI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.saldiri);
            }
            if ((strcmp(orkLegi.Goruk_Vahsi.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.saldiri =orkLegi.mizrakcilar.saldiri+((orkLegi.mizrakcilar.saldiri*orkLegi.Goruk_Vahsi.bonus_degeri)/100);
                printf ("GORUK VAHSI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.mizrakcilar.saldiri);
            }
            if ((strcmp(orkLegi.Goruk_Vahsi.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.saldiri =orkLegi.troller.saldiri+((orkLegi.troller.saldiri*orkLegi.Goruk_Vahsi.bonus_degeri)/100);
                printf ("GORUK VAHSI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.troller.saldiri);
            }
        }
    }
    if ((strcmp(kahramanismiOrk,"Thruk_Kemikkiran"))==0)
    {
        if(strcmp(orkLegi.Thruk_Kemikkiran.bonus_turu,"kritik_sans")==0)
        {
            if ((strcmp(orkLegi.Thruk_Kemikkiran.etkiledigi_birim,"varg_binicileri"))==0)
            {
                orkLegi.varg_binicileri.kritik_sans =orkLegi.varg_binicileri.kritik_sans+((orkLegi.varg_binicileri.kritik_sans*orkLegi.Thruk_Kemikkiran.bonus_degeri)/100);
                printf ("TRUK KEMIKKIRAN BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.varg_binicileri.kritik_sans);
            }
            if ((strcmp(orkLegi.Thruk_Kemikkiran.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.kritik_sans =orkLegi.ork_dovusculeri.kritik_sans+((orkLegi.ork_dovusculeri.kritik_sans*orkLegi.Thruk_Kemikkiran.bonus_degeri)/100);
                printf ("TRUK KEMIKKIRAN BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.kritik_sans);

            }
            if ((strcmp(orkLegi.Thruk_Kemikkiran.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.kritik_sans =orkLegi.mizrakcilar.kritik_sans+((orkLegi.mizrakcilar.kritik_sans*orkLegi.Thruk_Kemikkiran.bonus_degeri)/100);
                printf ("TRUK KEMIKKIRAN BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.mizrakcilar.kritik_sans);

            }
            if ((strcmp(orkLegi.Thruk_Kemikkiran.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.kritik_sans =orkLegi.troller.kritik_sans+((orkLegi.troller.kritik_sans*orkLegi.Thruk_Kemikkiran.bonus_degeri)/100);
                printf ("TRUK KEMIKKIRAN BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.troller.kritik_sans);
            }
        }
        if(strcmp(orkLegi.Thruk_Kemikkiran.bonus_turu,"savunma")==0)
        {
            if ((strcmp(orkLegi.Thruk_Kemikkiran.etkiledigi_birim,"varg_binicilerine"))==0)
            {
                orkLegi.varg_binicileri.savunma =orkLegi.varg_binicileri.savunma+((orkLegi.varg_binicileri.savunma*orkLegi.Thruk_Kemikkiran.bonus_degeri)/100);
                printf ("TRUK KEMIKKIRAN BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.varg_binicileri.savunma);
            }
            if ((strcmp(orkLegi.Thruk_Kemikkiran.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.savunma=orkLegi.ork_dovusculeri.savunma+((orkLegi.ork_dovusculeri.savunma*orkLegi.Thruk_Kemikkiran.bonus_degeri)/100);
                printf ("TRUK KEMIKKIRAN BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.savunma);

            }
            if ((strcmp(orkLegi.Thruk_Kemikkiran.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.savunma =orkLegi.mizrakcilar.savunma+((orkLegi.mizrakcilar.savunma*orkLegi.Thruk_Kemikkiran.bonus_degeri)/100);
                printf ("TRUK KEMIKKIRAN BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.mizrakcilar.savunma);

            }
            if ((strcmp(orkLegi.Thruk_Kemikkiran.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.savunma =orkLegi.troller.savunma+((orkLegi.troller.savunma*orkLegi.Thruk_Kemikkiran.bonus_degeri)/100);
                printf ("TRUK KEMIKKIRAN BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.troller.savunma);
            }
        }
        if(strcmp(orkLegi.Thruk_Kemikkiran.bonus_turu,"saldiri")==0)
        {
            if ((strcmp(orkLegi.Thruk_Kemikkiran.etkiledigi_birim,"varg_binicilerine"))==0)
            {
                orkLegi.varg_binicileri.saldiri =orkLegi.varg_binicileri.saldiri+((orkLegi.varg_binicileri.saldiri*orkLegi.Thruk_Kemikkiran.bonus_degeri)/100);
                printf ("TRUK KEMIKKIRAN BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.varg_binicileri.saldiri);
            }
            if ((strcmp(orkLegi.Thruk_Kemikkiran.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.saldiri =orkLegi.ork_dovusculeri.saldiri+((orkLegi.ork_dovusculeri.saldiri*orkLegi.Thruk_Kemikkiran.bonus_degeri)/100);
                printf ("TRUK KEMIKKIRAN BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.saldiri);
            }
            if ((strcmp(orkLegi.Thruk_Kemikkiran.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.saldiri =orkLegi.mizrakcilar.saldiri+((orkLegi.mizrakcilar.saldiri*orkLegi.Thruk_Kemikkiran.bonus_degeri)/100);
                printf ("TRUK KEMIKKIRAN BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.mizrakcilar.saldiri);
            }
            if ((strcmp(orkLegi.Thruk_Kemikkiran.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.saldiri =orkLegi.troller.saldiri+((orkLegi.troller.saldiri*orkLegi.Thruk_Kemikkiran.bonus_degeri)/100);
                printf ("TRUK KEMIKKIRAN BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.troller.saldiri);
            }
        }
    }
    if ((strcmp(CanavarismiOrk,"Ates_Iblisi"))==0)
    {
        printf("\n");
        if(strcmp(orkLegi.Ates_Iblisi.etki_turu,"kritik_sans")==0)
        {
            if ((strcmp(orkLegi.Ates_Iblisi.etkiledigi_birim,"varg_binicileri"))==0)
            {
                orkLegi.varg_binicileri.kritik_sans =orkLegi.varg_binicileri.kritik_sans+((orkLegi.varg_binicileri.kritik_sans*orkLegi.Ates_Iblisi.etki_deger)/100);
                printf ("ATES IBLISI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.varg_binicileri.kritik_sans);
            }
            if ((strcmp(orkLegi.Ates_Iblisi.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.kritik_sans =orkLegi.ork_dovusculeri.kritik_sans+((orkLegi.ork_dovusculeri.kritik_sans*orkLegi.Ates_Iblisi.etki_deger)/100);
                printf ("ATES IBLISI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.kritik_sans);

            }
            if ((strcmp(orkLegi.Ates_Iblisi.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.kritik_sans =orkLegi.mizrakcilar.kritik_sans+((orkLegi.mizrakcilar.kritik_sans*orkLegi.Ates_Iblisi.etki_deger)/100);
                printf ("ATES IBLISI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.mizrakcilar.kritik_sans);

            }
            if ((strcmp(orkLegi.Ates_Iblisi.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.kritik_sans =orkLegi.troller.kritik_sans+((orkLegi.troller.kritik_sans*orkLegi.Ates_Iblisi.etki_deger)/100);
                printf ("ATES IBLISI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.troller.kritik_sans);
            }
        }
        if(strcmp(orkLegi.Ates_Iblisi.etki_turu,"savunma")==0)
        {
            if ((strcmp(orkLegi.Ates_Iblisi.etkiledigi_birim,"varg_binicilerine"))==0)
            {
                orkLegi.varg_binicileri.savunma =orkLegi.varg_binicileri.savunma+((orkLegi.varg_binicileri.savunma*orkLegi.Ates_Iblisi.etki_deger)/100);
                printf ("ATES IBLISI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.varg_binicileri.savunma);
            }
            if ((strcmp(orkLegi.Ates_Iblisi.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.savunma=orkLegi.ork_dovusculeri.savunma+((orkLegi.ork_dovusculeri.savunma*orkLegi.Ates_Iblisi.etki_deger)/100);
                printf ("ATES IBLISI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.savunma);

            }
            if ((strcmp(orkLegi.Ates_Iblisi.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.savunma =orkLegi.mizrakcilar.savunma+((orkLegi.mizrakcilar.savunma*orkLegi.Ates_Iblisi.etki_deger)/100);
                printf ("ATES IBLISI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.mizrakcilar.savunma);

            }
            if ((strcmp(orkLegi.Ates_Iblisi.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.savunma =orkLegi.troller.savunma+((orkLegi.troller.savunma*orkLegi.Ates_Iblisi.etki_deger)/100);
                printf ("ATES IBLISI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.troller.savunma);
            }
        }
        if(strcmp(orkLegi.Ates_Iblisi.etki_turu,"saldiri")==0)
        {
            if ((strcmp(orkLegi.Ates_Iblisi.etkiledigi_birim,"varg_binicilerine"))==0)
            {
                orkLegi.varg_binicileri.saldiri =orkLegi.varg_binicileri.saldiri+((orkLegi.varg_binicileri.saldiri*orkLegi.Ates_Iblisi.etki_deger)/100);
                printf ("ATES IBLISI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.varg_binicileri.saldiri);
            }
            if ((strcmp(orkLegi.Ates_Iblisi.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.saldiri =orkLegi.ork_dovusculeri.saldiri+((orkLegi.ork_dovusculeri.saldiri*orkLegi.Ates_Iblisi.etki_deger)/100);
                printf ("ATES IBLISI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.saldiri);
            }
            if ((strcmp(orkLegi.Ates_Iblisi.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.saldiri =orkLegi.mizrakcilar.saldiri+((orkLegi.mizrakcilar.saldiri*orkLegi.Ates_Iblisi.etki_deger)/100);
                printf ("ATES IBLISI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.mizrakcilar.saldiri);
            }
            if ((strcmp(orkLegi.Ates_Iblisi.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.saldiri =orkLegi.troller.saldiri+((orkLegi.troller.saldiri*orkLegi.Ates_Iblisi.etki_deger)/100);
                printf ("ATES IBLISI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.troller.saldiri);
            }
        }
    }
    if ((strcmp(CanavarismiOrk,"Golge_Kurtlari"))==0)
    {
        if(strcmp(orkLegi.Golge_Kurtlari.etki_turu,"kritik_sans")==0)
        {
            if ((strcmp(orkLegi.Golge_Kurtlari.etkiledigi_birim,"varg_binicileri"))==0)
            {
                orkLegi.varg_binicileri.kritik_sans =orkLegi.varg_binicileri.kritik_sans+((orkLegi.varg_binicileri.kritik_sans*orkLegi.Golge_Kurtlari.etki_deger)/100);
                printf ("GOLGE KURTLARI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.varg_binicileri.kritik_sans);
            }
            if ((strcmp(orkLegi.Golge_Kurtlari.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.kritik_sans =orkLegi.ork_dovusculeri.kritik_sans+((orkLegi.ork_dovusculeri.kritik_sans*orkLegi.Golge_Kurtlari.etki_deger)/100);
                printf ("GOLGE KURTLARI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.kritik_sans);
            }
            if ((strcmp(orkLegi.Golge_Kurtlari.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.kritik_sans =orkLegi.mizrakcilar.kritik_sans+((orkLegi.mizrakcilar.kritik_sans*orkLegi.Golge_Kurtlari.etki_deger)/100);
                printf ("GOLGE KURTLARI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.mizrakcilar.kritik_sans);
            }
            if ((strcmp(orkLegi.Golge_Kurtlari.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.kritik_sans =orkLegi.troller.kritik_sans+((orkLegi.troller.kritik_sans*orkLegi.Golge_Kurtlari.etki_deger)/100);
                printf ("GOLGE KURTLARI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.troller.kritik_sans);
            }
        }
        if(strcmp(orkLegi.Golge_Kurtlari.etki_turu,"savunma")==0)
        {
            if ((strcmp(orkLegi.Golge_Kurtlari.etkiledigi_birim,"varg_binicilerine"))==0)
            {
                orkLegi.varg_binicileri.savunma =orkLegi.varg_binicileri.savunma+((orkLegi.varg_binicileri.savunma*orkLegi.Golge_Kurtlari.etki_deger)/100);
                printf ("GOLGE KURTLARI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.varg_binicileri.savunma);
            }

            if ((strcmp(orkLegi.Golge_Kurtlari.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.savunma=orkLegi.ork_dovusculeri.savunma+((orkLegi.ork_dovusculeri.savunma*orkLegi.Golge_Kurtlari.etki_deger)/100);
                printf ("GOLGE KURTLARI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.savunma);
            }
            if ((strcmp(orkLegi.Golge_Kurtlari.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.savunma =orkLegi.mizrakcilar.savunma+((orkLegi.mizrakcilar.savunma*orkLegi.Golge_Kurtlari.etki_deger)/100);
                printf ("GOLGE KURTLARI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.mizrakcilar.savunma);
            }
            if ((strcmp(orkLegi.Golge_Kurtlari.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.savunma =orkLegi.troller.savunma+((orkLegi.troller.savunma*orkLegi.Golge_Kurtlari.etki_deger)/100);
                printf ("GOLGE KURTLARI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.troller.savunma);
            }
        }
        if(strcmp(orkLegi.Golge_Kurtlari.etki_turu,"saldiri")==0)
        {
            if ((strcmp(orkLegi.Golge_Kurtlari.etkiledigi_birim,"varg_binicilerine"))==0)
            {
                orkLegi.varg_binicileri.saldiri =orkLegi.varg_binicileri.saldiri+((orkLegi.varg_binicileri.saldiri*orkLegi.Golge_Kurtlari.etki_deger)/100);
                printf ("GOLGE KURTLARI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.varg_binicileri.saldiri);
            }
            if ((strcmp(orkLegi.Golge_Kurtlari.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.saldiri =orkLegi.ork_dovusculeri.saldiri+((orkLegi.ork_dovusculeri.saldiri*orkLegi.Golge_Kurtlari.etki_deger)/100);
                printf ("GOLGE KURTLARI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.saldiri);
            }
            if ((strcmp(orkLegi.Golge_Kurtlari.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.saldiri =orkLegi.mizrakcilar.saldiri+((orkLegi.mizrakcilar.saldiri*orkLegi.Golge_Kurtlari.etki_deger)/100);
                printf ("GOLGE KURTLARI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.mizrakcilar.saldiri);
            }
            if ((strcmp(orkLegi.Golge_Kurtlari.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.saldiri =orkLegi.troller.saldiri+((orkLegi.troller.saldiri*orkLegi.Golge_Kurtlari.etki_deger)/100);
                printf ("GOLGE KURTLARI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.troller.saldiri);
            }
        }
    }
    if ((strcmp(CanavarismiOrk,"Kara_Troll"))==0)
    {
        if(strcmp(orkLegi.Kara_Troll.etki_turu,"kritik_sans")==0)
        {
            if ((strcmp(orkLegi.Kara_Troll.etkiledigi_birim,"varg_binicileri"))==0)
            {
                orkLegi.varg_binicileri.kritik_sans =orkLegi.varg_binicileri.kritik_sans+((orkLegi.varg_binicileri.kritik_sans*orkLegi.Kara_Troll.etki_deger)/100);
                printf ("KARA TROLLER BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.varg_binicileri.kritik_sans);
            }
            if ((strcmp(orkLegi.Kara_Troll.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.kritik_sans =orkLegi.ork_dovusculeri.kritik_sans+((orkLegi.ork_dovusculeri.kritik_sans*orkLegi.Kara_Troll.etki_deger)/100);
                printf ("KARA TROLLER BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.kritik_sans);
            }
            if ((strcmp(orkLegi.Kara_Troll.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.kritik_sans =orkLegi.mizrakcilar.kritik_sans+((orkLegi.mizrakcilar.kritik_sans*orkLegi.Kara_Troll.etki_deger)/100);
                printf ("KARA TROLLER KRITIK GUCU SUVARI : %f ", orkLegi.mizrakcilar.kritik_sans);
            }
            if ((strcmp(orkLegi.Kara_Troll.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.kritik_sans =orkLegi.troller.kritik_sans+((orkLegi.troller.kritik_sans*orkLegi.Kara_Troll.etki_deger)/100);
                printf ("KARA TROLLER BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.troller.kritik_sans);
            }
        }
        if(strcmp(orkLegi.Kara_Troll.etki_turu,"savunma")==0)
        {
            if ((strcmp(orkLegi.Kara_Troll.etkiledigi_birim,"varg_binicilerine"))==0)
            {
                orkLegi.varg_binicileri.savunma =orkLegi.varg_binicileri.savunma+((orkLegi.varg_binicileri.savunma*orkLegi.Kara_Troll.etki_deger)/100);
                printf ("KARA TROLLER BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.varg_binicileri.savunma);
            }
            if ((strcmp(orkLegi.Kara_Troll.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.savunma=orkLegi.ork_dovusculeri.savunma+((orkLegi.ork_dovusculeri.savunma*orkLegi.Kara_Troll.etki_deger)/100);
                printf ("KARA TROLLER BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.savunma);
            }
            if ((strcmp(orkLegi.Kara_Troll.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.savunma =orkLegi.mizrakcilar.savunma+((orkLegi.mizrakcilar.savunma*orkLegi.Kara_Troll.etki_deger)/100);
                printf ("KARA TROLLER BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.mizrakcilar.savunma);
            }
            if ((strcmp(orkLegi.Kara_Troll.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.savunma =orkLegi.troller.savunma+((orkLegi.troller.savunma*orkLegi.Kara_Troll.etki_deger)/100);
                printf ("KARA TROLLER BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.troller.savunma);
            }
        }
        if(strcmp(orkLegi.Kara_Troll.etki_turu,"saldiri")==0)
        {
            if ((strcmp(orkLegi.Kara_Troll.etkiledigi_birim,"varg_binicilerine"))==0)
            {
                orkLegi.varg_binicileri.saldiri =orkLegi.varg_binicileri.saldiri+((orkLegi.varg_binicileri.saldiri*orkLegi.Kara_Troll.etki_deger)/100);
                printf ("KARA TROLLER BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.varg_binicileri.saldiri);
            }
            if ((strcmp(orkLegi.Kara_Troll.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.saldiri =orkLegi.ork_dovusculeri.saldiri+((orkLegi.ork_dovusculeri.saldiri*orkLegi.Kara_Troll.etki_deger)/100);
                printf ("KARA TROLLER BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.saldiri);
            }
            if ((strcmp(orkLegi.Kara_Troll.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.saldiri =orkLegi.mizrakcilar.saldiri+((orkLegi.mizrakcilar.saldiri*orkLegi.Kara_Troll.etki_deger)/100);
                printf ("KARA TROLLER BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.mizrakcilar.saldiri);
            }
            if ((strcmp(orkLegi.Kara_Troll.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.saldiri =orkLegi.troller.saldiri+((orkLegi.troller.saldiri*orkLegi.Kara_Troll.etki_deger)/100);
                printf ("KARA TROLLER BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.troller.saldiri);
            }
        }
    }
    if ((strcmp(CanavarismiOrk,"Camur_Devleri"))==0)
    {
        if(strcmp(orkLegi.Camur_Devleri.etki_turu,"kritik_sans")==0)
        {
            if ((strcmp(orkLegi.Camur_Devleri.etkiledigi_birim,"varg_binicileri"))==0)
            {
                orkLegi.varg_binicileri.kritik_sans =orkLegi.varg_binicileri.kritik_sans+((orkLegi.varg_binicileri.kritik_sans*orkLegi.Camur_Devleri.etki_deger)/100);
                printf ("CAMUR DEVLERI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.varg_binicileri.kritik_sans);
            }
            if ((strcmp(orkLegi.Camur_Devleri.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.kritik_sans =orkLegi.ork_dovusculeri.kritik_sans+((orkLegi.ork_dovusculeri.kritik_sans*orkLegi.Camur_Devleri.etki_deger)/100);
                printf ("CAMUR DEVLERI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.kritik_sans);
            }
            if ((strcmp(orkLegi.Camur_Devleri.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.kritik_sans =orkLegi.mizrakcilar.kritik_sans+((orkLegi.mizrakcilar.kritik_sans*orkLegi.Camur_Devleri.etki_deger)/100);
                printf ("CAMUR DEVLERI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.mizrakcilar.kritik_sans);
            }
            if ((strcmp(orkLegi.Camur_Devleri.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.kritik_sans =orkLegi.troller.kritik_sans+((orkLegi.troller.kritik_sans*orkLegi.Camur_Devleri.etki_deger)/100);
                printf ("CAMUR DEVLERI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.troller.kritik_sans);
            }
        }
        if(strcmp(orkLegi.Camur_Devleri.etki_turu,"savunma")==0)
        {
            if ((strcmp(orkLegi.Camur_Devleri.etkiledigi_birim,"varg_binicilerine"))==0)
            {
                orkLegi.varg_binicileri.savunma =orkLegi.varg_binicileri.savunma+((orkLegi.varg_binicileri.savunma*orkLegi.Camur_Devleri.etki_deger)/100);
                printf ("CAMUR DEVLERI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.varg_binicileri.savunma);
            }
            if ((strcmp(orkLegi.Camur_Devleri.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.savunma=orkLegi.ork_dovusculeri.savunma+((orkLegi.ork_dovusculeri.savunma*orkLegi.Camur_Devleri.etki_deger)/100);
                printf ("CAMUR DEVLERI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.savunma);
            }
            if ((strcmp(orkLegi.Camur_Devleri.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.savunma =orkLegi.mizrakcilar.savunma+((orkLegi.mizrakcilar.savunma*orkLegi.Camur_Devleri.etki_deger)/100);
                printf ("CAMUR DEVLERI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.mizrakcilar.savunma);
            }
            if ((strcmp(orkLegi.Camur_Devleri.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.savunma =orkLegi.troller.savunma+((orkLegi.troller.savunma*orkLegi.Camur_Devleri.etki_deger)/100);
                printf ("CAMUR DEVLERI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.troller.savunma);
            }
        }
        if(strcmp(orkLegi.Camur_Devleri.etki_turu,"saldiri")==0)
        {
            if ((strcmp(orkLegi.Camur_Devleri.etkiledigi_birim,"varg_binicilerine"))==0)
            {
                orkLegi.varg_binicileri.saldiri =orkLegi.varg_binicileri.saldiri+((orkLegi.varg_binicileri.saldiri*orkLegi.Camur_Devleri.etki_deger)/100);
                printf ("CAMUR DEVLERI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.varg_binicileri.saldiri);
            }
            if ((strcmp(orkLegi.Camur_Devleri.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.saldiri =orkLegi.ork_dovusculeri.saldiri+((orkLegi.ork_dovusculeri.saldiri*orkLegi.Camur_Devleri.etki_deger)/100);
                printf ("CAMUR DEVLERI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.saldiri);
            }
            if ((strcmp(orkLegi.Camur_Devleri.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.saldiri =orkLegi.mizrakcilar.saldiri+((orkLegi.mizrakcilar.saldiri*orkLegi.Camur_Devleri.etki_deger)/100);
                printf ("CAMUR DEVLERI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.mizrakcilar.saldiri);
            }
            if ((strcmp(orkLegi.Camur_Devleri.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.saldiri =orkLegi.troller.saldiri+((orkLegi.troller.saldiri*orkLegi.Camur_Devleri.etki_deger)/100);
                printf ("CAMUR DEVLERI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.troller.saldiri);
            }
        }
    }
    if ((strcmp(CanavarismiOrk,"Makrog_Savas_Beyi"))==0)
    {
        if(strcmp(orkLegi.Makrog_Savas_Beyi.etki_turu,"kritik_sans")==0)
        {
            if ((strcmp(orkLegi.Makrog_Savas_Beyi.etkiledigi_birim,"varg_binicileri"))==0)
            {
                orkLegi.varg_binicileri.kritik_sans =orkLegi.varg_binicileri.kritik_sans+((orkLegi.varg_binicileri.kritik_sans*orkLegi.Makrog_Savas_Beyi.etki_deger)/100);
                printf ("MAKROG SAVAS BEYI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.varg_binicileri.kritik_sans);
            }
            if ((strcmp(orkLegi.Makrog_Savas_Beyi.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.kritik_sans =orkLegi.ork_dovusculeri.kritik_sans+((orkLegi.ork_dovusculeri.kritik_sans*orkLegi.Makrog_Savas_Beyi.etki_deger)/100);
                printf ("MAKROG SAVAS BEYI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.kritik_sans);
            }
            if ((strcmp(orkLegi.Makrog_Savas_Beyi.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.kritik_sans =orkLegi.mizrakcilar.kritik_sans+((orkLegi.mizrakcilar.kritik_sans*orkLegi.Makrog_Savas_Beyi.etki_deger)/100);
                printf ("MAKROG SAVAS BEYI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.mizrakcilar.kritik_sans);
            }
            if ((strcmp(orkLegi.Makrog_Savas_Beyi.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.kritik_sans =orkLegi.troller.kritik_sans+((orkLegi.troller.kritik_sans*orkLegi.Makrog_Savas_Beyi.etki_deger)/100);
                printf ("MAKROG SAVAS BEYI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.troller.kritik_sans);
            }
        }
        if(strcmp(orkLegi.Makrog_Savas_Beyi.etki_turu,"savunma")==0)
        {
            if ((strcmp(orkLegi.Makrog_Savas_Beyi.etkiledigi_birim,"varg_binicilerine"))==0)
            {
                orkLegi.varg_binicileri.savunma =orkLegi.varg_binicileri.savunma+((orkLegi.varg_binicileri.savunma*orkLegi.Makrog_Savas_Beyi.etki_deger)/100);
                printf ("MAKROG SAVAS BEYI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.varg_binicileri.savunma);
            }
            if ((strcmp(orkLegi.Makrog_Savas_Beyi.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.savunma=orkLegi.ork_dovusculeri.savunma+((orkLegi.ork_dovusculeri.savunma*orkLegi.Makrog_Savas_Beyi.etki_deger)/100);
                printf ("MAKROG SAVAS BEYI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.savunma);
            }
            if ((strcmp(orkLegi.Makrog_Savas_Beyi.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.savunma =orkLegi.mizrakcilar.savunma+((orkLegi.mizrakcilar.savunma*orkLegi.Makrog_Savas_Beyi.etki_deger)/100);
                printf ("MAKROG SAVAS BEYI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.mizrakcilar.savunma);
            }
            if ((strcmp(orkLegi.Makrog_Savas_Beyi.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.savunma =orkLegi.troller.savunma+((orkLegi.troller.savunma*orkLegi.Makrog_Savas_Beyi.etki_deger)/100);
                printf ("MAKROG SAVAS BEYI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.troller.savunma);
            }
        }
        if(strcmp(orkLegi.Makrog_Savas_Beyi.etki_turu,"saldiri")==0)
        {
            if ((strcmp(orkLegi.Makrog_Savas_Beyi.etkiledigi_birim,"varg_binicilerine"))==0)
            {
                orkLegi.varg_binicileri.saldiri =orkLegi.varg_binicileri.saldiri+((orkLegi.varg_binicileri.saldiri*orkLegi.Makrog_Savas_Beyi.etki_deger)/100);
                printf ("MAKROG SAVAS BEYI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.varg_binicileri.saldiri);
            }
            if ((strcmp(orkLegi.Makrog_Savas_Beyi.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.saldiri =orkLegi.ork_dovusculeri.saldiri+((orkLegi.ork_dovusculeri.saldiri*orkLegi.Makrog_Savas_Beyi.etki_deger)/100);
                printf ("MAKROG SAVAS BEYI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.saldiri);
            }
            if ((strcmp(orkLegi.Makrog_Savas_Beyi.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.saldiri =orkLegi.mizrakcilar.saldiri+((orkLegi.mizrakcilar.saldiri*orkLegi.Makrog_Savas_Beyi.etki_deger)/100);
                printf ("MAKROG SAVAS BEYI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.mizrakcilar.saldiri);
            }
            if ((strcmp(orkLegi.Makrog_Savas_Beyi.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.saldiri =orkLegi.troller.saldiri+((orkLegi.troller.saldiri*orkLegi.Makrog_Savas_Beyi.etki_deger)/100);
                printf ("MAKROG SAVAS BEYI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.troller.saldiri);
            }
        }
    }
    if ((strcmp(CanavarismiOrk,"Buz_Devleri"))==0)
    {
        if(strcmp(orkLegi.Buz_Devleri.etki_turu,"kritik_sans")==0)
        {
            if ((strcmp(orkLegi.Buz_Devleri.etkiledigi_birim,"varg_binicileri"))==0)
            {
                orkLegi.varg_binicileri.kritik_sans =orkLegi.varg_binicileri.kritik_sans+((orkLegi.varg_binicileri.kritik_sans*orkLegi.Buz_Devleri.etki_deger)/100);
                printf ("BUZ DEVLERI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.varg_binicileri.kritik_sans);
            }
            if ((strcmp(orkLegi.Buz_Devleri.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.kritik_sans =orkLegi.ork_dovusculeri.kritik_sans+((orkLegi.ork_dovusculeri.kritik_sans*orkLegi.Buz_Devleri.etki_deger)/100);
                printf ("BUZ DEVLERI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.kritik_sans);
            }
            if ((strcmp(orkLegi.Buz_Devleri.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.kritik_sans =orkLegi.mizrakcilar.kritik_sans+((orkLegi.mizrakcilar.kritik_sans*orkLegi.Buz_Devleri.etki_deger)/100);
                printf ("BUZ DEVLERI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.mizrakcilar.kritik_sans);
            }
            if ((strcmp(orkLegi.Buz_Devleri.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.kritik_sans =orkLegi.troller.kritik_sans+((orkLegi.troller.kritik_sans*orkLegi.Buz_Devleri.etki_deger)/100);
                printf ("BUZ DEVLERI BOOSTLU KRITIK GUCU SUVARI : %f ", orkLegi.troller.kritik_sans);
            }
        }
        if(strcmp(orkLegi.Buz_Devleri.etki_turu,"savunma")==0)
        {
            if ((strcmp(orkLegi.Buz_Devleri.etkiledigi_birim,"varg_binicilerine"))==0)
            {
                orkLegi.varg_binicileri.savunma =orkLegi.varg_binicileri.savunma+((orkLegi.varg_binicileri.savunma*orkLegi.Buz_Devleri.etki_deger)/100);
                printf ("BUZ DEVLERI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.varg_binicileri.savunma);
            }
            if ((strcmp(orkLegi.Buz_Devleri.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.savunma=orkLegi.ork_dovusculeri.savunma+((orkLegi.ork_dovusculeri.savunma*orkLegi.Buz_Devleri.etki_deger)/100);
                printf ("BUZ DEVLERI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.savunma);
            }
            if ((strcmp(orkLegi.Buz_Devleri.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.savunma =orkLegi.mizrakcilar.savunma+((orkLegi.mizrakcilar.savunma*orkLegi.Buz_Devleri.etki_deger)/100);
                printf ("BUZ DEVLERI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.mizrakcilar.savunma);
            }
            if ((strcmp(orkLegi.Buz_Devleri.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.savunma =orkLegi.troller.savunma+((orkLegi.troller.savunma*orkLegi.Buz_Devleri.etki_deger)/100);
                printf ("BUZ DEVLERI BOOSTLU SAVUNMA GUCU SUVARI : %f ", orkLegi.troller.savunma);
            }
        }
        if(strcmp(orkLegi.Buz_Devleri.etki_turu,"saldiri")==0)
        {
            if ((strcmp(orkLegi.Buz_Devleri.etkiledigi_birim,"varg_binicilerine"))==0)
            {
                orkLegi.varg_binicileri.saldiri =orkLegi.varg_binicileri.saldiri+((orkLegi.varg_binicileri.saldiri*orkLegi.Buz_Devleri.etki_deger)/100);
                printf ("BUZ DEVLERI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.varg_binicileri.saldiri);
            }
            if ((strcmp(orkLegi.Buz_Devleri.etkiledigi_birim,"ork_dovusculeri"))==0)
            {
                orkLegi.ork_dovusculeri.saldiri =orkLegi.ork_dovusculeri.saldiri+((orkLegi.ork_dovusculeri.saldiri*orkLegi.Buz_Devleri.etki_deger)/100);
                printf ("BUZ DEVLERI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.ork_dovusculeri.saldiri);
            }
            if ((strcmp(orkLegi.Buz_Devleri.etkiledigi_birim,"mizrakcilar"))==0)
            {
                orkLegi.mizrakcilar.saldiri =orkLegi.mizrakcilar.saldiri+((orkLegi.mizrakcilar.saldiri*orkLegi.Buz_Devleri.etki_deger)/100);
                printf ("BUZ DEVLERI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.mizrakcilar.saldiri);
            }
            if ((strcmp(orkLegi.Buz_Devleri.etkiledigi_birim,"troller"))==0)
            {
                orkLegi.troller.saldiri =orkLegi.troller.saldiri+((orkLegi.troller.saldiri*orkLegi.Buz_Devleri.etki_deger)/100);
                printf ("BUZ DEVLERI BOOSTLU SALDIRI GUCU SUVARI : %f ", orkLegi.troller.saldiri);
            }
        }
    }
    if (strcmp(CanavarismiInsan,"Ejderha")==0)
    {
        printf("\n");
        if (strcmp(insanImparatorlugu.Ejderha.etki_turu,"saldiri")==0)
        {
            if (strcmp(insanImparatorlugu.Ejderha.etkiledigi_birim,"piyade")==0)
            {
                insanImparatorlugu.piyadeler.saldiri=insanImparatorlugu.piyadeler.saldiri+((insanImparatorlugu.piyadeler.saldiri*insanImparatorlugu.Ejderha.etki_deger)/100) ;
                printf ("Ejderha saldiri boostlu piyade gucu :%f", insanImparatorlugu.piyadeler.saldiri) ;
            }
            if (strcmp(insanImparatorlugu.Ejderha.etkiledigi_birim,"okcular")==0)
            {
                insanImparatorlugu.okcular.saldiri=insanImparatorlugu.okcular.saldiri+((insanImparatorlugu.okcular.saldiri*insanImparatorlugu.Ejderha.etki_deger)/100) ;
                printf ("Ejderha saldiri boostlu okcular gucu :%f", insanImparatorlugu.okcular.saldiri) ;
            }
            if (strcmp(insanImparatorlugu.Ejderha.etkiledigi_birim,"suvariler")==0)
            {
                insanImparatorlugu.suvariler.saldiri=insanImparatorlugu.suvariler.saldiri+((insanImparatorlugu.suvariler.saldiri*insanImparatorlugu.Ejderha.etki_deger)/100) ;
                printf ("Ejderha saldiri boostlu suvariler gucu :%f", insanImparatorlugu.suvariler.saldiri) ;
            }
            if (strcmp(insanImparatorlugu.Ejderha.etkiledigi_birim,"kusatma_makineleri")==0)
            {
                insanImparatorlugu.kusatma_makineleri.saldiri=insanImparatorlugu.kusatma_makineleri.saldiri+((insanImparatorlugu.kusatma_makineleri.saldiri*insanImparatorlugu.Ejderha.etki_deger)/100) ;
                printf ("Ejderha saldiri boostlu kusatma makine gucu :%f", insanImparatorlugu.kusatma_makineleri.saldiri) ;
            }
        }
        if (strcmp(insanImparatorlugu.Ejderha.etki_turu,"savunma")==0)
        {
            if (strcmp(insanImparatorlugu.Ejderha.etkiledigi_birim,"piyade")==0)
            {
                insanImparatorlugu.piyadeler.savunma=insanImparatorlugu.piyadeler.savunma+((insanImparatorlugu.piyadeler.savunma*insanImparatorlugu.Ejderha.etki_deger)/100) ;
                printf ("Ejderha savunma boostlu piyade gucu :%f", insanImparatorlugu.piyadeler.savunma) ;
            }
            if (strcmp(insanImparatorlugu.Ejderha.etkiledigi_birim,"okcular")==0)
            {
                insanImparatorlugu.okcular.savunma=insanImparatorlugu.okcular.savunma+((insanImparatorlugu.okcular.savunma*insanImparatorlugu.Ejderha.etki_deger)/100) ;
                printf ("Ejderha savunma boostlu okcular gucu :%f", insanImparatorlugu.okcular.savunma) ;
            }
            if (strcmp(insanImparatorlugu.Ejderha.etkiledigi_birim,"suvariler")==0)
            {
                insanImparatorlugu.suvariler.savunma=insanImparatorlugu.suvariler.savunma+((insanImparatorlugu.suvariler.savunma*insanImparatorlugu.Ejderha.etki_deger)/100) ;
                printf ("Ejderha savunma boostlu suvariler gucu :%f", insanImparatorlugu.suvariler.savunma) ;
            }
            if (strcmp(insanImparatorlugu.Ejderha.etkiledigi_birim,"kusatma_makineleri")==0)
            {
                insanImparatorlugu.kusatma_makineleri.savunma=insanImparatorlugu.kusatma_makineleri.savunma+((insanImparatorlugu.kusatma_makineleri.savunma*insanImparatorlugu.Ejderha.etki_deger)/100) ;
                printf ("Ejderha savunma boostlu kusatma makine gucu :%f", insanImparatorlugu.kusatma_makineleri.savunma) ;
            }
            if (strcmp(insanImparatorlugu.Ejderha.etki_turu,"kritik_sans")==0)
            {
                if (strcmp(insanImparatorlugu.Ejderha.etkiledigi_birim,"piyade")==0)
                {
                    insanImparatorlugu.piyadeler.kritik_sans=insanImparatorlugu.piyadeler.kritik_sans+((insanImparatorlugu.piyadeler.kritik_sans*insanImparatorlugu.Ejderha.etki_deger)/100) ;
                    printf ("Ejderha kritik sans boostlu piyade gucu :%f", insanImparatorlugu.piyadeler.kritik_sans) ;
                }
                if (strcmp(insanImparatorlugu.Ejderha.etkiledigi_birim,"okcular")==0)
                {
                    insanImparatorlugu.okcular.kritik_sans=insanImparatorlugu.okcular.kritik_sans+((insanImparatorlugu.okcular.kritik_sans*insanImparatorlugu.Ejderha.etki_deger)/100) ;
                    printf ("Ejderha kritik sans boostlu okcular gucu :%f", insanImparatorlugu.okcular.kritik_sans) ;
                }
                if (strcmp(insanImparatorlugu.Ejderha.etkiledigi_birim,"suvariler")==0)
                {
                    insanImparatorlugu.suvariler.kritik_sans=insanImparatorlugu.suvariler.kritik_sans+((insanImparatorlugu.suvariler.kritik_sans*insanImparatorlugu.Ejderha.etki_deger)/100) ;
                    printf ("Ejderha kritik sans boostlu suvariler gucu :%f", insanImparatorlugu.suvariler.kritik_sans) ;
                }
                if (strcmp(insanImparatorlugu.Ejderha.etkiledigi_birim,"kusatma_makineleri")==0)
                {
                    insanImparatorlugu.kusatma_makineleri.kritik_sans=insanImparatorlugu.kusatma_makineleri.kritik_sans+((insanImparatorlugu.kusatma_makineleri.kritik_sans*insanImparatorlugu.Ejderha.etki_deger)/100) ;
                    printf ("Ejderha kritik sans boostlu kusatma makine gucu :%f", insanImparatorlugu.kusatma_makineleri.kritik_sans) ;
                }
            }
        }
    }
    if (strcmp(CanavarismiInsan,"Agri_Dagi_Devleri")==0)
    {
        if (strcmp(insanImparatorlugu.Agri_Dagi_Devleri.etki_turu,"saldiri")==0)
        {
            if (strcmp(insanImparatorlugu.Agri_Dagi_Devleri.etkiledigi_birim,"piyade")==0)
            {
                insanImparatorlugu.piyadeler.saldiri=insanImparatorlugu.piyadeler.saldiri+((insanImparatorlugu.piyadeler.saldiri*insanImparatorlugu.Agri_Dagi_Devleri.etki_deger)/100) ;
                printf ("Agri_Dagi_Devleri saldiri boostlu piyade gucu :%f", insanImparatorlugu.piyadeler.saldiri) ;
            }
            if (strcmp(insanImparatorlugu.Agri_Dagi_Devleri.etkiledigi_birim,"okcular")==0)
            {
                insanImparatorlugu.okcular.saldiri=insanImparatorlugu.okcular.saldiri+((insanImparatorlugu.okcular.saldiri*insanImparatorlugu.Agri_Dagi_Devleri.etki_deger)/100) ;
                printf ("Agri_Dagi_Devleri saldiri boostlu okcular gucu :%f", insanImparatorlugu.okcular.saldiri) ;
            }
            if (strcmp(insanImparatorlugu.Agri_Dagi_Devleri.etkiledigi_birim,"suvariler")==0)
            {
                insanImparatorlugu.suvariler.saldiri=insanImparatorlugu.suvariler.saldiri+((insanImparatorlugu.suvariler.saldiri*insanImparatorlugu.Agri_Dagi_Devleri.etki_deger)/100) ;
                printf ("Agri_Dagi_Devleri saldiri boostlu suvariler gucu :%f", insanImparatorlugu.suvariler.saldiri) ;
            }
            if (strcmp(insanImparatorlugu.Agri_Dagi_Devleri.etkiledigi_birim,"kusatma_makineleri")==0)
            {
                insanImparatorlugu.kusatma_makineleri.saldiri=insanImparatorlugu.kusatma_makineleri.saldiri+((insanImparatorlugu.kusatma_makineleri.saldiri*insanImparatorlugu.Agri_Dagi_Devleri.etki_deger)/100) ;
                printf ("Agri_Dagi_Devleri saldiri boostlu kusatma makine gucu :%f", insanImparatorlugu.kusatma_makineleri.saldiri) ;
            }
        }
        if (strcmp(insanImparatorlugu.Agri_Dagi_Devleri.etki_turu,"savunma")==0)
        {
            if (strcmp(insanImparatorlugu.Agri_Dagi_Devleri.etkiledigi_birim,"piyade")==0)
            {
                insanImparatorlugu.piyadeler.savunma=insanImparatorlugu.piyadeler.savunma+((insanImparatorlugu.piyadeler.savunma*insanImparatorlugu.Agri_Dagi_Devleri.etki_deger)/100) ;
                printf ("Agri_Dagi_Devleri savunma boostlu piyade gucu :%f", insanImparatorlugu.piyadeler.savunma) ;
            }
            if (strcmp(insanImparatorlugu.Agri_Dagi_Devleri.etkiledigi_birim,"okcular")==0)
            {
                insanImparatorlugu.okcular.savunma=insanImparatorlugu.okcular.savunma+((insanImparatorlugu.okcular.savunma*insanImparatorlugu.Agri_Dagi_Devleri.etki_deger)/100) ;
                printf ("Agri_Dagi_Devleri savunma boostlu okcular gucu :%f", insanImparatorlugu.okcular.savunma) ;
            }
            if (strcmp(insanImparatorlugu.Agri_Dagi_Devleri.etkiledigi_birim,"suvariler")==0)
            {
                insanImparatorlugu.suvariler.savunma=insanImparatorlugu.suvariler.savunma+((insanImparatorlugu.suvariler.savunma*insanImparatorlugu.Agri_Dagi_Devleri.etki_deger)/100) ;
                printf ("Agri_Dagi_Devleri savunma boostlu suvariler gucu :%f", insanImparatorlugu.suvariler.savunma) ;
            }
            if (strcmp(insanImparatorlugu.Agri_Dagi_Devleri.etkiledigi_birim,"kusatma_makineleri")==0)
            {
                insanImparatorlugu.kusatma_makineleri.savunma=insanImparatorlugu.kusatma_makineleri.savunma+((insanImparatorlugu.kusatma_makineleri.savunma*insanImparatorlugu.Agri_Dagi_Devleri.etki_deger)/100) ;
                printf ("Agri_Dagi_Devleri savunma boostlu kusatma makine gucu :%f", insanImparatorlugu.kusatma_makineleri.savunma) ;
            }
        }
        if (strcmp(insanImparatorlugu.Agri_Dagi_Devleri.etki_turu,"kritik_sans")==0)
        {
            if (strcmp(insanImparatorlugu.Agri_Dagi_Devleri.etkiledigi_birim,"piyade")==0)
            {
                insanImparatorlugu.piyadeler.kritik_sans=insanImparatorlugu.piyadeler.kritik_sans+((insanImparatorlugu.piyadeler.kritik_sans*insanImparatorlugu.Agri_Dagi_Devleri.etki_deger)/100) ;
                printf ("Agri_Dagi_Devleri kritik sans boostlu piyade gucu :%f", insanImparatorlugu.piyadeler.kritik_sans) ;
            }
            if (strcmp(insanImparatorlugu.Agri_Dagi_Devleri.etkiledigi_birim,"okcular")==0)
            {
                insanImparatorlugu.okcular.kritik_sans=insanImparatorlugu.okcular.kritik_sans+((insanImparatorlugu.okcular.kritik_sans*insanImparatorlugu.Agri_Dagi_Devleri.etki_deger)/100) ;
                printf ("Agri_Dagi_Devleri kritik sans boostlu okcular gucu :%f", insanImparatorlugu.okcular.kritik_sans) ;
            }
            if (strcmp(insanImparatorlugu.Agri_Dagi_Devleri.etkiledigi_birim,"suvariler")==0)
            {
                insanImparatorlugu.suvariler.kritik_sans=insanImparatorlugu.suvariler.kritik_sans+((insanImparatorlugu.suvariler.kritik_sans*insanImparatorlugu.Agri_Dagi_Devleri.etki_deger)/100) ;
                printf ("Agri_Dagi_Devleri kritik sans boostlu suvariler gucu :%f", insanImparatorlugu.suvariler.kritik_sans) ;
            }

            if (strcmp(insanImparatorlugu.Agri_Dagi_Devleri.etkiledigi_birim,"kusatma_makineleri")==0)
            {
                insanImparatorlugu.kusatma_makineleri.kritik_sans=insanImparatorlugu.kusatma_makineleri.kritik_sans+((insanImparatorlugu.kusatma_makineleri.kritik_sans*insanImparatorlugu.Agri_Dagi_Devleri.etki_deger)/100) ;
                printf ("Agri_Dagi_Devleri kritik sans boostlu kusatma makine gucu :%f", insanImparatorlugu.kusatma_makineleri.kritik_sans) ;
            }
        }
    }
    if (strcmp(CanavarismiInsan,"Tepegoz")==0)
    {
        if (strcmp(insanImparatorlugu.Tepegoz.etki_turu,"saldiri")==0)
        {
            if (strcmp(insanImparatorlugu.Tepegoz.etkiledigi_birim,"piyade")==0)
            {
                insanImparatorlugu.piyadeler.saldiri=insanImparatorlugu.piyadeler.saldiri+((insanImparatorlugu.piyadeler.saldiri*insanImparatorlugu.Tepegoz.etki_deger)/100) ;
                printf ("Tepegoz saldiri boostlu piyade gucu :%f", insanImparatorlugu.piyadeler.saldiri) ;
            }
            if (strcmp(insanImparatorlugu.Tepegoz.etkiledigi_birim,"okcu")==0)
            {
                insanImparatorlugu.okcular.saldiri=insanImparatorlugu.okcular.saldiri+((insanImparatorlugu.okcular.saldiri*insanImparatorlugu.Tepegoz.etki_deger)/100) ;
                printf ("Tepegoz saldiri boostlu okcular gucu :%f", insanImparatorlugu.okcular.saldiri) ;
            }
            if (strcmp(insanImparatorlugu.Tepegoz.etkiledigi_birim,"suvari")==0)
            {
                insanImparatorlugu.suvariler.saldiri=insanImparatorlugu.suvariler.saldiri+((insanImparatorlugu.suvariler.saldiri*insanImparatorlugu.Tepegoz.etki_deger)/100) ;
                printf ("Tepegoz saldiri boostlu suvariler gucu :%f", insanImparatorlugu.suvariler.saldiri) ;
            }
            if (strcmp(insanImparatorlugu.Tepegoz.etkiledigi_birim,"kusatma_makineleri")==0)
            {
                insanImparatorlugu.kusatma_makineleri.saldiri=insanImparatorlugu.kusatma_makineleri.saldiri+((insanImparatorlugu.kusatma_makineleri.saldiri*insanImparatorlugu.Tepegoz.etki_deger)/100) ;
                printf ("Tepegoz saldiri boostlu kusatma makine gucu :%f", insanImparatorlugu.kusatma_makineleri.saldiri) ;
            }
        }
        if (strcmp(insanImparatorlugu.Tepegoz.etki_turu,"savunma")==0)
        {
            if (strcmp(insanImparatorlugu.Tepegoz.etkiledigi_birim,"piyade")==0)
            {
                insanImparatorlugu.piyadeler.savunma=insanImparatorlugu.piyadeler.savunma+((insanImparatorlugu.piyadeler.savunma*insanImparatorlugu.Tepegoz.etki_deger)/100) ;
                printf ("Tepegoz savunma boostlu piyade gucu :%f", insanImparatorlugu.piyadeler.savunma) ;
            }
            if (strcmp(insanImparatorlugu.Tepegoz.etkiledigi_birim,"okcular")==0)
            {
                insanImparatorlugu.okcular.savunma=insanImparatorlugu.okcular.savunma+((insanImparatorlugu.okcular.savunma*insanImparatorlugu.Tepegoz.etki_deger)/100) ;
                printf ("Tepegoz savunma boostlu okcular gucu :%f", insanImparatorlugu.okcular.savunma) ;
            }
            if (strcmp(insanImparatorlugu.Tepegoz.etkiledigi_birim,"suvariler")==0)
            {
                insanImparatorlugu.suvariler.savunma=insanImparatorlugu.suvariler.savunma+((insanImparatorlugu.suvariler.savunma*insanImparatorlugu.Tepegoz.etki_deger)/100) ;
                printf ("Tepegoz savunma boostlu suvariler gucu :%f", insanImparatorlugu.suvariler.savunma) ;
            }
            if (strcmp(insanImparatorlugu.Tepegoz.etkiledigi_birim,"kusatma_makineleri")==0)
            {
                insanImparatorlugu.kusatma_makineleri.savunma=insanImparatorlugu.kusatma_makineleri.savunma+((insanImparatorlugu.kusatma_makineleri.savunma*insanImparatorlugu.Tepegoz.etki_deger)/100) ;
                printf ("Tepegoz savunma boostlu kusatma makine gucu :%f", insanImparatorlugu.kusatma_makineleri.savunma) ;
            }
        }
        if (strcmp(insanImparatorlugu.Tepegoz.etki_turu,"kritik_sans")==0)
        {
            if (strcmp(insanImparatorlugu.Tepegoz.etkiledigi_birim,"piyade")==0)
            {
                insanImparatorlugu.piyadeler.kritik_sans=insanImparatorlugu.piyadeler.kritik_sans+((insanImparatorlugu.piyadeler.kritik_sans*insanImparatorlugu.Tepegoz.etki_deger)/100) ;
                printf ("Tepegoz kritik sans boostlu piyade gucu :%f", insanImparatorlugu.piyadeler.kritik_sans) ;
            }
            if (strcmp(insanImparatorlugu.Tepegoz.etkiledigi_birim,"okcular")==0)
            {
                insanImparatorlugu.okcular.kritik_sans=insanImparatorlugu.okcular.kritik_sans+((insanImparatorlugu.okcular.kritik_sans*insanImparatorlugu.Tepegoz.etki_deger)/100) ;
                printf ("Tepegoz kritik sans boostlu okcular gucu :%f", insanImparatorlugu.okcular.kritik_sans) ;
            }
            if (strcmp(insanImparatorlugu.Tepegoz.etkiledigi_birim,"suvariler")==0)
            {
                insanImparatorlugu.suvariler.kritik_sans=insanImparatorlugu.suvariler.kritik_sans+((insanImparatorlugu.suvariler.kritik_sans*insanImparatorlugu.Tepegoz.etki_deger)/100) ;
                printf ("Tepegoz kritik sans boostlu suvariler gucu :%f", insanImparatorlugu.suvariler.kritik_sans) ;
            }
            if (strcmp(insanImparatorlugu.Tepegoz.etkiledigi_birim,"kusatma_makineleri")==0)
            {
                insanImparatorlugu.kusatma_makineleri.kritik_sans=insanImparatorlugu.kusatma_makineleri.kritik_sans+((insanImparatorlugu.kusatma_makineleri.kritik_sans*insanImparatorlugu.Tepegoz.etki_deger)/100) ;
                printf ("Tepegoz kritik sans boostlu kusatma makine gucu :%f", insanImparatorlugu.kusatma_makineleri.kritik_sans) ;
            }
        }
    }
    if(strcmp(CanavarismiInsan,"Karakurt")==0)
    {
        if (strcmp(insanImparatorlugu.Karakurt.etki_turu,"saldiri")==0)
        {
            if (strcmp(insanImparatorlugu.Karakurt.etkiledigi_birim,"piyade")==0)
            {
                insanImparatorlugu.piyadeler.saldiri=insanImparatorlugu.piyadeler.saldiri+((insanImparatorlugu.piyadeler.saldiri*insanImparatorlugu.Karakurt.etki_deger)/100) ;
                printf ("Karakurt saldiri boostlu piyade gucu :%f", insanImparatorlugu.piyadeler.saldiri) ;
            }
            if (strcmp(insanImparatorlugu.Karakurt.etkiledigi_birim,"okcular")==0)
            {
                insanImparatorlugu.okcular.saldiri=insanImparatorlugu.okcular.saldiri+((insanImparatorlugu.okcular.saldiri*insanImparatorlugu.Karakurt.etki_deger)/100) ;
                printf ("Karakurt saldiri boostlu okcular gucu :%f", insanImparatorlugu.okcular.saldiri) ;
            }
            if (strcmp(insanImparatorlugu.Karakurt.etkiledigi_birim,"suvariler")==0)
            {
                insanImparatorlugu.suvariler.saldiri=insanImparatorlugu.suvariler.saldiri+((insanImparatorlugu.suvariler.saldiri*insanImparatorlugu.Karakurt.etki_deger)/100) ;
                printf ("Karakurt saldiri boostlu suvariler gucu :%f", insanImparatorlugu.suvariler.saldiri) ;
            }
            if (strcmp(insanImparatorlugu.Karakurt.etkiledigi_birim,"kusatma_makineleri")==0)
            {
                insanImparatorlugu.kusatma_makineleri.saldiri=insanImparatorlugu.kusatma_makineleri.saldiri+((insanImparatorlugu.kusatma_makineleri.saldiri*insanImparatorlugu.Karakurt.etki_deger)/100) ;
                printf ("Karakurt saldiri boostlu kusatma makine gucu :%f", insanImparatorlugu.kusatma_makineleri.saldiri) ;
            }
        }
        if (strcmp(insanImparatorlugu.Karakurt.etki_turu,"savunma")==0)
        {
            if (strcmp(insanImparatorlugu.Karakurt.etkiledigi_birim,"piyade")==0)
            {
                insanImparatorlugu.piyadeler.savunma=insanImparatorlugu.piyadeler.savunma+((insanImparatorlugu.piyadeler.savunma*insanImparatorlugu.Karakurt.etki_deger)/100) ;
                printf ("Karakurt savunma boostlu piyade gucu :%f", insanImparatorlugu.piyadeler.savunma) ;
            }
            if (strcmp(insanImparatorlugu.Karakurt.etkiledigi_birim,"okcular")==0)
            {
                insanImparatorlugu.okcular.savunma=insanImparatorlugu.okcular.savunma+((insanImparatorlugu.okcular.savunma*insanImparatorlugu.Karakurt.etki_deger)/100) ;
                printf ("Karakurt savunma boostlu okcular gucu :%f", insanImparatorlugu.okcular.savunma) ;
            }
            if (strcmp(insanImparatorlugu.Karakurt.etkiledigi_birim,"suvariler")==0)
            {
                insanImparatorlugu.suvariler.savunma=insanImparatorlugu.suvariler.savunma+((insanImparatorlugu.suvariler.savunma*insanImparatorlugu.Karakurt.etki_deger)/100) ;
                printf ("Karakurt savunma boostlu suvariler gucu :%f", insanImparatorlugu.suvariler.savunma) ;
            }
            if (strcmp(insanImparatorlugu.Karakurt.etkiledigi_birim,"kusatma_makineleri")==0)
            {
                insanImparatorlugu.kusatma_makineleri.savunma=insanImparatorlugu.kusatma_makineleri.savunma+((insanImparatorlugu.kusatma_makineleri.savunma*insanImparatorlugu.Karakurt.etki_deger)/100) ;
                printf ("Karakurt savunma boostlu kusatma makine gucu :%f", insanImparatorlugu.kusatma_makineleri.savunma) ;
            }
        }
        if (strcmp(insanImparatorlugu.Karakurt.etki_turu,"kritik_sans")==0)
        {
            if (strcmp(insanImparatorlugu.Karakurt.etkiledigi_birim,"piyade")==0)
            {
                insanImparatorlugu.piyadeler.kritik_sans=insanImparatorlugu.piyadeler.kritik_sans+((insanImparatorlugu.piyadeler.kritik_sans*insanImparatorlugu.Karakurt.etki_deger)/100) ;
                printf ("Karakurt kritik sans boostlu piyade gucu :%f", insanImparatorlugu.piyadeler.kritik_sans) ;
            }
            if (strcmp(insanImparatorlugu.Karakurt.etkiledigi_birim,"okcular")==0)
            {
                insanImparatorlugu.okcular.kritik_sans=insanImparatorlugu.okcular.kritik_sans+((insanImparatorlugu.okcular.kritik_sans*insanImparatorlugu.Karakurt.etki_deger)/100) ;
                printf ("Karakurt kritik sans boostlu okcular gucu :%f", insanImparatorlugu.okcular.kritik_sans) ;
            }
            if (strcmp(insanImparatorlugu.Karakurt.etkiledigi_birim,"suvariler")==0)
            {
                insanImparatorlugu.suvariler.kritik_sans=insanImparatorlugu.suvariler.kritik_sans+((insanImparatorlugu.suvariler.kritik_sans*insanImparatorlugu.Karakurt.etki_deger)/100) ;
                printf ("Karakurt kritik sans boostlu suvariler gucu :%f", insanImparatorlugu.suvariler.kritik_sans) ;
            }
            if (strcmp(insanImparatorlugu.Karakurt.etkiledigi_birim,"kusatma_makineleri")==0)
            {
                insanImparatorlugu.kusatma_makineleri.kritik_sans=insanImparatorlugu.kusatma_makineleri.kritik_sans+((insanImparatorlugu.kusatma_makineleri.kritik_sans*insanImparatorlugu.Karakurt.etki_deger)/100) ;
                printf ("Karakurt kritik sans boostlu kusatma makine gucu :%f", insanImparatorlugu.kusatma_makineleri.kritik_sans) ;
            }
        }
    }
    if(strcmp(CanavarismiInsan,"Samur")==0)
    {
        if (strcmp(insanImparatorlugu.Samur.etki_turu,"saldiri")==0)
        {
            if (strcmp(insanImparatorlugu.Samur.etkiledigi_birim,"piyade")==0)
            {
                insanImparatorlugu.piyadeler.saldiri=insanImparatorlugu.piyadeler.saldiri+((insanImparatorlugu.piyadeler.saldiri*insanImparatorlugu.Samur.etki_deger)/100) ;
                printf ("Samur saldiri boostlu piyade gucu :%f", insanImparatorlugu.piyadeler.saldiri) ;
            }
            if (strcmp(insanImparatorlugu.Samur.etkiledigi_birim,"okcular")==0)
            {
                insanImparatorlugu.okcular.saldiri=insanImparatorlugu.okcular.saldiri+((insanImparatorlugu.okcular.saldiri*insanImparatorlugu.Samur.etki_deger)/100) ;
                printf ("Samur saldiri boostlu okcular gucu :%f", insanImparatorlugu.okcular.saldiri) ;
            }
            if (strcmp(insanImparatorlugu.Samur.etkiledigi_birim,"suvariler")==0)
            {
                insanImparatorlugu.suvariler.saldiri=insanImparatorlugu.suvariler.saldiri+((insanImparatorlugu.suvariler.saldiri*insanImparatorlugu.Samur.etki_deger)/100) ;
                printf ("Samur saldiri boostlu suvariler gucu :%f", insanImparatorlugu.suvariler.saldiri) ;
            }
            if (strcmp(insanImparatorlugu.Samur.etkiledigi_birim,"kusatma_makineleri")==0)
            {
                insanImparatorlugu.kusatma_makineleri.saldiri=insanImparatorlugu.kusatma_makineleri.saldiri+((insanImparatorlugu.kusatma_makineleri.saldiri*insanImparatorlugu.Samur.etki_deger)/100) ;
                printf ("Samur saldiri boostlu kusatma makine gucu :%f", insanImparatorlugu.kusatma_makineleri.saldiri) ;
            }
        }
        if (strcmp(insanImparatorlugu.Samur.etki_turu,"savunma")==0)
        {
            if (strcmp(insanImparatorlugu.Samur.etkiledigi_birim,"piyade")==0)
            {
                insanImparatorlugu.piyadeler.savunma=insanImparatorlugu.piyadeler.savunma+((insanImparatorlugu.piyadeler.savunma*insanImparatorlugu.Samur.etki_deger)/100) ;
                printf ("Samur savunma boostlu piyade gucu :%f", insanImparatorlugu.piyadeler.savunma) ;
            }
            if (strcmp(insanImparatorlugu.Samur.etkiledigi_birim,"okcular")==0)
            {
                insanImparatorlugu.okcular.savunma=insanImparatorlugu.okcular.savunma+((insanImparatorlugu.okcular.savunma*insanImparatorlugu.Samur.etki_deger)/100) ;
                printf ("Samur savunma boostlu okcular gucu :%f", insanImparatorlugu.okcular.savunma) ;
            }
            if (strcmp(insanImparatorlugu.Samur.etkiledigi_birim,"suvariler")==0)
            {
                insanImparatorlugu.suvariler.savunma=insanImparatorlugu.suvariler.savunma+((insanImparatorlugu.suvariler.savunma*insanImparatorlugu.Samur.etki_deger)/100) ;
                printf ("Samur savunma boostlu suvariler gucu :%f", insanImparatorlugu.suvariler.savunma) ;
            }
            if (strcmp(insanImparatorlugu.Samur.etkiledigi_birim,"kusatma_makineleri")==0)
            {
                insanImparatorlugu.kusatma_makineleri.savunma=insanImparatorlugu.kusatma_makineleri.savunma+((insanImparatorlugu.kusatma_makineleri.savunma*insanImparatorlugu.Samur.etki_deger)/100) ;
                printf ("Samur savunma boostlu kusatma makine gucu :%f", insanImparatorlugu.kusatma_makineleri.savunma) ;
            }
        }
        if (strcmp(insanImparatorlugu.Samur.etki_turu,"kritik_sans")==0)
        {
            if (strcmp(insanImparatorlugu.Samur.etkiledigi_birim,"piyade"))
            {
                insanImparatorlugu.piyadeler.kritik_sans=insanImparatorlugu.piyadeler.kritik_sans+((insanImparatorlugu.piyadeler.kritik_sans*insanImparatorlugu.Samur.etki_deger)/100) ;
                printf ("Samur kritik sans boostlu piyade gucu :%f", insanImparatorlugu.piyadeler.kritik_sans) ;
            }
            if (strcmp(insanImparatorlugu.Samur.etkiledigi_birim,"okcular")==0)
            {
                insanImparatorlugu.okcular.kritik_sans=insanImparatorlugu.okcular.kritik_sans+((insanImparatorlugu.okcular.kritik_sans*insanImparatorlugu.Samur.etki_deger)/100) ;
                printf ("Samur kritik sans boostlu okcular gucu :%f", insanImparatorlugu.okcular.kritik_sans) ;
            }

            if (strcmp(insanImparatorlugu.Samur.etkiledigi_birim,"suvariler")==0)
            {
                insanImparatorlugu.suvariler.kritik_sans=insanImparatorlugu.suvariler.kritik_sans+((insanImparatorlugu.suvariler.kritik_sans*insanImparatorlugu.Samur.etki_deger)/100) ;
                printf ("Samur kritik sans boostlu suvariler gucu :%f", insanImparatorlugu.suvariler.kritik_sans) ;
            }
            if (strcmp(insanImparatorlugu.Samur.etkiledigi_birim,"kusatma_makineleri")==0)
            {
                insanImparatorlugu.kusatma_makineleri.kritik_sans=insanImparatorlugu.kusatma_makineleri.kritik_sans+((insanImparatorlugu.kusatma_makineleri.kritik_sans*insanImparatorlugu.Samur.etki_deger)/100) ;
                printf ("Samur kritik sans boostlu kusatma makine gucu :%f", insanImparatorlugu.kusatma_makineleri.kritik_sans) ;
            }
        }
    }
    if (saldiri_gelistirmesi_insan  !=0)
    {
        if (saldiri_gelistirmesi_insan==1)
        {
            if (strcmp(saldiri_gelistirmesi.seviye_1.etkiledigi_birim,"tum_birimler")==0)
            {
                insanImparatorlugu.piyadeler.saldiri=insanImparatorlugu.piyadeler.saldiri+((insanImparatorlugu.piyadeler.saldiri*saldiri_gelistirmesi.seviye_1.deger)/100);
                insanImparatorlugu.okcular.saldiri=insanImparatorlugu.okcular.saldiri+((insanImparatorlugu.okcular.saldiri*saldiri_gelistirmesi.seviye_1.deger)/100);
                insanImparatorlugu.suvariler.saldiri=insanImparatorlugu.suvariler.saldiri+((insanImparatorlugu.suvariler.saldiri*saldiri_gelistirmesi.seviye_1.deger)/100);
                insanImparatorlugu.kusatma_makineleri.saldiri=insanImparatorlugu.kusatma_makineleri.saldiri+((insanImparatorlugu.kusatma_makineleri.saldiri*saldiri_gelistirmesi.seviye_1.deger)/100);
                printf ("\nTum birimlerin saldiri gucu saldiri gelistirmesi seviye 1 ile yuzde %f yukseltildi\n",saldiri_gelistirmesi.seviye_1.deger);
            }
        }
        if (saldiri_gelistirmesi_insan==2)
        {
            if (strcmp(saldiri_gelistirmesi.seviye_1.etkiledigi_birim,"tum_birimler")==0)
            {
                insanImparatorlugu.piyadeler.saldiri=insanImparatorlugu.piyadeler.saldiri+((insanImparatorlugu.piyadeler.saldiri*saldiri_gelistirmesi.seviye_2.deger)/100);
                insanImparatorlugu.okcular.saldiri=insanImparatorlugu.okcular.saldiri+((insanImparatorlugu.okcular.saldiri*saldiri_gelistirmesi.seviye_2.deger)/100);
                insanImparatorlugu.suvariler.saldiri=insanImparatorlugu.suvariler.saldiri+((insanImparatorlugu.suvariler.saldiri*saldiri_gelistirmesi.seviye_2.deger)/100);
                insanImparatorlugu.kusatma_makineleri.saldiri=insanImparatorlugu.kusatma_makineleri.saldiri+((insanImparatorlugu.kusatma_makineleri.saldiri*saldiri_gelistirmesi.seviye_2.deger)/100);
                printf ("\nTum birimlerin saldiri gucu saldiri gelistirmesi seviye 2 ile yuzde %f yukseltildi\n",saldiri_gelistirmesi.seviye_2.deger);
            }
        }
        if (saldiri_gelistirmesi_insan==3)
        {
            if (strcmp(saldiri_gelistirmesi.seviye_1.etkiledigi_birim,"tum_birimler")==0)
            {
                insanImparatorlugu.piyadeler.saldiri=insanImparatorlugu.piyadeler.saldiri+((insanImparatorlugu.piyadeler.saldiri*saldiri_gelistirmesi.seviye_3.deger)/100);
                insanImparatorlugu.okcular.saldiri=insanImparatorlugu.okcular.saldiri+((insanImparatorlugu.okcular.saldiri*saldiri_gelistirmesi.seviye_3.deger)/100);
                insanImparatorlugu.suvariler.saldiri=insanImparatorlugu.suvariler.saldiri+((insanImparatorlugu.suvariler.saldiri*saldiri_gelistirmesi.seviye_3.deger)/100);
                insanImparatorlugu.kusatma_makineleri.saldiri=insanImparatorlugu.kusatma_makineleri.saldiri+((insanImparatorlugu.kusatma_makineleri.saldiri*saldiri_gelistirmesi.seviye_3.deger)/100);
                printf ("\nTum birimlerin saldiri gucu saldiri gelistirmesi seviye 3 ile yuzde %f yukseltildi\n",saldiri_gelistirmesi.seviye_3.deger);
            }
        }
    }
    if (savunma_ustaligi_insan!=0)
    {
        if (savunma_ustaligi_insan==1)
        {
            insanImparatorlugu.piyadeler.savunma=insanImparatorlugu.piyadeler.savunma+((insanImparatorlugu.piyadeler.savunma*savunma_ustaligi.seviye_1.deger)/100);
            insanImparatorlugu.okcular.savunma=insanImparatorlugu.okcular.savunma+((insanImparatorlugu.okcular.savunma*savunma_ustaligi.seviye_1.deger)/100);
            insanImparatorlugu.suvariler.savunma=insanImparatorlugu.suvariler.savunma+((insanImparatorlugu.suvariler.savunma*savunma_ustaligi.seviye_1.deger)/100);
            insanImparatorlugu.kusatma_makineleri.savunma=insanImparatorlugu.kusatma_makineleri.savunma+((insanImparatorlugu.kusatma_makineleri.savunma*savunma_ustaligi.seviye_1.deger)/100);
            printf ("\nTum birimlerin savunma gucu savunma ustaligi seviye 1 ile yuzde %f yukseltildi\n",savunma_ustaligi.seviye_1.deger);
        }
        if (savunma_ustaligi_insan==2)
        {
            insanImparatorlugu.piyadeler.savunma=insanImparatorlugu.piyadeler.savunma+((insanImparatorlugu.piyadeler.savunma*savunma_ustaligi.seviye_2.deger)/100);
            insanImparatorlugu.okcular.savunma=insanImparatorlugu.okcular.savunma+((insanImparatorlugu.okcular.savunma*savunma_ustaligi.seviye_2.deger)/100);
            insanImparatorlugu.suvariler.savunma=insanImparatorlugu.suvariler.savunma+((insanImparatorlugu.suvariler.savunma*savunma_ustaligi.seviye_2.deger)/100);
            insanImparatorlugu.kusatma_makineleri.savunma=insanImparatorlugu.kusatma_makineleri.savunma+((insanImparatorlugu.kusatma_makineleri.savunma*savunma_ustaligi.seviye_2.deger)/100);
            printf ("\nTum birimlerin savunma gucu savunma ustaligi seviye 2 ile yuzde %f yukseltildi\n",savunma_ustaligi.seviye_2.deger);
        }
        if (savunma_ustaligi_insan==3)
        {
            insanImparatorlugu.piyadeler.savunma=insanImparatorlugu.piyadeler.savunma+((insanImparatorlugu.piyadeler.savunma*savunma_ustaligi.seviye_3.deger)/100);
            insanImparatorlugu.okcular.savunma=insanImparatorlugu.okcular.savunma+((insanImparatorlugu.okcular.savunma*savunma_ustaligi.seviye_3.deger)/100);
            insanImparatorlugu.suvariler.savunma=insanImparatorlugu.suvariler.savunma+((insanImparatorlugu.suvariler.savunma*savunma_ustaligi.seviye_3.deger)/100);
            insanImparatorlugu.kusatma_makineleri.savunma=insanImparatorlugu.kusatma_makineleri.savunma+((insanImparatorlugu.kusatma_makineleri.savunma*savunma_ustaligi.seviye_3.deger)/100);
            printf ("\nTum birimlerin savunma gucu savunma ustaligi seviye 3 ile yuzde %f yukseltildi\n",savunma_ustaligi.seviye_3.deger);
        }
    }
    if(elit_egitim_insan!=0)
    {
        if (elit_egitim_insan==1)
        {
            insanImparatorlugu.piyadeler.kritik_sans=insanImparatorlugu.piyadeler.kritik_sans+((insanImparatorlugu.piyadeler.kritik_sans*elit_egitim.seviye_1.deger)/100);
            insanImparatorlugu.okcular.kritik_sans=insanImparatorlugu.okcular.kritik_sans+((insanImparatorlugu.okcular.kritik_sans*elit_egitim.seviye_1.deger)/100);
            insanImparatorlugu.suvariler.kritik_sans=insanImparatorlugu.suvariler.kritik_sans+((insanImparatorlugu.suvariler.kritik_sans*elit_egitim.seviye_1.deger)/100);
            insanImparatorlugu.kusatma_makineleri.kritik_sans=insanImparatorlugu.kusatma_makineleri.kritik_sans+((insanImparatorlugu.kusatma_makineleri.kritik_sans*elit_egitim.seviye_1.deger)/100);
            printf ("\nTum birimlerin kritik_sans gucu elit egitim seviye 1 ile yuzde %f yukseltildi\n",elit_egitim.seviye_1.deger);
        }
        if (elit_egitim_insan==2)
        {
            insanImparatorlugu.piyadeler.kritik_sans=insanImparatorlugu.piyadeler.kritik_sans+((insanImparatorlugu.piyadeler.kritik_sans*elit_egitim.seviye_2.deger)/100);
            insanImparatorlugu.okcular.kritik_sans=insanImparatorlugu.okcular.kritik_sans+((insanImparatorlugu.okcular.kritik_sans*elit_egitim.seviye_2.deger)/100);
            insanImparatorlugu.suvariler.kritik_sans=insanImparatorlugu.suvariler.kritik_sans+((insanImparatorlugu.suvariler.kritik_sans*elit_egitim.seviye_2.deger)/100);
            insanImparatorlugu.kusatma_makineleri.kritik_sans=insanImparatorlugu.kusatma_makineleri.kritik_sans+((insanImparatorlugu.kusatma_makineleri.kritik_sans*elit_egitim.seviye_2.deger)/100);
            printf ("\nTum birimlerin kritik_sans gucu elit egitim seviye_2  ile yuzde %f yukseltildi\n",elit_egitim.seviye_2.deger);
        }
        if (elit_egitim_insan==3)
        {
            insanImparatorlugu.piyadeler.kritik_sans=insanImparatorlugu.piyadeler.kritik_sans+((insanImparatorlugu.piyadeler.kritik_sans*elit_egitim.seviye_3.deger)/100);
            insanImparatorlugu.okcular.kritik_sans=insanImparatorlugu.okcular.kritik_sans+((insanImparatorlugu.okcular.kritik_sans*elit_egitim.seviye_3.deger)/100);
            insanImparatorlugu.suvariler.kritik_sans=insanImparatorlugu.suvariler.kritik_sans+((insanImparatorlugu.suvariler.kritik_sans*elit_egitim.seviye_3.deger)/100);
            insanImparatorlugu.kusatma_makineleri.kritik_sans=insanImparatorlugu.kusatma_makineleri.kritik_sans+((insanImparatorlugu.kusatma_makineleri.kritik_sans*elit_egitim.seviye_3.deger)/100);
            printf ("\nTum birimlerin kritik_sans gucu elit egitim seviye_3 ile yuzde %f yukseltildi\n",elit_egitim.seviye_3.deger);
        }
    }
    if (kusatma_ustaligi_insan!=0)
    {
        if (kusatma_ustaligi_insan==1)
        {
            if (strcmp(kusatma_ustaligi.seviye_1.etkiledigi_birim,"kusatma_makineleri")==0)
            {
                insanImparatorlugu.kusatma_makineleri.saldiri=insanImparatorlugu.kusatma_makineleri.saldiri+((insanImparatorlugu.kusatma_makineleri.saldiri*kusatma_ustaligi.seviye_1.deger)/100);
                printf ("\nKusatma Makinelerinin saldiri gucu kusatma ustaligi seviye_1 ile yuzde %f yukseltildi \n", kusatma_ustaligi.seviye_1.deger);
            }
        }
        if (kusatma_ustaligi_insan==2)
        {
            if (strcmp(kusatma_ustaligi.seviye_2.etkiledigi_birim,"kusatma_makineleri")==0)
            {
                insanImparatorlugu.kusatma_makineleri.saldiri=insanImparatorlugu.kusatma_makineleri.saldiri+((insanImparatorlugu.kusatma_makineleri.saldiri*kusatma_ustaligi.seviye_2.deger)/100);
                printf ("\nKusatma Makinelerinin saldiri gucu kusatma ustaligi seviye_2 ile yuzde %f yukseltildi \n", kusatma_ustaligi.seviye_2.deger);
            }
        }
        if (kusatma_ustaligi_insan==3)
        {
            if (strcmp(kusatma_ustaligi.seviye_3.etkiledigi_birim,"kusatma_makineleri")==0)
            {
                insanImparatorlugu.kusatma_makineleri.saldiri=insanImparatorlugu.kusatma_makineleri.saldiri+((insanImparatorlugu.kusatma_makineleri.saldiri*kusatma_ustaligi.seviye_3.deger)/100);
                printf ("\nKusatma Makinelerinin saldiri gucu kusatma ustaligi seviye_3 ile yuzde %f yukseltildi \n", kusatma_ustaligi.seviye_3.deger);
            }
        }
    }
    if (saldiri_gelistirmesi_ork  !=0)
    {
        if (saldiri_gelistirmesi_ork==1)
        {
            if (strcmp(saldiri_gelistirmesi.seviye_1.etkiledigi_birim,"tum_birimler")==0)
            {
                orkLegi.ork_dovusculeri.saldiri=orkLegi.ork_dovusculeri.saldiri+((orkLegi.ork_dovusculeri.saldiri*saldiri_gelistirmesi.seviye_1.deger)/100);
                orkLegi.mizrakcilar.saldiri=orkLegi.mizrakcilar.saldiri+((orkLegi.mizrakcilar.saldiri*saldiri_gelistirmesi.seviye_1.deger)/100);
                orkLegi.troller.saldiri=orkLegi.troller.saldiri+((orkLegi.troller.saldiri*saldiri_gelistirmesi.seviye_1.deger)/100);
                orkLegi.varg_binicileri.saldiri=orkLegi.varg_binicileri.saldiri+((orkLegi.varg_binicileri.saldiri*saldiri_gelistirmesi.seviye_1.deger)/100);
                printf ("\nTum birimlerin saldiri gucu saldiri gelistirmesi seviye 1 ile yuzde %f yukseltildi\n",saldiri_gelistirmesi.seviye_1.deger);
            }
        }
        if (saldiri_gelistirmesi_ork==2)
        {
            if (strcmp(saldiri_gelistirmesi.seviye_1.etkiledigi_birim,"tum_birimler")==0)
            {
                orkLegi.ork_dovusculeri.saldiri=orkLegi.ork_dovusculeri.saldiri+((orkLegi.ork_dovusculeri.saldiri*saldiri_gelistirmesi.seviye_2.deger)/100);
                orkLegi.mizrakcilar.saldiri=orkLegi.mizrakcilar.saldiri+((orkLegi.mizrakcilar.saldiri*saldiri_gelistirmesi.seviye_2.deger)/100);
                orkLegi.varg_binicileri.saldiri=orkLegi.varg_binicileri.saldiri+((orkLegi.varg_binicileri.saldiri*saldiri_gelistirmesi.seviye_2.deger)/100);
                orkLegi.troller.saldiri=orkLegi.troller.saldiri+((orkLegi.troller.saldiri*saldiri_gelistirmesi.seviye_2.deger)/100);
                printf ("\nTum birimlerin saldiri gucu saldiri gelistirmesi seviye 2 ile yuzde %f yukseltildi\n",saldiri_gelistirmesi.seviye_2.deger);
            }
        }
        if (saldiri_gelistirmesi_ork==3)
        {
            if (strcmp(saldiri_gelistirmesi.seviye_1.etkiledigi_birim,"tum_birimler")==0)
            {
                orkLegi.ork_dovusculeri.saldiri=orkLegi.ork_dovusculeri.saldiri+((orkLegi.ork_dovusculeri.saldiri*saldiri_gelistirmesi.seviye_3.deger)/100);
                orkLegi.mizrakcilar.saldiri=orkLegi.mizrakcilar.saldiri+((orkLegi.mizrakcilar.saldiri*saldiri_gelistirmesi.seviye_3.deger)/100);
                orkLegi.varg_binicileri.saldiri=orkLegi.varg_binicileri.saldiri+((orkLegi.varg_binicileri.saldiri*saldiri_gelistirmesi.seviye_3.deger)/100);
                orkLegi.troller.saldiri=orkLegi.troller.saldiri+((orkLegi.troller.saldiri*saldiri_gelistirmesi.seviye_3.deger)/100);
                printf ("\nTum birimlerin saldiri gucu saldiri gelistirmesi seviye 3 ile yuzde %f yukseltildi\n",saldiri_gelistirmesi.seviye_3.deger);
            }
        }
    }
    if (savunma_ustaligi_ork!=0)
    {
        if (savunma_ustaligi_ork==1)
        {
            orkLegi.ork_dovusculeri.savunma=orkLegi.ork_dovusculeri.savunma+((orkLegi.ork_dovusculeri.savunma*savunma_ustaligi.seviye_1.deger)/100);
            orkLegi.mizrakcilar.savunma=orkLegi.mizrakcilar.savunma+((orkLegi.mizrakcilar.savunma*savunma_ustaligi.seviye_1.deger)/100);
            orkLegi.varg_binicileri.savunma=orkLegi.varg_binicileri.savunma+((orkLegi.varg_binicileri.savunma*savunma_ustaligi.seviye_1.deger)/100);
            orkLegi.troller.savunma=orkLegi.troller.savunma+((orkLegi.troller.savunma*savunma_ustaligi.seviye_1.deger)/100);
            printf ("\nTum birimlerin savunma gucu savunma ustaligi seviye 1 ile yuzde %f yukseltildi\n",savunma_ustaligi.seviye_1.deger);
        }
        if (savunma_ustaligi_ork==2)
        {
            orkLegi.ork_dovusculeri.savunma=orkLegi.ork_dovusculeri.savunma+((orkLegi.ork_dovusculeri.savunma*savunma_ustaligi.seviye_2.deger)/100);
            orkLegi.mizrakcilar.savunma=orkLegi.mizrakcilar.savunma+((orkLegi.mizrakcilar.savunma*savunma_ustaligi.seviye_2.deger)/100);
            orkLegi.varg_binicileri.savunma=orkLegi.varg_binicileri.savunma+((orkLegi.varg_binicileri.savunma*savunma_ustaligi.seviye_2.deger)/100);
            orkLegi.troller.savunma=orkLegi.troller.savunma+((orkLegi.troller.savunma*savunma_ustaligi.seviye_2.deger)/100);
            printf ("\nTum birimlerin savunma gucu savunma ustaligi seviye 2 ile yuzde %f yukseltildi\n",savunma_ustaligi.seviye_2.deger);
        }
        if (savunma_ustaligi_ork==3)
        {
            orkLegi.ork_dovusculeri.savunma=orkLegi.ork_dovusculeri.savunma+((orkLegi.ork_dovusculeri.savunma*savunma_ustaligi.seviye_3.deger)/100);
            orkLegi.mizrakcilar.savunma=orkLegi.mizrakcilar.savunma+((orkLegi.mizrakcilar.savunma*savunma_ustaligi.seviye_3.deger)/100);
            orkLegi.varg_binicileri.savunma=orkLegi.varg_binicileri.savunma+((orkLegi.varg_binicileri.savunma*savunma_ustaligi.seviye_3.deger)/100);
            orkLegi.troller.savunma=orkLegi.troller.savunma+((orkLegi.troller.savunma*savunma_ustaligi.seviye_3.deger)/100);
            printf ("\nTum birimlerin savunma gucu savunma ustaligi seviye 3 ile yuzde %f yukseltildi\n",savunma_ustaligi.seviye_3.deger);
        }
    }
    if(elit_egitim_ork!=0)
    {
        if (elit_egitim_ork==1)
        {
            orkLegi.ork_dovusculeri.kritik_sans=orkLegi.ork_dovusculeri.kritik_sans+((orkLegi.ork_dovusculeri.kritik_sans*elit_egitim.seviye_1.deger)/100);
            orkLegi.mizrakcilar.kritik_sans=orkLegi.mizrakcilar.kritik_sans+((orkLegi.mizrakcilar.kritik_sans*elit_egitim.seviye_1.deger)/100);
            orkLegi.varg_binicileri.kritik_sans=orkLegi.varg_binicileri.kritik_sans+((orkLegi.varg_binicileri.kritik_sans*elit_egitim.seviye_1.deger)/100);
            orkLegi.troller.kritik_sans=orkLegi.troller.kritik_sans+((orkLegi.troller.kritik_sans*elit_egitim.seviye_1.deger)/100);
            printf ("\nTum birimlerin kritik_sans gucu elit egitim seviye 1 ile yuzde %f yukseltildi\n",elit_egitim.seviye_1.deger);
        }
        if (elit_egitim_ork==2)
        {
            orkLegi.ork_dovusculeri.kritik_sans=orkLegi.ork_dovusculeri.kritik_sans+((orkLegi.ork_dovusculeri.kritik_sans*elit_egitim.seviye_2.deger)/100);
            orkLegi.mizrakcilar.kritik_sans=orkLegi.mizrakcilar.kritik_sans+((orkLegi.mizrakcilar.kritik_sans*elit_egitim.seviye_2.deger)/100);
            orkLegi.varg_binicileri.kritik_sans=orkLegi.varg_binicileri.kritik_sans+((orkLegi.varg_binicileri.kritik_sans*elit_egitim.seviye_2.deger)/100);
            orkLegi.troller.kritik_sans=orkLegi.troller.kritik_sans+((orkLegi.troller.kritik_sans*elit_egitim.seviye_2.deger)/100);
            printf ("\nTum birimlerin kritik_sans gucu elit egitim seviye_2  ile yuzde %f yukseltildi\n",elit_egitim.seviye_2.deger);
        }
        if (elit_egitim_ork==3)
        {
            orkLegi.ork_dovusculeri.kritik_sans=orkLegi.ork_dovusculeri.kritik_sans+((orkLegi.ork_dovusculeri.kritik_sans*elit_egitim.seviye_3.deger)/100);
            orkLegi.mizrakcilar.kritik_sans=orkLegi.mizrakcilar.kritik_sans+((orkLegi.mizrakcilar.kritik_sans*elit_egitim.seviye_3.deger)/100);
            orkLegi.varg_binicileri.kritik_sans=orkLegi.varg_binicileri.kritik_sans+((orkLegi.varg_binicileri.kritik_sans*elit_egitim.seviye_3.deger)/100);
            orkLegi.troller.kritik_sans=orkLegi.troller.kritik_sans+((orkLegi.troller.kritik_sans*elit_egitim.seviye_3.deger)/100);
            printf ("\nTum birimlerin kritik_sans gucu elit egitim seviye_3 ile yuzde %f yukseltildi\n",elit_egitim.seviye_3.deger);
        }
    }
    FILE *Simulator =fopen("savas_sim.txt", "w" );
    if (Simulator ==NULL)
    {
        printf ("savas_sim.txt Acilamadi !!! ")  ;
        return 1 ;
    }
    fprintf (Simulator, "Insan Imparatorlugu Senaryo Baslangic Bilgileri \n") ;
    fprintf (Simulator, "Piyadeler : %.1f Birim (Saldiri Gucu :%f , Savunma Gucu : %f, Saglik :%f Kritik Vurus Sansi :%f\n", insanImparatorlugu.piyadeler.birlik_sayisi, insanImparatorlugu.piyadeler.saldiri,insanImparatorlugu.piyadeler.savunma,insanImparatorlugu.piyadeler.saglik,insanImparatorlugu.piyadeler.kritik_sans) ;
    fprintf(Simulator, "Okçular: %.1f Birim (Saldiri Gucu: %f, Savunma Gucu: %f, Saglik: %f, Kritik Vurus Sansi: %f)\n", insanImparatorlugu.okcular.birlik_sayisi, insanImparatorlugu.okcular.saldiri, insanImparatorlugu.okcular.savunma, insanImparatorlugu.okcular.saglik, insanImparatorlugu.okcular.kritik_sans);
    fprintf(Simulator, "Suvariler: %.1f Birim (Saldiri Gucu: %f, Savunma Gucu: %f, Saglik: %f, Kritik Vurus Sansi: %f)\n", insanImparatorlugu.suvariler.birlik_sayisi, insanImparatorlugu.suvariler.saldiri, insanImparatorlugu.suvariler.savunma, insanImparatorlugu.suvariler.saglik, insanImparatorlugu.suvariler.kritik_sans);
    fprintf(Simulator, "Kusatma Makineleri: %.1f Birim (Saldiri Gucu: %f, Savunma Gucu: %f, Saglik: %f, Kritik Vurus Sansi: %f)\n", insanImparatorlugu.kusatma_makineleri.birlik_sayisi, insanImparatorlugu.kusatma_makineleri.saldiri, insanImparatorlugu.kusatma_makineleri.savunma, insanImparatorlugu.kusatma_makineleri.saglik, insanImparatorlugu.kusatma_makineleri.kritik_sans);
    if(strcmp (kahramanismiinsan, "Alparslan")==0)
    {
        fprintf(Simulator, "Kahraman: %s %s %s gucunu %f artirir.\n", kahramanismiinsan, insanImparatorlugu.Alparslan.etkiledigi_birim, insanImparatorlugu.Alparslan.bonus_turu, insanImparatorlugu.Alparslan.bonus_degeri);
    }
    if(strcmp (kahramanismiinsan, "Fatih_Sultan_Mehmet")==0)
    {
        fprintf (Simulator, "Kahraman : %s  %s %s gucunu %f artirir.\n", kahramanismiinsan, insanImparatorlugu.Fatih_Sultan_Mehmet.etkiledigi_birim, insanImparatorlugu.Fatih_Sultan_Mehmet.bonus_turu, insanImparatorlugu.Fatih_Sultan_Mehmet.bonus_degeri);
    }
    if(strcmp (kahramanismiinsan, "Mete_Han")==0)
    {
        fprintf(Simulator, "Kahraman: %s %s %s gucunu %f artirir.\n", kahramanismiinsan, insanImparatorlugu.Mete_Han.etkiledigi_birim, insanImparatorlugu.Mete_Han.bonus_turu, insanImparatorlugu.Mete_Han.bonus_degeri);
    }
    if(strcmp (kahramanismiinsan, "Yavuz_Sultan_Selim")==0)
    {
        fprintf(Simulator, "Kahraman: %s %s %s gucunu %f artirir.\n", kahramanismiinsan, insanImparatorlugu.Yavuz_Sultan_Selim.etkiledigi_birim, insanImparatorlugu.Yavuz_Sultan_Selim.bonus_turu, insanImparatorlugu.Yavuz_Sultan_Selim.bonus_degeri);
    }
    if(strcmp (kahramanismiinsan, "Tugrul_Bey")==0)
    {
        fprintf(Simulator, "Kahraman: %s %s %s gucunu %f artirir.\n", kahramanismiinsan, insanImparatorlugu.Tugrul_Bey.etkiledigi_birim, insanImparatorlugu.Tugrul_Bey.bonus_turu, insanImparatorlugu.Tugrul_Bey.bonus_degeri);
    }
    if (strcmp(CanavarismiInsan, "Ejderha")==0)
    {
        fprintf (Simulator,"Canavar : %s (%s %s gucunu %f arttirir)", CanavarismiInsan, insanImparatorlugu.Ejderha.etkiledigi_birim, insanImparatorlugu.Ejderha.etki_turu,insanImparatorlugu.Ejderha.etki_deger );
    }
    if (strcmp(CanavarismiInsan, "Ejderha")==0)
    {
        fprintf (Simulator,"Canavar : %s (%s %s gucunu %f arttirir)", CanavarismiInsan, insanImparatorlugu.Ejderha.etkiledigi_birim, insanImparatorlugu.Ejderha.etki_turu,insanImparatorlugu.Ejderha.etki_deger );
    }
    if (strcmp(CanavarismiInsan, "Agri_Dagi_Devleri") == 0)
    {
        fprintf(Simulator, "Canavar: %s (%s %s gucunu %f arttirir)\n", CanavarismiInsan, insanImparatorlugu.Agri_Dagi_Devleri.etkiledigi_birim, insanImparatorlugu.Agri_Dagi_Devleri.etki_turu, insanImparatorlugu.Agri_Dagi_Devleri.etki_deger);
    }
    if (strcmp(CanavarismiInsan, "Tepegoz") == 0)
    {
        fprintf(Simulator, "Canavar: %s (%s %s gucunu %f arttirir)\n", CanavarismiInsan, insanImparatorlugu.Tepegoz.etkiledigi_birim, insanImparatorlugu.Tepegoz.etki_turu, insanImparatorlugu.Tepegoz.etki_deger);
    }
    if (strcmp(CanavarismiInsan, "Karakurt") == 0)
    {
        fprintf(Simulator, "Canavar: %s (%s %s gucunu %f arttirir)\n", CanavarismiInsan, insanImparatorlugu.Karakurt.etkiledigi_birim, insanImparatorlugu.Karakurt.etki_turu, insanImparatorlugu.Karakurt.etki_deger);
    }
    if (strcmp(CanavarismiInsan, "Samur") == 0)
    {
        fprintf(Simulator, "Canavar: %s (%s %s gucunu %f arttirir)\n", CanavarismiInsan, insanImparatorlugu.Samur.etkiledigi_birim, insanImparatorlugu.Samur.etki_turu, insanImparatorlugu.Samur.etki_deger);
    }
    fprintf (Simulator, "\n\n\nOrk Legi Senaryo Baslangic Bilgileri \n\n\n") ;

    fprintf(Simulator, "Ork Dovusculeri: %.1f Birim (Saldiri Gucu: %f, Savunma Gucu: %f, Saglik: %f, Kritik Vurus Sansi: %f)\n", orkLegi.ork_dovusculeri.birlik_sayisi, orkLegi.ork_dovusculeri.saldiri, orkLegi.ork_dovusculeri.savunma, orkLegi.ork_dovusculeri.saglik, orkLegi.ork_dovusculeri.kritik_sans);
    fprintf(Simulator, "Mizrakcilar: %.1f Birim (Saldiri Gucu: %f, Savunma Gucu: %f, Saglik: %f, Kritik Vurus Sansi: %f)\n", orkLegi.mizrakcilar.birlik_sayisi, orkLegi.mizrakcilar.saldiri, orkLegi.mizrakcilar.savunma, orkLegi.mizrakcilar.saglik, orkLegi.mizrakcilar.kritik_sans);
    fprintf(Simulator, "Varg Binicileri: %.1f Birim (Saldiri Gucu: %f, Savunma Gucu: %f, Saglik: %f, Kritik Vurus Sansi: %f)\n", orkLegi.varg_binicileri.birlik_sayisi, orkLegi.varg_binicileri.saldiri, orkLegi.varg_binicileri.savunma, orkLegi.varg_binicileri.saglik, orkLegi.varg_binicileri.kritik_sans);
    fprintf(Simulator, "Troller: %.1f Birim (Saldiri Gucu: %f, Savunma Gucu: %f, Saglik: %f, Kritik Vurus Sansi: %f)\n", orkLegi.troller.birlik_sayisi, orkLegi.troller.saldiri, orkLegi.troller.savunma, orkLegi.troller.saglik, orkLegi.troller.kritik_sans);
    if(strcmp (kahramanismiinsan, "Goruk_Vahsi")==0)
    {
        fprintf(Simulator, "Kahraman: %s %s %s gucunu %f artirir.\n", kahramanismiOrk, orkLegi.Goruk_Vahsi.etkiledigi_birim, orkLegi.Goruk_Vahsi.bonus_turu, orkLegi.Goruk_Vahsi.bonus_degeri);
    }
    if (strcmp(kahramanismiinsan, "Thruk_Kemikkiran") == 0)
    {
        fprintf(Simulator, "Kahraman: %s %s %s gucunu %f artirir.\n", kahramanismiOrk, orkLegi.Thruk_Kemikkiran.etkiledigi_birim, orkLegi.Thruk_Kemikkiran.bonus_turu, orkLegi.Thruk_Kemikkiran.bonus_degeri);
    }
    if (strcmp(kahramanismiinsan, "Vrog_Kafakiran") == 0)
    {
        fprintf(Simulator, "Kahraman: %s %s %s gucunu %f artirir.\n", kahramanismiOrk, orkLegi.Vrog_Kafakiran.etkiledigi_birim, orkLegi.Vrog_Kafakiran.bonus_turu, orkLegi.Vrog_Kafakiran.bonus_degeri);
    }
    if (strcmp(kahramanismiinsan, "Ugar_Zalim") == 0)
    {
        fprintf(Simulator, "Kahraman: %s %s %s gucunu %f artirir.\n", kahramanismiOrk, orkLegi.Ugar_Zalim.etkiledigi_birim, orkLegi.Ugar_Zalim.bonus_turu, orkLegi.Ugar_Zalim.bonus_degeri);
    }
    if (strcmp(CanavarismiOrk, "Kara_Troll") == 0)
    {
        fprintf(Simulator, "Canavar: %s (%s %s gucunu %f arttirir)", CanavarismiOrk, orkLegi.Kara_Troll.etkiledigi_birim, orkLegi.Kara_Troll.etki_turu, orkLegi.Kara_Troll.etki_deger);
    }
    if (strcmp(CanavarismiOrk, "Golge_Kurtlari") == 0)
    {
        fprintf(Simulator, "Canavar: %s (%s %s gucunu %f arttirir)", CanavarismiOrk, orkLegi.Golge_Kurtlari.etkiledigi_birim, orkLegi.Golge_Kurtlari.etki_turu, orkLegi.Golge_Kurtlari.etki_deger);
    }
    if (strcmp(CanavarismiOrk, "Camur_Devleri") == 0)
    {
        fprintf(Simulator, "Canavar: %s (%s %s gucunu %f arttirir)", CanavarismiOrk, orkLegi.Camur_Devleri.etkiledigi_birim, orkLegi.Camur_Devleri.etki_turu, orkLegi.Camur_Devleri.etki_deger);
    }
    if (strcmp(CanavarismiOrk, "Ates_Iblisi") == 0)
    {
        fprintf(Simulator, "Canavar: %s (%s %s gucunu %f arttirir)", CanavarismiOrk, orkLegi.Ates_Iblisi.etkiledigi_birim, orkLegi.Ates_Iblisi.etki_turu, orkLegi.Ates_Iblisi.etki_deger);
    }
    fprintf (Simulator, "Arastirmalar\nInsan Imparatorlugu\n") ;

    SDL_Window *pencere = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Surface *arkaPlanYuzeyi = NULL;
    SDL_Texture *arkaPlanTexture = NULL;
    SDL_Surface *okcularYuzeyi = NULL;
    SDL_Texture *okcularTexture = NULL;
    SDL_Surface *piyadelerYuzeyi = NULL;
    SDL_Texture *piyadelerTexture = NULL;
    SDL_Surface *suvarilerYuzeyi = NULL;
    SDL_Texture *suvarilerTexture = NULL;
    SDL_Surface *kusatmaMakinesiYuzeyi = NULL;
    SDL_Texture *kusatmaMakinesiTexture = NULL;
    SDL_Surface *orkDovuscuYuzeyi = NULL;
    SDL_Texture *orkDovuscuTexture = NULL;
    SDL_Surface *vargBiniciYuzeyi = NULL;
    SDL_Texture *vargBiniciTexture = NULL;
    SDL_Surface *mizrakciYuzeyi = NULL;
    SDL_Texture *mizrakciTexture = NULL;
    SDL_Surface *trollerYuzeyi = NULL;
    SDL_Texture *trollerTexture = NULL;
    SDL_Surface *ejderhaYuzeyi = NULL;
    SDL_Texture *ejderhaTexture = NULL;
    SDL_Surface *agriDagiDeviYuzeyi = NULL;
    SDL_Texture *agriDagiDeviTexture = NULL;
    SDL_Surface *tepegozYuzeyi = NULL;
    SDL_Texture *tepegozTexture = NULL;
    SDL_Surface *karakurtYuzeyi = NULL;
    SDL_Texture *karakurtTexture = NULL;
    SDL_Texture *samurTexture = NULL;
    SDL_Surface *samurYuzeyi = NULL;
    SDL_Surface *golgeKurtYuzeyi = NULL;
    SDL_Texture *golgeKurtTexture = NULL;
    SDL_Surface *buzDeviYuzeyi = NULL;
    SDL_Texture *buzDeviTexture = NULL;
    SDL_Surface *karaTrollYuzeyi = NULL;
    SDL_Texture *karaTrollTexture = NULL;
    SDL_Surface *camurDevYuzeyi = NULL;
    SDL_Texture *camurDevTexture = NULL;
    SDL_Surface *atesIblisiYuzeyi = NULL;
    SDL_Texture *atesIblisiTexture = NULL;
    SDL_Surface *makrogYuzeyi = NULL;
    SDL_Texture *makrogTexture = NULL;
    SDL_Surface *alparslanYuzeyi = NULL;
    SDL_Texture *alparslanTexture = NULL;
    SDL_Surface *yavuzSultanYuzeyi = NULL;
    SDL_Texture *yavuzSultanTexture = NULL;
    SDL_Surface *fatihSultanYuzeyi = NULL;
    SDL_Texture *fatihSultanTexture = NULL;
    SDL_Surface *meteHanYuzeyi = NULL;
    SDL_Texture *meteHanTexture = NULL;
    SDL_Surface *tugrulBeyYuzeyi = NULL;
    SDL_Texture *tugrulBeyTexture = NULL;
    SDL_Surface *gorukVahsiYuzeyi = NULL;
    SDL_Texture *gorukVahsiTexture = NULL;
    SDL_Surface *thrukYuzeyi = NULL;
    SDL_Texture *thrukTexture = NULL;
    SDL_Surface *vrogYuzeyi = NULL;
    SDL_Texture *vrogTexture = NULL;
    SDL_Surface *ugarYuzeyi = NULL;
    SDL_Texture *ugarTexture = NULL;
    int insanKazandi=0;
    int OrkKazandi=0 ;
    int saglikOkcular;
    int saglikPiyadeler;
    int saglikSuvariler;
    int saglikKusatma;
    int saglikOrk;
    int saglikMizrakcilar;
    int saglikTroller;
    int saglikVarg;
    int ekran_genislik, ekran_yukseklik;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("SDL baslatilamadi: %s", SDL_GetError());
        return 1;
    }
    pencere = SDL_CreateWindow("Kare Izgara", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (!pencere)
    {
        SDL_Log("Pencere olusturulamadi: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    renderer = SDL_CreateRenderer(pencere, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        SDL_Log("Renderer olusturulamadi: %s", SDL_GetError());
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    arkaPlanYuzeyi = SDL_LoadBMP("background.bmp");
    if (!arkaPlanYuzeyi)
    {
        SDL_Log("Resim yuklenemedi: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    arkaPlanTexture = SDL_CreateTextureFromSurface(renderer, arkaPlanYuzeyi);
    SDL_FreeSurface(arkaPlanYuzeyi);

    okcularYuzeyi = IMG_Load("okcular.png");
    if (!okcularYuzeyi)
    {
        SDL_Log("Okçular resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    okcularTexture = SDL_CreateTextureFromSurface(renderer, okcularYuzeyi);
    SDL_FreeSurface(okcularYuzeyi);
    SDL_GetWindowSize(pencere, &ekran_genislik, &ekran_yukseklik);

    int oyunAlaniGenislik = GRID_COLS * SQUARE_SIZE;
    int oyunAlaniYukseklik = GRID_ROWS * SQUARE_SIZE;

    int baslangicX = (ekran_genislik - oyunAlaniGenislik) / 2;
    int baslangicY = (ekran_yukseklik - oyunAlaniYukseklik) / 2;

    int maxOkcuPerKare = 100;
    int toplamOkcu = insanImparatorlugu.okcular.birlik_sayisi;
    int kalanOkcu = toplamOkcu;
    int mevcutKare = 0;

    int toplamKare = GRID_ROWS * GRID_COLS;

    int okcuInKare[toplamKare];
    memset(okcuInKare, 0, sizeof(okcuInKare));

    for (int i = 0; i < toplamOkcu && mevcutKare < toplamKare; i++)
    {
        okcuInKare[mevcutKare]++;

        if (okcuInKare[mevcutKare] >= maxOkcuPerKare)
        {
            mevcutKare++;
        }
    }
    piyadelerYuzeyi = IMG_Load("piyadeler.png");
    if (!piyadelerYuzeyi)
    {
        SDL_Log("Piyadeler resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    piyadelerTexture = SDL_CreateTextureFromSurface(renderer, piyadelerYuzeyi);
    SDL_FreeSurface(piyadelerYuzeyi);

    int maxPiyadePerKare = 100;
    int toplamPiyade = insanImparatorlugu.piyadeler.birlik_sayisi;
    int kalanPiyade = toplamPiyade;
    int mevcutKarePiyade = 0;

    int piyadeInKare[toplamKare];
    memset(piyadeInKare, 0, sizeof(piyadeInKare));

    for (int i = 0; i < toplamPiyade && mevcutKarePiyade < toplamKare; i++)
    {
        piyadeInKare[mevcutKarePiyade]++;
        if (piyadeInKare[mevcutKarePiyade] >= maxPiyadePerKare)
        {
            mevcutKarePiyade++;
        }
    }

    suvarilerYuzeyi = IMG_Load("suvariler.png");
    if (!suvarilerYuzeyi)
    {
        SDL_Log("Suvariler resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    suvarilerTexture = SDL_CreateTextureFromSurface(renderer, suvarilerYuzeyi);
    SDL_FreeSurface(suvarilerYuzeyi);

    int maxSuvariPerKare = 100;
    int toplamSuvari = insanImparatorlugu.suvariler.birlik_sayisi;
    int kalanSuvari = toplamSuvari;
    int mevcutKareSuvari = 0;

    int suvariInKare[toplamKare];
    memset(suvariInKare, 0, sizeof(suvariInKare));

    for (int i = 0; i < toplamSuvari && mevcutKareSuvari < toplamKare; i++)
    {
        suvariInKare[mevcutKareSuvari]++;
        if (suvariInKare[mevcutKareSuvari] >= maxSuvariPerKare)
        {
            mevcutKareSuvari++;
        }
    }

    kusatmaMakinesiYuzeyi = IMG_Load("kusatma_makineleri.png");
    if (!kusatmaMakinesiYuzeyi)
    {
        SDL_Log("Kusatma makineleri resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    kusatmaMakinesiTexture = SDL_CreateTextureFromSurface(renderer, kusatmaMakinesiYuzeyi);
    SDL_FreeSurface(kusatmaMakinesiYuzeyi);

    int maxKusatmaPerKare = 100;
    int toplamKusatma = insanImparatorlugu.kusatma_makineleri.birlik_sayisi;
    int kalanKusatma = toplamKusatma;
    int mevcutKareKusatma = 0;

    int kusatmaInKare[toplamKare];
    memset(kusatmaInKare, 0, sizeof(kusatmaInKare));

    for (int i = 0; i < toplamKusatma && mevcutKareKusatma < toplamKare; i++)
    {
        kusatmaInKare[mevcutKareKusatma]++;
        if (kusatmaInKare[mevcutKareKusatma] >= maxKusatmaPerKare)
        {
            mevcutKareKusatma++;
        }
    }
    orkDovuscuYuzeyi = IMG_Load("ork_dovusculeri.png");
    if (!orkDovuscuYuzeyi)
    {
        SDL_Log("ork_dovusculeri resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    orkDovuscuTexture = SDL_CreateTextureFromSurface(renderer, orkDovuscuYuzeyi);
    SDL_FreeSurface(orkDovuscuYuzeyi);

    int maxOrkPerKare = 100;
    int toplamOrk = orkLegi.ork_dovusculeri.birlik_sayisi;
    int kalanOrk = toplamOrk;
    int mevcutKareOrk = 0;

    int orkInKare[toplamKare];
    memset(orkInKare, 0, sizeof(orkInKare));

    for (int i = 0; i < toplamOrk && mevcutKareOrk < toplamKare; i++)
    {
        orkInKare[mevcutKareOrk]++;
        if (orkInKare[mevcutKareOrk] >= maxOrkPerKare)
        {
            mevcutKareOrk++;
        }
    }

    vargBiniciYuzeyi = IMG_Load("varg_binicileri.png");
    if (!vargBiniciYuzeyi)
    {
        SDL_Log("varg_binicileri resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    vargBiniciTexture = SDL_CreateTextureFromSurface(renderer, vargBiniciYuzeyi);
    SDL_FreeSurface(vargBiniciYuzeyi);

    int maxVargPerKare = 100;
    int toplamVarg = orkLegi.varg_binicileri.birlik_sayisi;
    int kalanVarg = toplamVarg;
    int mevcutKareVarg = 0;

    int vargInKare[toplamKare];
    memset(vargInKare, 0, sizeof(vargInKare));

    for (int i = 0; i < toplamVarg && mevcutKareVarg < toplamKare; i++)
    {
        vargInKare[mevcutKareVarg]++;
        if (vargInKare[mevcutKareVarg] >= maxVargPerKare)
        {
            mevcutKareVarg++;
        }
    }

    mizrakciYuzeyi = IMG_Load("mizrakcilar.png");
    if (!mizrakciYuzeyi)
    {
        SDL_Log("mizrakcilar.png resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    mizrakciTexture = SDL_CreateTextureFromSurface(renderer, mizrakciYuzeyi);
    SDL_FreeSurface(mizrakciYuzeyi);
    int maxMizrakciPerKare = 100;
    int toplamMizrakci = orkLegi.mizrakcilar.birlik_sayisi;
    int kalanMizrakci = toplamMizrakci;
    int mevcutKareMizrakci = 0;

    int mizrakInKare[toplamKare];
    memset(mizrakInKare, 0, sizeof(mizrakInKare));

    for (int i = 0; i < toplamMizrakci && mevcutKareMizrakci < toplamKare; i++)
    {
        mizrakInKare[mevcutKareMizrakci]++;
        if (mizrakInKare[mevcutKareMizrakci] >= maxMizrakciPerKare)
        {
            mevcutKareMizrakci++;
        }
    }

    trollerYuzeyi = IMG_Load("troller.png");
    if (!trollerYuzeyi)
    {
        SDL_Log("Troller resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    trollerTexture = SDL_CreateTextureFromSurface(renderer, trollerYuzeyi);
    SDL_FreeSurface(trollerYuzeyi);

    int maxTrollPerKare = 100;
    int toplamTroll = orkLegi.troller.birlik_sayisi;
    int kalanTroll = toplamTroll;
    int mevcutKareTroll = 0;

    int trollInKare[toplamKare];
    memset(trollInKare, 0, sizeof(trollInKare));

    for (int i = 0; i < toplamTroll && mevcutKareTroll < toplamKare; i++)
    {
        trollInKare[mevcutKareTroll]++;
        if (trollInKare[mevcutKareTroll] >= maxTrollPerKare)
        {
            mevcutKareTroll++;
        }
    }
    ejderhaYuzeyi = IMG_Load("ejderha.png");
    if (!ejderhaYuzeyi)
    {
        SDL_Log("ejderha resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    ejderhaTexture = SDL_CreateTextureFromSurface(renderer, ejderhaYuzeyi);
    SDL_FreeSurface(ejderhaYuzeyi);

    tepegozYuzeyi = IMG_Load("tepegoz.png");
    if (!tepegozYuzeyi)
    {
        SDL_Log("Tepegoz resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    tepegozTexture = SDL_CreateTextureFromSurface(renderer, tepegozYuzeyi);
    SDL_FreeSurface(tepegozYuzeyi);

    agriDagiDeviYuzeyi = IMG_Load("agri_dagi_devleri.png");
    if (!agriDagiDeviYuzeyi)
    {
        SDL_Log("Agri dagi devi resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    agriDagiDeviTexture = SDL_CreateTextureFromSurface(renderer, agriDagiDeviYuzeyi);
    SDL_FreeSurface(agriDagiDeviYuzeyi);

    karakurtYuzeyi = IMG_Load("karakurt.png");
    if (!karakurtYuzeyi)
    {
        SDL_Log("Karakurt  resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    karakurtTexture = SDL_CreateTextureFromSurface(renderer, karakurtYuzeyi);
    SDL_FreeSurface(karakurtYuzeyi);

    samurYuzeyi = IMG_Load("samur.png");
    if (!samurYuzeyi)
    {
        SDL_Log("samur  resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    samurTexture = SDL_CreateTextureFromSurface(renderer, samurYuzeyi);
    SDL_FreeSurface(samurYuzeyi);

    golgeKurtYuzeyi = IMG_Load("golge_kurtlari.png");
    if (!golgeKurtYuzeyi)
    {
        SDL_Log("golge_kurtlari  resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    golgeKurtTexture = SDL_CreateTextureFromSurface(renderer, golgeKurtYuzeyi);
    SDL_FreeSurface(golgeKurtYuzeyi);

    buzDeviYuzeyi = IMG_Load("buz_devi.png");
    if (!buzDeviYuzeyi)
    {
        SDL_Log("BUz devi resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    buzDeviTexture = SDL_CreateTextureFromSurface(renderer, buzDeviYuzeyi);
    SDL_FreeSurface(buzDeviYuzeyi);

    karaTrollYuzeyi = IMG_Load("kara_troll.png");
    if (!karaTrollYuzeyi)
    {
        SDL_Log("kara_troll resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    karaTrollTexture = SDL_CreateTextureFromSurface(renderer, karaTrollYuzeyi);
    SDL_FreeSurface(karaTrollYuzeyi);

    camurDevYuzeyi = IMG_Load("camur_devi.png");
    if (!camurDevYuzeyi)
    {
        SDL_Log("camur_devi resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    camurDevTexture = SDL_CreateTextureFromSurface(renderer, camurDevYuzeyi);
    SDL_FreeSurface(camurDevYuzeyi);

    atesIblisiYuzeyi = IMG_Load("ates_iblisi.png");
    if (!atesIblisiYuzeyi)
    {
        SDL_Log("ates_iblisi resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    atesIblisiTexture= SDL_CreateTextureFromSurface(renderer, atesIblisiYuzeyi);
    SDL_FreeSurface(atesIblisiYuzeyi);

    makrogYuzeyi = IMG_Load("makrog_savas_beyi.png");
    if (!makrogYuzeyi)
    {
        SDL_Log("makrog_savas_beyi resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    makrogTexture = SDL_CreateTextureFromSurface(renderer, makrogYuzeyi);
    SDL_FreeSurface(makrogYuzeyi);
    gorukVahsiYuzeyi = IMG_Load("Goruk_Vahsi.png");
    if (!gorukVahsiYuzeyi)
    {
        SDL_Log("Goruk_Vahsi resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    gorukVahsiTexture = SDL_CreateTextureFromSurface(renderer, gorukVahsiYuzeyi);
    SDL_FreeSurface(gorukVahsiYuzeyi);

    thrukYuzeyi = IMG_Load("Thruk_Kemikkiran.png");
    if (!thrukYuzeyi)
    {
        SDL_Log("Thruh_Kemikkiran resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    thrukTexture = SDL_CreateTextureFromSurface(renderer, thrukYuzeyi);
    SDL_FreeSurface(thrukYuzeyi);

    vrogYuzeyi = IMG_Load("Vrog_Kafakiran.png");
    if (!vrogYuzeyi)
    {
        SDL_Log("Vrog_Kafakiran resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    vrogTexture = SDL_CreateTextureFromSurface(renderer, vrogYuzeyi);
    SDL_FreeSurface(vrogYuzeyi);
    ugarYuzeyi = IMG_Load("Ugar_Zalim.png");
    if (!ugarYuzeyi)
    {
        SDL_Log("Ugar_Zalim resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    ugarTexture = SDL_CreateTextureFromSurface(renderer, ugarYuzeyi);
    SDL_FreeSurface(ugarYuzeyi);

    alparslanYuzeyi = IMG_Load("Alparslan.png");
    if (!alparslanYuzeyi)
    {
        SDL_Log("Alparslan resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    alparslanTexture = SDL_CreateTextureFromSurface(renderer, alparslanYuzeyi);
    SDL_FreeSurface(alparslanYuzeyi);

    yavuzSultanYuzeyi= IMG_Load("Yavuz_Sultan_Selim.png");
    if (!yavuzSultanYuzeyi)
    {
        SDL_Log("Yavuz_Sultan_Selim resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    yavuzSultanTexture = SDL_CreateTextureFromSurface(renderer, yavuzSultanYuzeyi);
    SDL_FreeSurface(yavuzSultanYuzeyi);

    fatihSultanYuzeyi = IMG_Load("Fatih_Sultan_Mehmet.png");
    if (!fatihSultanYuzeyi)
    {
        SDL_Log("Fatih_Sultan_Mehmet resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    fatihSultanTexture= SDL_CreateTextureFromSurface(renderer, fatihSultanYuzeyi);
    SDL_FreeSurface(fatihSultanYuzeyi);

    meteHanYuzeyi = IMG_Load("Mete_Han.png");
    if (!meteHanYuzeyi)
    {
        SDL_Log("Mete_Han resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    meteHanTexture = SDL_CreateTextureFromSurface(renderer, meteHanYuzeyi);
    SDL_FreeSurface(meteHanYuzeyi);

    tugrulBeyYuzeyi = IMG_Load("Tugrul_Bey.png");
    if (!tugrulBeyYuzeyi)
    {
        SDL_Log("Tugrul_Bey resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    tugrulBeyTexture = SDL_CreateTextureFromSurface(renderer, tugrulBeyYuzeyi);
    SDL_FreeSurface(tugrulBeyYuzeyi);

    srand(SDL_GetTicks());

    SDL_GetWindowSize(pencere, &ekran_genislik, &ekran_yukseklik);
    int ejderhaX = baslangicX + (2 * SQUARE_SIZE);
    int ejderhaY = baslangicY + (8 * SQUARE_SIZE);
    SDL_Rect ejderhakonum = {ejderhaX, ejderhaY, SQUARE_SIZE, SQUARE_SIZE};

    int tepegozX = baslangicX + (2 * SQUARE_SIZE);
    int tepegozY = baslangicY + (8 * SQUARE_SIZE);
    SDL_Rect tepegozkonum = {tepegozX, tepegozY, SQUARE_SIZE, SQUARE_SIZE};

    int agriDagiX = baslangicX + (2 * SQUARE_SIZE);
    int agriDagiY = baslangicY + (8 * SQUARE_SIZE);
    SDL_Rect agriDagikonum = {agriDagiX, agriDagiY, SQUARE_SIZE, SQUARE_SIZE};

    int karakurtX = baslangicX + (2 * SQUARE_SIZE);
    int karakurtY = baslangicY + (8 * SQUARE_SIZE);
    SDL_Rect karakurtkonum = {karakurtX, karakurtY, SQUARE_SIZE, SQUARE_SIZE};

    int samurX = baslangicX + (2 * SQUARE_SIZE);
    int samurY = baslangicY + (8 * SQUARE_SIZE);
    SDL_Rect samurkonum = {samurX, samurY, SQUARE_SIZE, SQUARE_SIZE};

    int golgeKurtX = baslangicX + (2 * SQUARE_SIZE);
    int golgeKurtY = baslangicY + (11 * SQUARE_SIZE);
    SDL_Rect GolgeKurtKonum = {golgeKurtX, golgeKurtY, SQUARE_SIZE, SQUARE_SIZE};

    int buzDeviX = baslangicX + (2 * SQUARE_SIZE);
    int buzDeviY = baslangicY + (11 * SQUARE_SIZE);
    SDL_Rect buzDeviKonum = {buzDeviX, buzDeviY, SQUARE_SIZE, SQUARE_SIZE};

    int camurDevX = baslangicX + (2 * SQUARE_SIZE);
    int camurDevY = baslangicY + (11 * SQUARE_SIZE);
    SDL_Rect camurDevKonum = {camurDevX, camurDevY, SQUARE_SIZE, SQUARE_SIZE};

    int atesIblisX = baslangicX + (2 * SQUARE_SIZE);
    int atesIblisY = baslangicY + (11 * SQUARE_SIZE);
    SDL_Rect atesIblisKonum = {atesIblisX, atesIblisY, SQUARE_SIZE, SQUARE_SIZE};

    int makrogX = baslangicX + (2 * SQUARE_SIZE);
    int makrogY = baslangicY + (11 * SQUARE_SIZE);
    SDL_Rect makrogKonum = {makrogX, makrogY, SQUARE_SIZE, SQUARE_SIZE};

    int vargX = baslangicX + (2 * SQUARE_SIZE);
    int vargY = baslangicY + (17 * SQUARE_SIZE);
    SDL_Rect vargKonum = {vargX, vargY, SQUARE_SIZE, SQUARE_SIZE};

    int karaTrollX = baslangicX + (2 * SQUARE_SIZE);
    int karaTrollY = baslangicY + (11 * SQUARE_SIZE);
    SDL_Rect karaTrollKonum = {karaTrollX, karaTrollY, SQUARE_SIZE, SQUARE_SIZE};

    int alparslanX = baslangicX + (2 * SQUARE_SIZE);
    int alparslanY = baslangicY + (6 * SQUARE_SIZE);
    SDL_Rect alparslanKonum = {alparslanX, alparslanY, SQUARE_SIZE, SQUARE_SIZE};

    int yavuzSultanX = baslangicX + (2 * SQUARE_SIZE);
    int yavuzSultanY = baslangicY + (6 * SQUARE_SIZE);
    SDL_Rect yavuzSultanKonum = {yavuzSultanX, yavuzSultanY, SQUARE_SIZE, SQUARE_SIZE};

    int fatihSultanX = baslangicX + (2 * SQUARE_SIZE);
    int fatihSultanY = baslangicY + (6 * SQUARE_SIZE);
    SDL_Rect fatihSultanKonum = {fatihSultanX, fatihSultanY, SQUARE_SIZE, SQUARE_SIZE};

    int metehanX = baslangicX + (2 * SQUARE_SIZE);
    int metehanY = baslangicY + (6 * SQUARE_SIZE);
    SDL_Rect metehanKonum = {metehanX, metehanY, SQUARE_SIZE, SQUARE_SIZE};

    int tugrulBeyX = baslangicX + (2 * SQUARE_SIZE);
    int tugrulBeyY = baslangicY + (6 * SQUARE_SIZE);
    SDL_Rect tugrulBeyKonum = {tugrulBeyX, tugrulBeyY, SQUARE_SIZE, SQUARE_SIZE};

    int gorukX = baslangicX + (2 * SQUARE_SIZE);
    int gorukY = baslangicY + (13 * SQUARE_SIZE);
    SDL_Rect gorukKonum = {gorukX, gorukY, SQUARE_SIZE, SQUARE_SIZE};

    int thrukX = baslangicX + (2 * SQUARE_SIZE);
    int thrukY = baslangicY + (13 * SQUARE_SIZE);
    SDL_Rect thrukKonum = {thrukX, thrukY, SQUARE_SIZE, SQUARE_SIZE};

    int vrogX = baslangicX + (2 * SQUARE_SIZE);
    int vrogY = baslangicY + (13 * SQUARE_SIZE);
    SDL_Rect vrogKonum = {vrogX, vrogY, SQUARE_SIZE, SQUARE_SIZE};

    int ugarX = baslangicX + (2 * SQUARE_SIZE);
    int ugarY = baslangicY + (13 * SQUARE_SIZE);
    SDL_Rect ugarKonum = {ugarX, ugarY, SQUARE_SIZE, SQUARE_SIZE};

    if (TTF_Init() == -1)
    {
        SDL_Log("SDL_ttf baslatilamadi: %s", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 14); // Font dosyasının yolu
    if (!font)
    {
        SDL_Log("Font yuklenemedi: %s", TTF_GetError());
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    int running = 1;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, arkaPlanTexture, NULL, NULL);

        for (int row = 0; row < GRID_ROWS; row++)
        {
            for (int col = 0; col < GRID_COLS; col++)
            {
                int x = baslangicX + col * SQUARE_SIZE;
                int y = baslangicY + row * SQUARE_SIZE;
                SDL_Color renk;
                if (row < GRID_ROWS / 2)
                {
                    renk = (SDL_Color)
                    {
                        0, 100, 0, 255
                    };
                }
                else
                {
                    renk = (SDL_Color)
                    {
                        0, 0, 139, 255
                    };
                }
                KareleriDoldur(renderer, x, y, SQUARE_SIZE, renk);
                SDL_Color kenarrenk = {0, 0, 0, 255};
                KareCiz(renderer, x, y, SQUARE_SIZE, kenarrenk);
            }
        }

        SDL_Color textrenk = {255, 255, 255};
        mevcutKare = 0;
        for (int row = 0; row < GRID_ROWS; row++)
        {
            for (int col = 0; col < GRID_COLS; col++)
            {
                int x = baslangicX + col * SQUARE_SIZE + 210 ;
                int y = baslangicY + row * SQUARE_SIZE;

                if (okcuInKare[mevcutKare] > 0)
                {
                    SDL_Rect okcukonum = {x, y, SQUARE_SIZE, SQUARE_SIZE};

                    SDL_RenderCopy(renderer, okcularTexture, NULL, &okcukonum);
                    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 11);

                    SDL_Color textrenk = {255, 255, 255};
                    SDL_Surface* textyuzeyismi = TTF_RenderText_Solid(font, "Okcular", textrenk);
                    if (textyuzeyismi)
                    {
                        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzeyismi);
                        SDL_FreeSurface(textyuzeyismi);

                        int textgenislik, textuzunluk;
                        SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                        SDL_Rect textkonum = {x, y + SQUARE_SIZE - 13, textgenislik, textuzunluk};

                        SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                        SDL_DestroyTexture(textTexture);
                    }
                    else
                    {
                        SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                    }
                    char text[10];
                    snprintf(text, sizeof(text), "%d", okcuInKare[mevcutKare]);
                    SDL_Surface* textyuzey = TTF_RenderText_Solid(font, text, textrenk);
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);

                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum = {x + (SQUARE_SIZE - textgenislik) / 2, y + SQUARE_SIZE, textgenislik, textuzunluk};

                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);

                    int canbarigenislik = 40;
                    SDL_Rect canbarikonum = {x+4 + (SQUARE_SIZE - canbarigenislik) / 2, y, canbarigenislik, 5};

                    saglikOkcular = okcuInKare[mevcutKare]*80;
                    int canbaridolulugu = (saglikOkcular * canbarigenislik) / 100;

                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    canbarikonum.w = 30;
                    SDL_RenderFillRect(renderer, &canbarikonum);

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &canbarikonum);
                }
                mevcutKare++;
            }
        }
        mevcutKarePiyade = 0;
        for (int row = 0; row < GRID_ROWS; row++)
        {
            for (int col = 0; col < GRID_COLS; col++)
            {
                int x = baslangicX + col * SQUARE_SIZE + 210;
                int y = baslangicY + row+3 * SQUARE_SIZE;

                if (piyadeInKare[mevcutKarePiyade] > 0)
                {
                    SDL_Rect Piyadekonum = {x, y, SQUARE_SIZE, SQUARE_SIZE};

                    SDL_RenderCopy(renderer, piyadelerTexture, NULL, &Piyadekonum);
                    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);

                    SDL_Color textrenk = {255, 255, 255};
                    SDL_Surface* textyuzeyismi = TTF_RenderText_Solid(font, "Piyadeler", textrenk);
                    if (textyuzeyismi)
                    {
                        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzeyismi);
                        SDL_FreeSurface(textyuzeyismi);

                        int textgenislik, textuzunluk;
                        SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                        SDL_Rect textRect = {x, y + SQUARE_SIZE - 13, textgenislik, textuzunluk};

                        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                        SDL_DestroyTexture(textTexture);
                    }
                    else
                    {
                        SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                    }

                    char text[10];
                    snprintf(text, sizeof(text), "%d", piyadeInKare[mevcutKarePiyade]);
                    SDL_Surface* textyuzey = TTF_RenderText_Solid(font, text, textrenk);
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);

                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textRect = {x + (SQUARE_SIZE - textgenislik) / 2, y + SQUARE_SIZE, textgenislik, textuzunluk};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                    SDL_DestroyTexture(textTexture);

                    int canbarigenisligi = 30;
                    SDL_Rect canbarkonum = {x + (SQUARE_SIZE - canbarigenisligi) / 2, y, canbarigenisligi, 5};

                    saglikPiyadeler = piyadeInKare[mevcutKarePiyade]*100;
                    int canbardoluluk = (saglikPiyadeler * canbarigenisligi) / 100;

                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    canbarkonum.w = 30;
                    SDL_RenderFillRect(renderer, &canbarkonum);

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &canbarkonum);
                }
                mevcutKarePiyade++;
            }
        }
        mevcutKareSuvari = 0;
        for (int row = 0; row < GRID_ROWS; row++)
        {
            for (int col = 0; col < GRID_COLS; col++)
            {
                int x = baslangicX + col * SQUARE_SIZE+210;
                int y = baslangicY + row+6 * SQUARE_SIZE;
                if (suvariInKare[mevcutKareSuvari] > 0)
                {
                    SDL_Rect Suvarikonum = {x, y, SQUARE_SIZE, SQUARE_SIZE};

                    SDL_RenderCopy(renderer, suvarilerTexture, NULL, &Suvarikonum);
                    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 11);

                    SDL_Color textrenk = {255, 255, 255};
                    SDL_Surface* textyuzeyismi = TTF_RenderText_Solid(font, "Suvariler", textrenk);
                    if (textyuzeyismi)
                    {
                        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzeyismi);
                        SDL_FreeSurface(textyuzeyismi);

                        int textgenislik, textuzunluk;
                        SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                        SDL_Rect textkonum = {x, y + SQUARE_SIZE - 13, textgenislik, textuzunluk};

                        SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                        SDL_DestroyTexture(textTexture);
                    }
                    else
                    {
                        SDL_Log("Yaziyuzeyi olusturulamadi: %s", TTF_GetError());
                    }
                    char text[10];
                    snprintf(text, sizeof(text), "%d", suvariInKare[mevcutKareSuvari]);
                    SDL_Surface* textyuzey = TTF_RenderText_Solid(font, text, textrenk);
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);

                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum = {x + (SQUARE_SIZE - textgenislik) / 2, y + SQUARE_SIZE, textgenislik, textuzunluk};

                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);

                    int canbarigenisligi = 25;
                    SDL_Rect canbarkonum = {x-3+ (SQUARE_SIZE - canbarigenisligi) / 2, y, canbarigenisligi, 5};

                    saglikSuvariler =suvariInKare[mevcutKareSuvari] *120;
                    int canbardoluluk = (saglikSuvariler * canbarigenisligi) / 100;

                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    canbarkonum.w = 30;
                    SDL_RenderFillRect(renderer, &canbarkonum);

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &canbarkonum);
                }
                mevcutKareSuvari++;
            }
        }
        mevcutKareKusatma = 0;
        for (int row = 0; row < GRID_ROWS; row++)
        {
            for (int col = 0; col < GRID_COLS; col++)
            {
                int x = baslangicX + col * SQUARE_SIZE+210;
                int y = baslangicY + row+9 * SQUARE_SIZE;
                if (kusatmaInKare[mevcutKareKusatma] > 0)
                {
                    SDL_Rect Kusatmakonum = {x, y, SQUARE_SIZE, SQUARE_SIZE};

                    SDL_RenderCopy(renderer, kusatmaMakinesiTexture, NULL, &Kusatmakonum);
                    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);

                    SDL_Color textrenk = {255, 255, 255};
                    SDL_Surface* textSurfaceName = TTF_RenderText_Solid(font, "Kusatma M.", textrenk);

                    if (textSurfaceName)
                    {
                        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurfaceName);
                        SDL_FreeSurface(textSurfaceName);

                        int textgenislik, textuzunluk;
                        SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                        SDL_Rect textRect = {x, y + SQUARE_SIZE - 13, textgenislik, textuzunluk};

                        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                        SDL_DestroyTexture(textTexture);
                    }
                    else
                    {
                        SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                    }
                    char text[10];
                    snprintf(text, sizeof(text), "%d", kusatmaInKare[mevcutKareKusatma]);
                    SDL_Surface* textyuzey = TTF_RenderText_Solid(font, text, textrenk);
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);

                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textRect = {x + (SQUARE_SIZE - textgenislik) / 2, y + SQUARE_SIZE, textgenislik, textuzunluk};

                    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                    SDL_DestroyTexture(textTexture);

                    int canbarigenisligi = 20;
                    SDL_Rect canbarkonum = {x-5 + (SQUARE_SIZE - canbarigenisligi) / 2, y, canbarigenisligi, 5};

                    saglikKusatma = kusatmaInKare[mevcutKareKusatma]*150;
                    int canbardoluluk = (saglikKusatma * canbarigenisligi) / 100;

                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    canbarkonum.w = 30;
                    SDL_RenderFillRect(renderer, &canbarkonum);

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &canbarkonum);
                }
                mevcutKareKusatma++;
            }
        }
        mevcutKareOrk = 0;
        for (int row = 0; row < GRID_ROWS; row++)
        {
            for (int col = 0; col < GRID_COLS; col++)
            {
                int x = baslangicX + col * SQUARE_SIZE+210;
                int y = baslangicY + row+11 * SQUARE_SIZE;
                if (orkInKare[mevcutKareOrk] > 0)
                {
                    SDL_Rect Orkkonum= {x, y, SQUARE_SIZE, SQUARE_SIZE};

                    SDL_RenderCopy(renderer, orkDovuscuTexture, NULL, &Orkkonum);

                    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);

                    SDL_Color textrenk = {255, 255, 255};
                    SDL_Surface* textyuzeyismi = TTF_RenderText_Solid(font, "Ork", textrenk);
                    if (textyuzeyismi)
                    {
                        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzeyismi);
                        SDL_FreeSurface(textyuzeyismi);

                        int textgenislik, textuzunluk;
                        SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                        SDL_Rect textRect = {x, y + SQUARE_SIZE - 13, textgenislik, textuzunluk};

                        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                        SDL_DestroyTexture(textTexture);
                    }
                    else
                    {
                        SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                    }
                    SDL_Color textrenk2 = {255, 255, 255};
                    SDL_Surface* textyuzeyismi2 = TTF_RenderText_Solid(font, "Dovusculeri", textrenk2);
                    if (textyuzeyismi2)
                    {
                        SDL_Texture* textTexture2 = SDL_CreateTextureFromSurface(renderer, textyuzeyismi2);
                        SDL_FreeSurface(textyuzeyismi2);

                        int textgenislik, textuzunluk;
                        SDL_QueryTexture(textTexture2, NULL, NULL, &textgenislik, &textuzunluk);
                        SDL_Rect textRect = {x, y + SQUARE_SIZE -3, textgenislik, textuzunluk};

                        SDL_RenderCopy(renderer, textTexture2, NULL, &textRect);
                        SDL_DestroyTexture(textTexture2);
                    }
                    else
                    {
                        SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                    }
                    char text[10];
                    snprintf(text, sizeof(text), "%d", orkInKare[mevcutKareOrk]);
                    SDL_Surface* textyuzey = TTF_RenderText_Solid(font, text, textrenk);
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);

                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textRect = {x + (SQUARE_SIZE - textgenislik) / 2, y + SQUARE_SIZE+7, textgenislik, textuzunluk};

                    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                    SDL_DestroyTexture(textTexture);

                    int canbarigenisligi = 30;
                    SDL_Rect canbarkonum = {x + (SQUARE_SIZE - canbarigenisligi) / 2, y, canbarigenisligi, 5};

                    saglikOrk = orkInKare[mevcutKareOrk]*100;
                    int canbardoluluk = (saglikOrk * canbarigenisligi) / 100;

                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    canbarkonum.w = 30;
                    SDL_RenderFillRect(renderer, &canbarkonum);

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &canbarkonum);
                }
                mevcutKareOrk++;
            }
        }
        mevcutKareVarg = 0;
        for (int row = 0; row < GRID_ROWS; row++)
        {
            for (int col = 0; col < GRID_COLS; col++)
            {
                int x = baslangicX + col * SQUARE_SIZE+210;
                int y = baslangicY + row+13 * SQUARE_SIZE;

                if (vargInKare[mevcutKareVarg] > 0)
                {
                    SDL_Rect vargkonum = {x, y, SQUARE_SIZE, SQUARE_SIZE};

                    SDL_RenderCopy(renderer, vargBiniciTexture, NULL, &vargkonum);

                    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);

                    SDL_Color textrenk = {255, 255, 255};
                    SDL_Surface* textyuzeyismi = TTF_RenderText_Solid(font, "Varg", textrenk);

                    if (textyuzeyismi)
                    {
                        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzeyismi);
                        SDL_FreeSurface(textyuzeyismi);

                        int  textgenislik, textuzunluk;
                        SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                        SDL_Rect textRect = {x+9, y + SQUARE_SIZE - 13, textgenislik, textuzunluk};

                        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                        SDL_DestroyTexture(textTexture);
                    }
                    else
                    {
                        SDL_Log("Yaziyuzeyi olusturulamadi: %s", TTF_GetError());
                    }
                    SDL_Color textrenk2 = {255, 255, 255};
                    SDL_Surface* textyuzeyismi2 = TTF_RenderText_Solid(font, "Binicileri", textrenk2);

                    if (textyuzeyismi2)
                    {
                        SDL_Texture* textTexture2 = SDL_CreateTextureFromSurface(renderer, textyuzeyismi2);
                        SDL_FreeSurface(textyuzeyismi2);

                        int textgenislik, textuzunluk;
                        SDL_QueryTexture(textTexture2, NULL, NULL, &textgenislik, &textuzunluk);
                        SDL_Rect textRect = {x+1, y + SQUARE_SIZE, textgenislik, textuzunluk};

                        SDL_RenderCopy(renderer, textTexture2, NULL, &textRect);
                        SDL_DestroyTexture(textTexture2);
                    }
                    else
                    {
                        SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                    }
                    char text[10];
                    snprintf(text, sizeof(text), "%d", vargInKare[mevcutKareVarg]);
                    SDL_Surface* textyuzey = TTF_RenderText_Solid(font, text, textrenk);
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);

                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textRect = {x + (SQUARE_SIZE - textgenislik) / 2, y + SQUARE_SIZE+8, textgenislik, textuzunluk};

                    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                    SDL_DestroyTexture(textTexture);

                    int canbariningenislik = 25;
                    SDL_Rect canbarkonum = {x-3 + (SQUARE_SIZE - canbariningenislik) / 2, y, canbariningenislik, 5};

                    saglikVarg = vargInKare[mevcutKareVarg]*130;
                    int canbarinindoluluk = (saglikVarg * canbariningenislik) / 100;

                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    canbarkonum.w = 30;
                    SDL_RenderFillRect(renderer, &canbarkonum);
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &canbarkonum);
                }
                mevcutKareVarg++;
            }
        }
        mevcutKareMizrakci = 0;
        for (int row = 0; row < GRID_ROWS; row++)
        {
            for (int col = 0; col < GRID_COLS; col++)
            {
                int x = baslangicX + col * SQUARE_SIZE+210;
                int y = baslangicY + row+15 * SQUARE_SIZE;
                if (mizrakInKare[mevcutKareMizrakci] > 0)
                {
                    SDL_Rect mizrakkonum = {x, y, SQUARE_SIZE, SQUARE_SIZE};
                    SDL_RenderCopy(renderer, mizrakciTexture, NULL, &mizrakkonum);

                    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);

                    SDL_Color textrenk = {255, 255, 255};
                    SDL_Surface* textyuzeyismi = TTF_RenderText_Solid(font, "Mizrakcilar", textrenk);
                    if (textyuzeyismi)
                    {
                        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzeyismi);
                        SDL_FreeSurface(textyuzeyismi);

                        int textgenislik, textuzunluk;
                        SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                        SDL_Rect textRect = {x, y + SQUARE_SIZE - 13, textgenislik, textuzunluk};

                        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                        SDL_DestroyTexture(textTexture);
                    }
                    else
                    {
                        SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                    }
                    char text[10];
                    snprintf(text, sizeof(text), "%d", mizrakInKare[mevcutKareMizrakci]);
                    SDL_Surface* textyuzey = TTF_RenderText_Solid(font, text, textrenk);
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);

                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textRect = {x + (SQUARE_SIZE - textgenislik) / 2, y + SQUARE_SIZE, textgenislik, textuzunluk};

                    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                    SDL_DestroyTexture(textTexture);

                    int canbariningenislik = 35;
                    SDL_Rect canbarikonum = {x+3 + (SQUARE_SIZE - canbariningenislik) / 2, y, canbariningenislik, 5};

                    saglikMizrakcilar = mizrakInKare[mevcutKareMizrakci]*90;
                    int canbarinindoluluk = (saglikMizrakcilar * canbariningenislik) / 100;

                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    canbarikonum.w = 30;
                    SDL_RenderFillRect(renderer, &canbarikonum);

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &canbarikonum);
                }
                mevcutKareMizrakci++;
            }
        }
        mevcutKareTroll = 0;
        for (int row = 0; row < GRID_ROWS; row++)
        {
            for (int col = 0; col < GRID_COLS; col++)
            {
                int x = baslangicX + col * SQUARE_SIZE+210;
                int y = baslangicY + row+17 * SQUARE_SIZE;
                if (trollInKare[mevcutKareTroll] > 0)
                {
                    SDL_Rect trollkonum = {x, y, SQUARE_SIZE, SQUARE_SIZE};
                    SDL_RenderCopy(renderer, trollerTexture, NULL, &trollkonum);
                    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 13);

                    SDL_Color textkonum = {255, 255, 255};
                    SDL_Surface* textyuzeyismi = TTF_RenderText_Solid(font, "Troller", textkonum);

                    if (textyuzeyismi)
                    {
                        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzeyismi);
                        SDL_FreeSurface(textyuzeyismi);
                        int textgenislik, textuzunluk;
                        SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                        SDL_Rect textRect = {x, y + SQUARE_SIZE - 13, textgenislik, textuzunluk};
                        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                        SDL_DestroyTexture(textTexture);
                    }
                    else
                    {
                        SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                    }
                    char text[10];
                    snprintf(text, sizeof(text), "%d", trollInKare[mevcutKareTroll]);
                    SDL_Surface* textyuzey = TTF_RenderText_Solid(font, text, textkonum);
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);
                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textRect = {x + (SQUARE_SIZE - textgenislik) / 2, y + SQUARE_SIZE, textgenislik, textuzunluk};

                    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                    SDL_DestroyTexture(textTexture);

                    int canbariningenislik = 15;
                    SDL_Rect canbarikonum = {x-6 + (SQUARE_SIZE - canbariningenislik) / 2, y, canbariningenislik, 5};

                    saglikTroller = trollInKare[mevcutKareTroll]*200;
                    int canbaridoluluk = (saglikTroller * canbariningenislik) / 100;

                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    canbarikonum.w = 30;
                    SDL_RenderFillRect(renderer, &canbarikonum);

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &canbarikonum);
                }
                mevcutKareTroll++;
            }
        }
        int heroRow = 7;
        int heroCol = 2;
        int alparslanX = baslangicX + heroCol * SQUARE_SIZE;
        int alparslanY = baslangicY + heroRow * SQUARE_SIZE;
        SDL_Rect alparslankonum = {alparslanX, alparslanY, SQUARE_SIZE, SQUARE_SIZE};
        if (strcmp(kahramanismiinsan, "Alparslan") == 0)
        {
            SDL_RenderCopy(renderer, alparslanTexture, NULL, &alparslankonum);
            TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
            SDL_Color kirmizirenk = {255, 0, 0, 255}; // Kırmızı
            SDL_Surface* textyuzey = TTF_RenderText_Solid(font, " Alparslan", textrenk);
            if (textyuzey)
            {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                SDL_FreeSurface(textyuzey);
                int textgenislik, textuzunluk;
                SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                SDL_Rect textkonum = {alparslanX, alparslanY + SQUARE_SIZE, textgenislik, textuzunluk};

                SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                SDL_DestroyTexture(textTexture);
            }
            else
            {
                SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
            }
        }
        if (strcmp(CanavarismiInsan,"Ejderha")==0)
        {
            SDL_RenderCopy(renderer, ejderhaTexture, NULL, &ejderhakonum);
            TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
            SDL_Color textrenk = {255, 255, 255};
            SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Ejderha", textrenk);
            if (textyuzey)
            {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                SDL_FreeSurface(textyuzey);
                int  textgenislik, textuzunluk;
                SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                SDL_Rect textkonum = {ejderhaX, ejderhaY + SQUARE_SIZE, textgenislik, textuzunluk};
                SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                SDL_DestroyTexture(textTexture);
            }
            else
            {
                SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
            }
        }
        if (strcmp(CanavarismiInsan,"Tepegoz")==0)
        {
            SDL_RenderCopy(renderer, tepegozTexture, NULL, &tepegozkonum);
            TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
            SDL_Color textrenk = {255, 255, 255};
            SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Tepegoz", textrenk);
            if (textyuzey)
            {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                SDL_FreeSurface(textyuzey);
                int textgenislik, textuzunluk;
                SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                SDL_Rect textkonum = {tepegozX, tepegozY + SQUARE_SIZE, textgenislik, textuzunluk};
                SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                SDL_DestroyTexture(textTexture);
            }
            else
            {
                SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
            }
        }
        if (strcmp(CanavarismiInsan,"Agri_Dagi_Devleri")==0)
        {
            SDL_RenderCopy(renderer, agriDagiDeviTexture, NULL, &agriDagikonum);
            TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
            SDL_Color textrenk = {255, 255, 255};
            SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Agri Dagi Devi", textrenk);
            if (textyuzey)
            {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                SDL_FreeSurface(textyuzey);
                int textgenislik, textuzunluk;
                SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                SDL_Rect textkonum = {agriDagiX, agriDagiY + SQUARE_SIZE, textgenislik, textuzunluk};
                SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                SDL_DestroyTexture(textTexture);
            }
            else
            {
                SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
            }
        }
        if (strcmp(CanavarismiInsan,"Karakurt")==0)
        {
            SDL_RenderCopy(renderer, karakurtTexture, NULL, &karakurtkonum);
            TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
            SDL_Color textrenk = {255, 255, 255};
            SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Karakurt", textrenk);
            if (textyuzey)
            {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                SDL_FreeSurface(textyuzey);
                int textgenislik, textuzunluk;
                SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                SDL_Rect textkonum = {karakurtX, karakurtY + SQUARE_SIZE, textgenislik, textuzunluk};
                SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                SDL_DestroyTexture(textTexture);
            }
            else
            {
                SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
            }
        }
        if (strcmp(CanavarismiInsan,"Samur")==0)
        {
            SDL_RenderCopy(renderer, samurTexture, NULL, &samurkonum);
            TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
            SDL_Color textrenk = {255, 255, 255};
            SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Samur", textrenk);
            if (textyuzey)
            {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                SDL_FreeSurface(textyuzey);
                int textgenislik, textuzunluk;
                SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                SDL_Rect textkonum = {samurX, samurY + SQUARE_SIZE, textgenislik, textuzunluk};
                SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                SDL_DestroyTexture(textTexture);
            }
            else
            {
                SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
            }
        }
        if (strcmp(CanavarismiOrk,"Golge_Kurtlari")==0)
        {
            SDL_RenderCopy(renderer, golgeKurtTexture, NULL, &GolgeKurtKonum);
            TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
            SDL_Color textrenk = {255, 255, 255};
            SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Golge Kurtlari", textrenk);
            if (textyuzey)
            {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                SDL_FreeSurface(textyuzey);
                int textgenislik, textuzunluk;
                SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                SDL_Rect textkonum = {golgeKurtX, golgeKurtY + SQUARE_SIZE, textgenislik, textuzunluk};
                SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                SDL_DestroyTexture(textTexture);
            }
            else
            {
                SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
            }
        }
        if (strcmp(CanavarismiOrk,"Buz_Devi")==0)
        {
            SDL_RenderCopy(renderer, buzDeviTexture, NULL, &buzDeviKonum);
            TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
            SDL_Color textrenk = {255, 255, 255};
            SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Buz Devi", textrenk);
            if (textyuzey)
            {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                SDL_FreeSurface(textyuzey);
                int textgenislik, textuzunluk;
                SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                SDL_Rect textkonum= {buzDeviX, buzDeviY + SQUARE_SIZE, textgenislik, textuzunluk};
                SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                SDL_DestroyTexture(textTexture);
            }
            else
            {
                SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
            }
        }
        if (strcmp(CanavarismiOrk,"Kara_Troll")==0)
        {
            SDL_RenderCopy(renderer, karaTrollTexture, NULL, &karaTrollKonum);
            TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
            SDL_Color textrenk = {255, 255, 255};
            SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Kara Troll", textrenk);
            if (textyuzey)
            {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                SDL_FreeSurface(textyuzey);
                int textgenislik, textuzunluk;
                SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                SDL_Rect textkonum= {karaTrollX, karaTrollY + SQUARE_SIZE, textgenislik, textuzunluk};
                SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                SDL_DestroyTexture(textTexture);
            }
            else
            {
                SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
            }
        }
        if (strcmp(CanavarismiOrk,"Camur_Devleri")==0)
        {
            SDL_RenderCopy(renderer, camurDevTexture, NULL, &camurDevKonum);
            TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
            SDL_Color textrenk= {255, 255, 255};
            SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Camur Devi", textrenk);
            if (textyuzey)
            {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                SDL_FreeSurface(textyuzey);
                int  textgenislik, textuzunluk;
                SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                SDL_Rect textkonum= {camurDevX, camurDevY + SQUARE_SIZE, textgenislik, textuzunluk};
                SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                SDL_DestroyTexture(textTexture);
            }
            else
            {
                SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
            }
        }
        if (strcmp(CanavarismiOrk,"Ates_Iblisi")==0)
        {
            SDL_RenderCopy(renderer, atesIblisiTexture, NULL, &atesIblisKonum);
            TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
            SDL_Color textrenk = {255, 255, 255};
            SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Ates Iblisi", textrenk);
            if (textyuzey)
            {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                SDL_FreeSurface(textyuzey);
                int textgenislik, textuzunluk;
                SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                SDL_Rect textkonum = {atesIblisX, atesIblisY + SQUARE_SIZE, textgenislik, textuzunluk};
                SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                SDL_DestroyTexture(textTexture);
            }
            else
            {
                SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
            }
        }
        if (strcmp(CanavarismiOrk,"Makrog_Savas_Beyi")==0)
        {
            SDL_RenderCopy(renderer, makrogTexture, NULL, &makrogKonum);
            TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
            SDL_Color textrenk = {255, 255, 255};
            SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Makrog Savas\nBeyi", textrenk);
            if (textyuzey)
            {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                SDL_FreeSurface(textyuzey);
                int textgenislik, textuzunluk;
                SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                SDL_Rect textkonum= {makrogX, makrogY + SQUARE_SIZE, textgenislik, textuzunluk};
                SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                SDL_DestroyTexture(textTexture);
            }
            else
            {
                SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
            }
        }

        if (strcmp(kahramanismiinsan,"Alparslan")==0)
        {
            SDL_RenderCopy(renderer, alparslanTexture, NULL, &alparslankonum);
            TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
            SDL_Color textrenk = {255, 255, 255};
            SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Alparslan", textrenk);
            if (textyuzey)
            {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                SDL_FreeSurface(textyuzey);
                int textgenislik, textuzunluk;
                SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                SDL_Rect textkonum= {alparslanX, alparslanY + SQUARE_SIZE, textgenislik, textuzunluk};
                SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                SDL_DestroyTexture(textTexture);
            }
            else
            {
                SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
            }
        }
        if (strcmp(kahramanismiinsan,"Yavuz_Sultan_Selim")==0)
        {
            SDL_RenderCopy(renderer, yavuzSultanTexture, NULL, &yavuzSultanKonum);
            TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
            SDL_Color textrenk = {255, 255, 255};
            SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Yavuz Sultan Selim", textrenk);
            if (textyuzey)
            {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                SDL_FreeSurface(textyuzey);
                int textgenislik, textuzunluk;
                SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                SDL_Rect textkonum = {yavuzSultanX, yavuzSultanY + SQUARE_SIZE, textgenislik, textuzunluk};
                SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                SDL_DestroyTexture(textTexture);
            }
            else
            {
                SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
            }
        }
        if (strcmp(kahramanismiinsan,"Fatih_Sultan_Mehmet")==0)
        {
            SDL_RenderCopy(renderer, fatihSultanTexture, NULL, &fatihSultanKonum);
            TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
            SDL_Color textrenk= {255, 255, 255};
            SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Fatih Sultan Mehmet", textrenk);
            if (textyuzey)
            {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                SDL_FreeSurface(textyuzey);
                int textgenislik, textuzunluk;
                SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                SDL_Rect textkonum = {fatihSultanX, fatihSultanY + SQUARE_SIZE, textgenislik, textuzunluk};
                SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                SDL_DestroyTexture(textTexture);
            }
            else
            {
                SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
            }
        }
        if (strcmp(kahramanismiinsan,"Mete_Han")==0)
        {
            SDL_RenderCopy(renderer, meteHanTexture, NULL, &metehanKonum);
            TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
            SDL_Color textrenk = {255, 255, 255};
            SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "MeteHan", textrenk);
            if (textyuzey)
            {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                SDL_FreeSurface(textyuzey);
                int textgenislik, textuzunluk;
                SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                SDL_Rect textkonum = {metehanX, metehanY + SQUARE_SIZE, textgenislik, textuzunluk};
                SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                SDL_DestroyTexture(textTexture);
            }
            else
            {
                SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
            }
        }
        if (strcmp(kahramanismiinsan,"Tugrul_Bey")==0)
        {
            SDL_RenderCopy(renderer, tugrulBeyTexture, NULL, &tugrulBeyKonum);
            TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
            SDL_Color textrenk = {255, 255, 255};
            SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Tugrul Bey", textrenk);
            if (textyuzey)
            {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                SDL_FreeSurface(textyuzey);
                int textgenislik, textuzunluk;
                SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                SDL_Rect textkonum = {tugrulBeyX, tugrulBeyY + SQUARE_SIZE, textgenislik, textuzunluk};
                SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                SDL_DestroyTexture(textTexture);
            }
            else
            {
                SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
            }
        }
        if (strcmp(kahramanismiOrk,"Vrog_Kafakiran")==0)
        {
            SDL_RenderCopy(renderer, vrogTexture, NULL, &vrogKonum);
            TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
            SDL_Color textrenk = {255, 255, 255};
            SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Vrog Kafakiran", textrenk);
            if (textyuzey)
            {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                SDL_FreeSurface(textyuzey);
                int textgenislik, textuzunluk;
                SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                SDL_Rect textkonum = {vrogX, vrogY + SQUARE_SIZE, textgenislik, textuzunluk};
                SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                SDL_DestroyTexture(textTexture);
            }
            else
            {
                SDL_Log("Yazi yuzeyi olusturulamadi:%s", TTF_GetError());
            }
        }
        if (strcmp(kahramanismiOrk,"Goruk_Vahsi")==0)
        {
            SDL_RenderCopy(renderer, gorukVahsiTexture, NULL, &gorukKonum);
            TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
            SDL_Color textrenk= {255, 255, 255};
            SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Goruk Vahsi", textrenk);
            if (textyuzey)
            {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                SDL_FreeSurface(textyuzey);
                int textgenislik, textuzunluk;
                SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                SDL_Rect textkonum = {gorukX, gorukY + SQUARE_SIZE, textgenislik, textuzunluk};
                SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                SDL_DestroyTexture(textTexture);
            }
            else
            {
                SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
            }
        }
        if (strcmp(kahramanismiOrk,"Thruk_Kemikkiran")==0)
        {
            SDL_RenderCopy(renderer, thrukTexture, NULL, &thrukKonum);
            TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
            SDL_Color textrenk= {255, 255, 255};
            SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Thruk Kemikkiran", textrenk);
            if (textyuzey)
            {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                SDL_FreeSurface(textyuzey);
                int textgenislik, textuzunluk;
                SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                SDL_Rect textkonum = {thrukX, thrukY + SQUARE_SIZE, textgenislik, textuzunluk};
                SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                SDL_DestroyTexture(textTexture);
            }
            else
            {
                SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
            }
        }
        if (strcmp(kahramanismiOrk,"Ugar_Zalim")==0)
        {
            SDL_RenderCopy(renderer, ugarTexture, NULL, &ugarKonum);
            TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
            SDL_Color textrenk= {255, 255, 255};
            SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Ugar Zalim", textrenk);
            if (textyuzey)
            {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                SDL_FreeSurface(textyuzey);
                int textgenislik, textuzunluk;
                SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                SDL_Rect textkonum = {ugarX, ugarY + SQUARE_SIZE, textgenislik, textuzunluk};
                SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                SDL_DestroyTexture(textTexture);
            }
            else
            {
                SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(4000);
        break ;
    }
    printf("SDL dongusu bitti, diger kodlar calismaya devam ediyor...\n");
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyTexture(arkaPlanTexture);
SDL_DestroyTexture(okcularTexture);
SDL_DestroyTexture(piyadelerTexture);
SDL_DestroyTexture(suvarilerTexture);
SDL_DestroyTexture(kusatmaMakinesiTexture);
SDL_DestroyTexture(orkDovuscuTexture);
SDL_DestroyTexture(vargBiniciTexture);
SDL_DestroyTexture(mizrakciTexture);
SDL_DestroyTexture(trollerTexture);
SDL_DestroyTexture(ejderhaTexture);
SDL_DestroyTexture(agriDagiDeviTexture);
SDL_DestroyTexture(tepegozTexture);
SDL_DestroyTexture(karakurtTexture);
SDL_DestroyTexture(samurTexture);
SDL_DestroyTexture(golgeKurtTexture);
SDL_DestroyTexture(buzDeviTexture);
SDL_DestroyTexture(karaTrollTexture);
SDL_DestroyTexture(camurDevTexture);
SDL_DestroyTexture(atesIblisiTexture);
SDL_DestroyTexture(makrogTexture);
SDL_DestroyTexture(alparslanTexture);
SDL_DestroyTexture(yavuzSultanTexture);
SDL_DestroyTexture(fatihSultanTexture);
SDL_DestroyTexture(meteHanTexture);
SDL_DestroyTexture(tugrulBeyTexture);
SDL_DestroyTexture(gorukVahsiTexture);
SDL_DestroyTexture(thrukTexture);
SDL_DestroyTexture(vrogTexture);
SDL_DestroyTexture(ugarTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(pencere);
    SDL_Quit();

    while(1)
    {
        insanBoostluSavunmaGucu= ((int)ceil(insanImparatorlugu.okcular.birlik_sayisi)*insanImparatorlugu.okcular.saldiri)+((int)ceil(insanImparatorlugu.suvariler.birlik_sayisi)*insanImparatorlugu.suvariler.saldiri) + ((int)ceil(insanImparatorlugu.kusatma_makineleri.birlik_sayisi)*insanImparatorlugu.kusatma_makineleri.saldiri)+((int)ceil(insanImparatorlugu.piyadeler.birlik_sayisi)*insanImparatorlugu.piyadeler.saldiri);
        insanBoostluSavunmaGucu= ((int)ceil(insanImparatorlugu.okcular.birlik_sayisi)*insanImparatorlugu.okcular.savunma)+((int)ceil(insanImparatorlugu.suvariler.birlik_sayisi)*insanImparatorlugu.suvariler.savunma) + ((int)ceil(insanImparatorlugu.kusatma_makineleri.birlik_sayisi)*insanImparatorlugu.kusatma_makineleri.savunma)+((int)ceil(insanImparatorlugu.piyadeler.birlik_sayisi)*insanImparatorlugu.piyadeler.savunma);
        orkLegiBoostluSavunmaGucu=((int)ceil(orkLegi.ork_dovusculeri.birlik_sayisi)*orkLegi.ork_dovusculeri.savunma) + ((int)ceil(orkLegi.mizrakcilar.birlik_sayisi)*orkLegi.mizrakcilar.savunma)+ ((int)ceil(orkLegi.varg_binicileri.birlik_sayisi)*orkLegi.varg_binicileri.savunma) +((int)ceil(orkLegi.troller.birlik_sayisi)*orkLegi.troller.savunma);
        orkLegiBoostluSaldiriGucu= ((int)ceil(orkLegi.ork_dovusculeri.birlik_sayisi)*orkLegi.ork_dovusculeri.saldiri) + ((int)ceil(orkLegi.mizrakcilar.birlik_sayisi)*orkLegi.mizrakcilar.saldiri)+ ((int)ceil(orkLegi.varg_binicileri.birlik_sayisi)*orkLegi.varg_binicileri.saldiri) +((int)ceil(orkLegi.troller.birlik_sayisi)*orkLegi.troller.saldiri);
        netHasarInsan = insanBoostluSaldiriGucu*(1-(orkLegiBoostluSavunmaGucu/insanBoostluSaldiriGucu)) ;
        netHasarOrk= orkLegiBoostluSaldiriGucu*(1-(insanBoostluSavunmaGucu/orkLegiBoostluSaldiriGucu)) ;

        if(netHasarInsan<0)
        {
            netHasarInsan=0;
        }
        if(netHasarOrk<0)
        {
            netHasarOrk=0;
        }

        printf ("Insanlar anlik saldiri gucu : %lf\n", insanBoostluSaldiriGucu) ;
        fprintf (Simulator, "Insanlar anlik saldiri gucu : %lf\n", insanBoostluSaldiriGucu);
        printf ("Insanlar anlik savunma gucu : %lf\n", insanBoostluSavunmaGucu) ;
        fprintf (Simulator,"Insanlar anlik savunma gucu : %lf\n", insanBoostluSavunmaGucu);
        printf ("Orklar anlik saldiri gucu : %lf\n", orkLegiBoostluSaldiriGucu) ;
        fprintf (Simulator,"Orklar anlik saldiri gucu : %lf\n", orkLegiBoostluSaldiriGucu);
        printf ("Orklar anlik savunma gucu : %lf\n", orkLegiBoostluSavunmaGucu) ;
        fprintf (Simulator, "Insanlar anlik savunma gucu : %lf\n", orkLegiBoostluSavunmaGucu);

        sayac++;
        if(sayac%5==0)
        {
            insanImparatorlugu.piyadeler.saldiri=insanImparatorlugu.piyadeler.saldiri-insanImparatorlugu.piyadeler.saldiri*10/100;
            insanImparatorlugu.okcular.saldiri=insanImparatorlugu.okcular.saldiri-insanImparatorlugu.okcular.saldiri*10/100;
            insanImparatorlugu.kusatma_makineleri.saldiri=insanImparatorlugu.kusatma_makineleri.saldiri-insanImparatorlugu.kusatma_makineleri.saldiri*10/100;
            insanImparatorlugu.suvariler.saldiri=insanImparatorlugu.suvariler.saldiri-insanImparatorlugu.suvariler.saldiri*10/100;
            insanImparatorlugu.piyadeler.savunma=insanImparatorlugu.piyadeler.savunma-insanImparatorlugu.piyadeler.savunma*10/100;
            insanImparatorlugu.okcular.savunma=insanImparatorlugu.okcular.savunma-insanImparatorlugu.okcular.savunma*10/100;
            insanImparatorlugu.kusatma_makineleri.savunma=insanImparatorlugu.kusatma_makineleri.savunma-insanImparatorlugu.kusatma_makineleri.savunma*10/100;
            insanImparatorlugu.suvariler.savunma=insanImparatorlugu.suvariler.savunma-insanImparatorlugu.suvariler.savunma*10/100;
            orkLegi.varg_binicileri.saldiri=orkLegi.varg_binicileri.saldiri-orkLegi.varg_binicileri.saldiri*10/100;
            orkLegi.mizrakcilar.saldiri=orkLegi.mizrakcilar.saldiri-orkLegi.mizrakcilar.saldiri*10/100;
            orkLegi.troller.saldiri=orkLegi.troller.saldiri-orkLegi.troller.saldiri*10/100;
            orkLegi.ork_dovusculeri.saldiri=orkLegi.ork_dovusculeri.saldiri-orkLegi.ork_dovusculeri.saldiri*10/100;
            orkLegi.varg_binicileri.savunma=orkLegi.varg_binicileri.savunma-orkLegi.varg_binicileri.savunma*10/100;
            orkLegi.mizrakcilar.savunma=orkLegi.mizrakcilar.savunma-orkLegi.mizrakcilar.savunma*10/100;
            orkLegi.troller.savunma=orkLegi.troller.savunma-orkLegi.troller.savunma*10/100;
            orkLegi.ork_dovusculeri.savunma=orkLegi.ork_dovusculeri.savunma-orkLegi.ork_dovusculeri.savunma*10/100;
            insanBoostluSaldiriGucu= ((int)ceil(insanImparatorlugu.okcular.birlik_sayisi)*insanImparatorlugu.okcular.saldiri)+((int)ceil(insanImparatorlugu.suvariler.birlik_sayisi)*insanImparatorlugu.suvariler.saldiri) + ((int)ceil(insanImparatorlugu.kusatma_makineleri.birlik_sayisi)*insanImparatorlugu.kusatma_makineleri.saldiri)+((int)ceil(insanImparatorlugu.piyadeler.birlik_sayisi)*insanImparatorlugu.piyadeler.saldiri);
            insanBoostluSavunmaGucu= ((int)ceil(insanImparatorlugu.okcular.birlik_sayisi)*insanImparatorlugu.okcular.savunma)+((int)ceil(insanImparatorlugu.suvariler.birlik_sayisi)*insanImparatorlugu.suvariler.savunma) + ((int)ceil(insanImparatorlugu.kusatma_makineleri.birlik_sayisi)*insanImparatorlugu.kusatma_makineleri.savunma)+((int)ceil(insanImparatorlugu.piyadeler.birlik_sayisi)*insanImparatorlugu.piyadeler.savunma);
            orkLegiBoostluSavunmaGucu=((int)ceil(orkLegi.ork_dovusculeri.birlik_sayisi)*orkLegi.ork_dovusculeri.savunma) + ((int)ceil(orkLegi.mizrakcilar.birlik_sayisi)*orkLegi.mizrakcilar.savunma)+ ((int)ceil(orkLegi.varg_binicileri.birlik_sayisi)*orkLegi.varg_binicileri.savunma) +((int)ceil(orkLegi.troller.birlik_sayisi)*orkLegi.troller.savunma);
            orkLegiBoostluSaldiriGucu= ((int)ceil(orkLegi.ork_dovusculeri.birlik_sayisi)*orkLegi.ork_dovusculeri.saldiri) + ((int)ceil(orkLegi.mizrakcilar.birlik_sayisi)*orkLegi.mizrakcilar.saldiri)+ ((int)ceil(orkLegi.varg_binicileri.birlik_sayisi)*orkLegi.varg_binicileri.saldiri) +((int)ceil(orkLegi.troller.birlik_sayisi)*orkLegi.troller.saldiri);
            netHasarInsan = insanBoostluSaldiriGucu*(1-(orkLegiBoostluSavunmaGucu/insanBoostluSaldiriGucu)) ;
            netHasarOrk= orkLegiBoostluSaldiriGucu*(1-(insanBoostluSavunmaGucu/orkLegiBoostluSaldiriGucu)) ;
            printf("%10 azaltildi");
            fprintf (Simulator,"YORGUNLUK ! %10 AZALTILDI.");
            if(netHasarInsan<0)
            {
                netHasarInsan=0;
            }
            if(netHasarOrk<0)
            {
                netHasarOrk=0;
            }
        }
        if(sayac == (int)(100/insanImparatorlugu.piyadeler.kritik_sans))
        {
            insanBoostluSaldiriGucu= ((int)ceil(insanImparatorlugu.okcular.birlik_sayisi)*insanImparatorlugu.okcular.saldiri)+((int)ceil(insanImparatorlugu.suvariler.birlik_sayisi)*insanImparatorlugu.suvariler.saldiri) + (insanImparatorlugu.kusatma_makineleri.birlik_sayisi*insanImparatorlugu.kusatma_makineleri.saldiri)+((int)ceil(insanImparatorlugu.piyadeler.birlik_sayisi)*insanImparatorlugu.piyadeler.saldiri*1.5);
            netHasarInsan = insanBoostluSaldiriGucu*(1-(orkLegiBoostluSavunmaGucu/insanBoostluSaldiriGucu)) ;
        }
        if(sayac == (int)(100/insanImparatorlugu.okcular.kritik_sans))
        {
            insanBoostluSaldiriGucu=((int)ceil(insanImparatorlugu.okcular.birlik_sayisi)*insanImparatorlugu.okcular.saldiri*1.5)+((int)ceil(insanImparatorlugu.suvariler.birlik_sayisi)*insanImparatorlugu.suvariler.saldiri) + (insanImparatorlugu.kusatma_makineleri.birlik_sayisi*insanImparatorlugu.kusatma_makineleri.saldiri)+((int)ceil(insanImparatorlugu.piyadeler.birlik_sayisi)*insanImparatorlugu.piyadeler.saldiri);
            netHasarInsan = insanBoostluSaldiriGucu*(1-(orkLegiBoostluSavunmaGucu/insanBoostluSaldiriGucu)) ;
        }
        if(sayac == (int)(100/insanImparatorlugu.suvariler.kritik_sans))
        {
            insanBoostluSaldiriGucu= ((int)ceil(insanImparatorlugu.okcular.birlik_sayisi)*insanImparatorlugu.okcular.saldiri)+((int)ceil(insanImparatorlugu.suvariler.birlik_sayisi)*insanImparatorlugu.suvariler.saldiri*1.5) + (insanImparatorlugu.kusatma_makineleri.birlik_sayisi*insanImparatorlugu.kusatma_makineleri.saldiri)+((int)ceil(insanImparatorlugu.piyadeler.birlik_sayisi)*insanImparatorlugu.piyadeler.saldiri);
            netHasarInsan = insanBoostluSaldiriGucu*(1-(orkLegiBoostluSavunmaGucu/insanBoostluSaldiriGucu)) ;
        }
        if(sayac == (int)(100/insanImparatorlugu.kusatma_makineleri.kritik_sans))
        {
            insanBoostluSaldiriGucu= ((int)ceil(insanImparatorlugu.okcular.birlik_sayisi)*insanImparatorlugu.okcular.saldiri)+((int)ceil(insanImparatorlugu.suvariler.birlik_sayisi)*insanImparatorlugu.suvariler.saldiri) + (insanImparatorlugu.kusatma_makineleri.birlik_sayisi*insanImparatorlugu.kusatma_makineleri.saldiri*1.5)+((int)ceil(insanImparatorlugu.piyadeler.birlik_sayisi)*insanImparatorlugu.piyadeler.saldiri);
            netHasarInsan = insanBoostluSaldiriGucu*(1-(orkLegiBoostluSavunmaGucu/insanBoostluSaldiriGucu)) ;
        }
        if(netHasarInsan<0)
        {
            netHasarInsan=0;
        }
        if(netHasarOrk<0)
        {
            netHasarOrk=0;
        }

        printf("\nInsanlar vuracak tur :%d\n",sayac);
        fprintf (Simulator,"\nInsanlar vuracak tur :%d\n",sayac);
        //OrklegiToplamTemelSaglikGucu=(orkLegi.ork_dovusculeri.birlik_sayisi*orkLegi.ork_dovusculeri.saglik)+(orkLegi.mizrakcilar.birlik_sayisi*orkLegi.mizrakcilar.saglik)+(orkLegi.varg_binicileri.birlik_sayisi*orkLegi.varg_binicileri.saglik)+(orkLegi.troller.birlik_sayisi*orkLegi.troller.saglik) ;
        OrklegiToplamTemelSaglikGucu=OrklegiToplamTemelSaglikGucu-netHasarInsan;
        if(OrklegiToplamTemelSaglikGucu<=0)
        {
            orkLegi.mizrakcilar.birlik_sayisi=0;
            orkLegi.ork_dovusculeri.birlik_sayisi=0;
            orkLegi.troller.birlik_sayisi=0;
            orkLegi.varg_binicileri.birlik_sayisi=0;
            OrklegiToplamTemelSaglikGucu=0;
        }
        printf ("\nOrk leginin insan tarafindan yedigi hasar : %lf", netHasarInsan) ;
        fprintf (Simulator, "\nOrk leginin insan tarafindan yedigi hasar : %lf", netHasarInsan) ;

        float OrkDovuscusuYedigiHasar = (((int)ceil(orkLegi.ork_dovusculeri.birlik_sayisi)*orkLegi.ork_dovusculeri.savunma)/orkLegiBoostluSavunmaGucu)*netHasarInsan;
        printf("\nOrk leginde ork_dovuscusunun yedigi hasar : %f",OrkDovuscusuYedigiHasar);
        fprintf (Simulator,"\nOrk leginde ork_dovuscusunun yedigi hasar : %f",OrkDovuscusuYedigiHasar);
        orkLegi.ork_dovusculeri.birlik_sayisi= orkLegi.ork_dovusculeri.birlik_sayisi-(OrkDovuscusuYedigiHasar/orkLegi.ork_dovusculeri.saglik);
        if(orkLegi.ork_dovusculeri.birlik_sayisi<0)
        {
            orkLegi.ork_dovusculeri.birlik_sayisi=0;
        }
        printf("\nbirlik sayisi:%d", (int)ceil(orkLegi.ork_dovusculeri.birlik_sayisi));
        fprintf(Simulator, "\nbirlik sayisi: %d", (int)ceil(orkLegi.ork_dovusculeri.birlik_sayisi));

        float mizrakcilarinYedigiHasar = (((int)ceil(orkLegi.mizrakcilar.birlik_sayisi)*orkLegi.mizrakcilar.savunma)/orkLegiBoostluSavunmaGucu)*netHasarInsan;
        printf("\nOrk leginde mizrakcilarin yedigi hasar : %f",mizrakcilarinYedigiHasar);
        fprintf(Simulator, "\nOrk leginde mizrakcilarin yedigi hasar: %f", mizrakcilarinYedigiHasar);

        orkLegi.mizrakcilar.birlik_sayisi= orkLegi.mizrakcilar.birlik_sayisi-(mizrakcilarinYedigiHasar/orkLegi.mizrakcilar.saglik);
        if(orkLegi.mizrakcilar.birlik_sayisi<0)
        {
            orkLegi.mizrakcilar.birlik_sayisi=0;
        }
        printf("\nbirlik sayisi:%d", (int)ceil(orkLegi.mizrakcilar.birlik_sayisi));
        fprintf(Simulator, "\nbirlik sayisi: %d", (int)ceil(orkLegi.mizrakcilar.birlik_sayisi));

        float trollerinYedigiHasar = (((int)ceil(orkLegi.troller.birlik_sayisi)*orkLegi.troller.savunma)/orkLegiBoostluSavunmaGucu)*netHasarInsan;
        printf("\nOrk leginde trollerin yedigi hasar : %f",trollerinYedigiHasar);
        fprintf(Simulator, "\nOrk leginde trollerin yedigi hasar : %f", trollerinYedigiHasar);

        orkLegi.troller.birlik_sayisi= orkLegi.troller.birlik_sayisi-(trollerinYedigiHasar/orkLegi.troller.saglik);
        if(orkLegi.troller.birlik_sayisi<0)
        {
            orkLegi.troller.birlik_sayisi=0;
        }
        printf("\nbirlik sayisi:%d", (int)ceil(orkLegi.troller.birlik_sayisi));
        fprintf(Simulator, "\nbirlik sayisi:%d", (int)ceil(orkLegi.troller.birlik_sayisi));

        float varg_binicilerinYedigiHasar = (((int)ceil(orkLegi.varg_binicileri.birlik_sayisi)*orkLegi.varg_binicileri.savunma)/orkLegiBoostluSavunmaGucu)*netHasarInsan;
        printf("\nOrk leginde varg_binicilerinin yedigi hasar : %f",varg_binicilerinYedigiHasar);
        fprintf(Simulator, "\nOrk leginde varg_binicilerinin yedigi hasar : %f", varg_binicilerinYedigiHasar);

        orkLegi.varg_binicileri.birlik_sayisi= orkLegi.varg_binicileri.birlik_sayisi-(varg_binicilerinYedigiHasar/orkLegi.varg_binicileri.saglik);
        if(orkLegi.varg_binicileri.birlik_sayisi<0)
        {
            orkLegi.varg_binicileri.birlik_sayisi=0;
        }
        printf("\nbirlik sayisi:%d", (int)ceil(orkLegi.varg_binicileri.birlik_sayisi));
        fprintf(Simulator, "\nbirlik sayisi: %d", (int)ceil(orkLegi.varg_binicileri.birlik_sayisi));

        if(OrklegiToplamTemelSaglikGucu<=0)
        {
            OrklegiToplamTemelSaglikGucu=0;
            insanKazandi=1 ;
            printf ("\n\nORK KALAN CAN : %f\n", OrklegiToplamTemelSaglikGucu) ;
            printf("\nINSANLAR KAZANDI..");
            fprintf(Simulator, "\n\nORK KALAN CAN : %f\n", OrklegiToplamTemelSaglikGucu);
            fprintf(Simulator, "\nINSANLAR KAZANDI..");
            fclose(Simulator) ;
            break;
        }
        printf ("\n\nORK KALAN CAN : %f\n", OrklegiToplamTemelSaglikGucu) ;
        fprintf(Simulator, "\n\nORK KALAN CAN : %f\n", OrklegiToplamTemelSaglikGucu);
        sayac++;
        if(sayac%5==0)
        {
            insanImparatorlugu.piyadeler.saldiri=insanImparatorlugu.piyadeler.saldiri-insanImparatorlugu.piyadeler.saldiri*10/100;
            insanImparatorlugu.okcular.saldiri=insanImparatorlugu.okcular.saldiri-insanImparatorlugu.okcular.saldiri*10/100;
            insanImparatorlugu.kusatma_makineleri.saldiri=insanImparatorlugu.kusatma_makineleri.saldiri-insanImparatorlugu.kusatma_makineleri.saldiri*10/100;
            insanImparatorlugu.suvariler.saldiri=insanImparatorlugu.suvariler.saldiri-insanImparatorlugu.suvariler.saldiri*10/100;
            insanImparatorlugu.piyadeler.savunma=insanImparatorlugu.piyadeler.savunma-insanImparatorlugu.piyadeler.savunma*10/100;
            insanImparatorlugu.okcular.savunma=insanImparatorlugu.okcular.savunma-insanImparatorlugu.okcular.savunma*10/100;
            insanImparatorlugu.kusatma_makineleri.savunma=insanImparatorlugu.kusatma_makineleri.savunma-insanImparatorlugu.kusatma_makineleri.savunma*10/100;
            insanImparatorlugu.suvariler.savunma=insanImparatorlugu.suvariler.savunma-insanImparatorlugu.suvariler.savunma*10/100;
            orkLegi.varg_binicileri.saldiri=orkLegi.varg_binicileri.saldiri-orkLegi.varg_binicileri.saldiri*10/100;
            orkLegi.mizrakcilar.saldiri=orkLegi.mizrakcilar.saldiri-orkLegi.mizrakcilar.saldiri*10/100;
            orkLegi.troller.saldiri=orkLegi.troller.saldiri-orkLegi.troller.saldiri*10/100;
            orkLegi.ork_dovusculeri.saldiri=orkLegi.ork_dovusculeri.saldiri-orkLegi.ork_dovusculeri.saldiri*10/100;
            orkLegi.varg_binicileri.savunma=orkLegi.varg_binicileri.savunma-orkLegi.varg_binicileri.savunma*10/100;
            orkLegi.mizrakcilar.savunma=orkLegi.mizrakcilar.savunma-orkLegi.mizrakcilar.savunma*10/100;
            orkLegi.troller.savunma=orkLegi.troller.savunma-orkLegi.troller.savunma*10/100;
            orkLegi.ork_dovusculeri.savunma=orkLegi.ork_dovusculeri.savunma-orkLegi.ork_dovusculeri.savunma*10/100;
            insanBoostluSaldiriGucu= ((int)ceil(insanImparatorlugu.okcular.birlik_sayisi)*insanImparatorlugu.okcular.saldiri)+((int)ceil(insanImparatorlugu.suvariler.birlik_sayisi)*insanImparatorlugu.suvariler.saldiri) + ((int)ceil(insanImparatorlugu.kusatma_makineleri.birlik_sayisi)*insanImparatorlugu.kusatma_makineleri.saldiri)+((int)ceil(insanImparatorlugu.piyadeler.birlik_sayisi)*insanImparatorlugu.piyadeler.saldiri);
            insanBoostluSavunmaGucu= ((int)ceil(insanImparatorlugu.okcular.birlik_sayisi)*insanImparatorlugu.okcular.savunma)+((int)ceil(insanImparatorlugu.suvariler.birlik_sayisi)*insanImparatorlugu.suvariler.savunma) + ((int)ceil(insanImparatorlugu.kusatma_makineleri.birlik_sayisi)*insanImparatorlugu.kusatma_makineleri.savunma)+((int)ceil(insanImparatorlugu.piyadeler.birlik_sayisi)*insanImparatorlugu.piyadeler.savunma);
            orkLegiBoostluSavunmaGucu=((int)ceil(orkLegi.ork_dovusculeri.birlik_sayisi)*orkLegi.ork_dovusculeri.savunma) + ((int)ceil(orkLegi.mizrakcilar.birlik_sayisi)*orkLegi.mizrakcilar.savunma)+ ((int)ceil(orkLegi.varg_binicileri.birlik_sayisi)*orkLegi.varg_binicileri.savunma) +((int)ceil(orkLegi.troller.birlik_sayisi)*orkLegi.troller.savunma);
            orkLegiBoostluSaldiriGucu= ((int)ceil(orkLegi.ork_dovusculeri.birlik_sayisi)*orkLegi.ork_dovusculeri.saldiri) + ((int)ceil(orkLegi.mizrakcilar.birlik_sayisi)*orkLegi.mizrakcilar.saldiri)+ ((int)ceil(orkLegi.varg_binicileri.birlik_sayisi)*orkLegi.varg_binicileri.saldiri) +((int)ceil(orkLegi.troller.birlik_sayisi)*orkLegi.troller.saldiri);
            netHasarInsan = insanBoostluSaldiriGucu*(1-(orkLegiBoostluSavunmaGucu/insanBoostluSaldiriGucu)) ;
            netHasarOrk= orkLegiBoostluSaldiriGucu*(1-(insanBoostluSavunmaGucu/orkLegiBoostluSaldiriGucu)) ;
            printf("%10 azaltildi");
            fprintf (Simulator, "YORGUNLUK ! %10 AZALTILDI.") ;
            if(netHasarInsan<0)
            {
                netHasarInsan=0;
            }
            if(netHasarOrk<0)
            {
                netHasarOrk=0;
            }
        }
        if(sayac == (int)(100/orkLegi.ork_dovusculeri.kritik_sans))
        {
            orkLegiBoostluSaldiriGucu= ((int)ceil(orkLegi.ork_dovusculeri.birlik_sayisi)*orkLegi.ork_dovusculeri.saldiri*1.5) + ((int)ceil(orkLegi.mizrakcilar.birlik_sayisi)*orkLegi.mizrakcilar.saldiri)+ ((int)ceil(orkLegi.varg_binicileri.birlik_sayisi)*orkLegi.varg_binicileri.saldiri) +((int)ceil(orkLegi.troller.birlik_sayisi)*orkLegi.troller.saldiri);
            netHasarOrk= orkLegiBoostluSaldiriGucu*(1-(insanBoostluSavunmaGucu/orkLegiBoostluSaldiriGucu)) ;
        }
        if(sayac == (int)(100/orkLegi.varg_binicileri.kritik_sans))
        {
            orkLegiBoostluSaldiriGucu= ((int)ceil(orkLegi.ork_dovusculeri.birlik_sayisi)*orkLegi.ork_dovusculeri.saldiri) + ((int)ceil(orkLegi.mizrakcilar.birlik_sayisi)*orkLegi.mizrakcilar.saldiri)+ ((int)ceil(orkLegi.varg_binicileri.birlik_sayisi)*orkLegi.varg_binicileri.saldiri*1.5) +((int)ceil(orkLegi.troller.birlik_sayisi)*orkLegi.troller.saldiri);
            netHasarOrk= orkLegiBoostluSaldiriGucu*(1-(insanBoostluSavunmaGucu/orkLegiBoostluSaldiriGucu)) ;
        }
        if(sayac == (int)(100/orkLegi.mizrakcilar.kritik_sans))
        {
            orkLegiBoostluSaldiriGucu= ((int)ceil(orkLegi.ork_dovusculeri.birlik_sayisi)*orkLegi.ork_dovusculeri.saldiri) + ((int)ceil(orkLegi.mizrakcilar.birlik_sayisi)*orkLegi.mizrakcilar.saldiri*1.5)+ ((int)ceil(orkLegi.varg_binicileri.birlik_sayisi)*orkLegi.varg_binicileri.saldiri) +((int)ceil(orkLegi.troller.birlik_sayisi)*orkLegi.troller.saldiri);
            netHasarOrk= orkLegiBoostluSaldiriGucu*(1-(insanBoostluSavunmaGucu/orkLegiBoostluSaldiriGucu)) ;
        }
        if(sayac ==(int)(100/orkLegi.troller.kritik_sans))
        {
            orkLegiBoostluSaldiriGucu= ((int)ceil(orkLegi.ork_dovusculeri.birlik_sayisi)*orkLegi.ork_dovusculeri.saldiri) + ((int)ceil(orkLegi.mizrakcilar.birlik_sayisi)*orkLegi.mizrakcilar.saldiri)+ ((int)ceil(orkLegi.varg_binicileri.birlik_sayisi)*orkLegi.varg_binicileri.saldiri) +((int)ceil(orkLegi.troller.birlik_sayisi)*orkLegi.troller.saldiri*1.5);
            netHasarOrk= orkLegiBoostluSaldiriGucu*(1-(insanBoostluSavunmaGucu/orkLegiBoostluSaldiriGucu)) ;
        }
        if(netHasarInsan<0)
        {
            netHasarInsan=0;
        }
        if(netHasarOrk<0)
        {
            netHasarOrk=0;
        }
        printf("\nOrk legi vuracak tur:%d\n",sayac);
        fprintf(Simulator, "\nOrk legi vuracak tur: %d\n", sayac);

        InsanToplamTemelSaglikGucu=InsanToplamTemelSaglikGucu-netHasarOrk;
        printf ("\nInsanlarin Orklar tarafindan yedigi hasar :  %lf",netHasarOrk);
        fprintf(Simulator, "\nInsanlarin Orklar tarafindan yedigi hasar :  %lf", netHasarOrk);

        float PiyadelerinYedigiHasar = (((int)ceil(insanImparatorlugu.piyadeler.birlik_sayisi)*insanImparatorlugu.piyadeler.savunma)/insanBoostluSavunmaGucu)*netHasarOrk;
        if(InsanToplamTemelSaglikGucu<=0)
        {
            insanImparatorlugu.okcular.birlik_sayisi=0;
            insanImparatorlugu.piyadeler.birlik_sayisi=0;
            insanImparatorlugu.suvariler.birlik_sayisi=0;
            insanImparatorlugu.kusatma_makineleri.birlik_sayisi=0;
            InsanToplamTemelSaglikGucu=0;
        }
        printf("\nInsan imparatorlugunda piyadelerin yedigi hasar : %f",PiyadelerinYedigiHasar);
        fprintf(Simulator, "\nInsan imparatorlugunda piyadelerin yedigi hasar : %f", PiyadelerinYedigiHasar);

        insanImparatorlugu.piyadeler.birlik_sayisi= insanImparatorlugu.piyadeler.birlik_sayisi-(PiyadelerinYedigiHasar/insanImparatorlugu.piyadeler.saglik);
        if(insanImparatorlugu.piyadeler.birlik_sayisi<0)
        {
            insanImparatorlugu.piyadeler.birlik_sayisi=0;
        }
        printf("\nbirlik sayisi:%d", (int)ceil(insanImparatorlugu.piyadeler.birlik_sayisi));
        fprintf(Simulator, "\nbirlik sayisi:%d", (int)ceil(insanImparatorlugu.piyadeler.birlik_sayisi));

        float OkcularinYedigiHasar = (((int)ceil(insanImparatorlugu.okcular.birlik_sayisi)*insanImparatorlugu.okcular.savunma)/insanBoostluSavunmaGucu)*netHasarOrk;
        printf("\nInsan imparatorlugunda okcularin yedigi hasar : %f",OkcularinYedigiHasar);
        fprintf(Simulator, "\nInsan imparatorlugunda okcularin yedigi hasar : %f", OkcularinYedigiHasar);

        insanImparatorlugu.okcular.birlik_sayisi= insanImparatorlugu.okcular.birlik_sayisi-(OkcularinYedigiHasar/insanImparatorlugu.okcular.saglik);
        if(insanImparatorlugu.okcular.birlik_sayisi<0)
        {
            insanImparatorlugu.okcular.birlik_sayisi=0;
        }
        printf("\nbirlik sayisi:%d", (int)ceil(insanImparatorlugu.okcular.birlik_sayisi));
        fprintf(Simulator, "\nbirlik sayisi:%d", (int)ceil(insanImparatorlugu.okcular.birlik_sayisi));

        float SuvarilerinYedigiHasar = (((int)ceil(insanImparatorlugu.suvariler.birlik_sayisi)*insanImparatorlugu.suvariler.savunma)/insanBoostluSavunmaGucu)*netHasarOrk;
        printf("\nInsan imparatorlugunda suvarilerin yedigi hasar : %f",SuvarilerinYedigiHasar);
        fprintf(Simulator, "\nInsan imparatorlugunda suvarilerin yedigi hasar : %f", SuvarilerinYedigiHasar);

        insanImparatorlugu.suvariler.birlik_sayisi= insanImparatorlugu.suvariler.birlik_sayisi-(SuvarilerinYedigiHasar/insanImparatorlugu.suvariler.saglik);
        if(insanImparatorlugu.suvariler.birlik_sayisi<0)
        {
            insanImparatorlugu.suvariler.birlik_sayisi=0;
        }
        printf("\nbirlik sayisi:%d", (int)ceil(insanImparatorlugu.suvariler.birlik_sayisi));
        fprintf(Simulator, "\nbirlik sayisi:%d", (int)ceil(insanImparatorlugu.suvariler.birlik_sayisi));

        float kusatma_makinelerinYedigiHasar = (((int)ceil(insanImparatorlugu.kusatma_makineleri.birlik_sayisi)*insanImparatorlugu.kusatma_makineleri.savunma)/insanBoostluSavunmaGucu)*netHasarOrk;
        printf("\nInsan imparatorlugunda kusatma makineleri yedigi hasar : %f",kusatma_makinelerinYedigiHasar);
        fprintf(Simulator, "\nInsan imparatorlugunda kusatma makineleri yedigi hasar : %f", kusatma_makinelerinYedigiHasar);

        insanImparatorlugu.kusatma_makineleri.birlik_sayisi= insanImparatorlugu.kusatma_makineleri.birlik_sayisi-(kusatma_makinelerinYedigiHasar/insanImparatorlugu.kusatma_makineleri.saglik);
        if(insanImparatorlugu.kusatma_makineleri.birlik_sayisi<0)
        {
            insanImparatorlugu.kusatma_makineleri.birlik_sayisi=0;
        }
        printf("\nbirlik sayisi:%d", (int)ceil(insanImparatorlugu.kusatma_makineleri.birlik_sayisi));
        fprintf(Simulator, "\nbirlik sayisi:%d", (int)ceil(insanImparatorlugu.kusatma_makineleri.birlik_sayisi));

        if(InsanToplamTemelSaglikGucu<=0)
        {
            InsanToplamTemelSaglikGucu=0;
            insanImparatorlugu.okcular.birlik_sayisi=0;
            insanImparatorlugu.piyadeler.birlik_sayisi=0;
            insanImparatorlugu.suvariler.birlik_sayisi=0;
            insanImparatorlugu.kusatma_makineleri.birlik_sayisi=0;
            OrkKazandi=1 ;
            printf ("\n\nINSAN KALAN CAN : %f", InsanToplamTemelSaglikGucu);
            printf("\nORK LEGI KAZANDI..");
            fprintf(Simulator, "\n\nINSAN KALAN CAN : %f", InsanToplamTemelSaglikGucu);
            fprintf(Simulator, "\nORK LEGI KAZANDI..");

            break;
        }
        printf ("\n\nINSAN KALAN CAN : %f", InsanToplamTemelSaglikGucu);
        fprintf(Simulator, "\n\nINSAN KALAN CAN : %f", InsanToplamTemelSaglikGucu);
    }
    fclose(Simulator) ;


    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("SDL baslatilamadi: %s", SDL_GetError());
        return 1;
    }
    pencere = SDL_CreateWindow("Kare Izgara", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (!pencere)
    {
        SDL_Log("Pencere olusturulamadi: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    renderer = SDL_CreateRenderer(pencere, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        SDL_Log("Renderer olusturulamadi: %s", SDL_GetError());
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    arkaPlanYuzeyi = SDL_LoadBMP("background.bmp");
    if (!arkaPlanYuzeyi)
    {
        SDL_Log("Resim yuklenemedi: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    arkaPlanTexture = SDL_CreateTextureFromSurface(renderer, arkaPlanYuzeyi);
    SDL_FreeSurface(arkaPlanYuzeyi);
    okcularYuzeyi = IMG_Load("okcular.png");
    if (!okcularYuzeyi)
    {
        SDL_Log("Okcular resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    okcularTexture = SDL_CreateTextureFromSurface(renderer, okcularYuzeyi);
    SDL_FreeSurface(okcularYuzeyi);
    SDL_GetWindowSize(pencere, &ekran_genislik, &ekran_yukseklik);
    oyunAlaniGenislik = GRID_COLS * SQUARE_SIZE;
    oyunAlaniYukseklik = GRID_ROWS * SQUARE_SIZE;
    baslangicX = (ekran_genislik - oyunAlaniGenislik) / 2;
    baslangicY = (ekran_yukseklik - oyunAlaniYukseklik) / 2;

    maxOkcuPerKare = 100;
    toplamOkcu = (int)ceil(insanImparatorlugu.okcular.birlik_sayisi);
    kalanOkcu = toplamOkcu;
    mevcutKare = 0;

    toplamKare = GRID_ROWS * GRID_COLS;

    okcuInKare[toplamKare];
    memset(okcuInKare, 0, sizeof(okcuInKare));

    for (int i = 0; i < toplamOkcu && mevcutKare < toplamKare; i++)
    {
        okcuInKare[mevcutKare]++;
        if (okcuInKare[mevcutKare] >= maxOkcuPerKare)
        {
            mevcutKare++;
        }
    }
    piyadelerYuzeyi = IMG_Load("piyadeler.png");
    if (!piyadelerYuzeyi)
    {
        SDL_Log("Piyadeler resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    piyadelerTexture = SDL_CreateTextureFromSurface(renderer, piyadelerYuzeyi);
    SDL_FreeSurface(piyadelerYuzeyi);
    maxPiyadePerKare = 100;
    toplamPiyade = (int)ceil(insanImparatorlugu.piyadeler.birlik_sayisi);
    kalanPiyade = toplamPiyade;
    mevcutKarePiyade = 0;

    piyadeInKare[toplamKare];
    memset(piyadeInKare, 0, sizeof(piyadeInKare));

    for (int i = 0; i < toplamPiyade && mevcutKarePiyade < toplamKare; i++)
    {
        piyadeInKare[mevcutKarePiyade]++;
        if (piyadeInKare[mevcutKarePiyade] >= maxPiyadePerKare)
        {
            mevcutKarePiyade++;
        }
    }
    suvarilerYuzeyi = IMG_Load("suvariler.png"); // PNG dosyasını yükle
    if (!suvarilerYuzeyi)
    {
        SDL_Log("Suvariler resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    suvarilerTexture = SDL_CreateTextureFromSurface(renderer, suvarilerYuzeyi);
    SDL_FreeSurface(suvarilerYuzeyi);  // Yüzeyi serbest bırak

    maxSuvariPerKare = 100;
    toplamSuvari = (int)ceil(insanImparatorlugu.suvariler.birlik_sayisi);
    kalanSuvari = toplamSuvari;
    mevcutKareSuvari = 0;

    suvariInKare[toplamKare];
    memset(suvariInKare, 0, sizeof(suvariInKare));

    for (int i = 0; i < toplamSuvari && mevcutKareSuvari < toplamKare; i++)
    {
        suvariInKare[mevcutKareSuvari]++;
        if (suvariInKare[mevcutKareSuvari] >= maxSuvariPerKare)
        {
            mevcutKareSuvari++;
        }
    }
    kusatmaMakinesiYuzeyi = IMG_Load("kusatma_makineleri.png");
    if (!kusatmaMakinesiYuzeyi)
    {
        SDL_Log("Kusatma makineleri resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }

    kusatmaMakinesiTexture = SDL_CreateTextureFromSurface(renderer, kusatmaMakinesiYuzeyi);
    SDL_FreeSurface(kusatmaMakinesiYuzeyi);

    maxKusatmaPerKare = 100;
    toplamKusatma = (int)ceil(insanImparatorlugu.kusatma_makineleri.birlik_sayisi);
    kalanKusatma = toplamKusatma;
    mevcutKareKusatma = 0;

    kusatmaInKare[toplamKare];
    memset(kusatmaInKare, 0, sizeof(kusatmaInKare));

    for (int i = 0; i < toplamKusatma && mevcutKareKusatma < toplamKare; i++)
    {
        kusatmaInKare[mevcutKareKusatma]++;
        if (kusatmaInKare[mevcutKareKusatma] >= maxKusatmaPerKare)
        {
            mevcutKareKusatma++;
        }
    }
    orkDovuscuYuzeyi = IMG_Load("ork_dovusculeri.png");
    if (!orkDovuscuYuzeyi)
    {
        SDL_Log("ork_dovusculeri resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    orkDovuscuTexture = SDL_CreateTextureFromSurface(renderer, orkDovuscuYuzeyi);
    SDL_FreeSurface(orkDovuscuYuzeyi);

    maxOrkPerKare = 100;
    toplamOrk = (int)ceil(orkLegi.ork_dovusculeri.birlik_sayisi);
    kalanOrk = toplamOrk;
    mevcutKareOrk = 0;

    orkInKare[toplamKare];
    memset(orkInKare, 0, sizeof(orkInKare));
    for (int i = 0; i < toplamOrk && mevcutKareOrk < toplamKare; i++)
    {
        orkInKare[mevcutKareOrk]++;
        if (orkInKare[mevcutKareOrk] >= maxOrkPerKare)
        {
            mevcutKareOrk++;
        }
    }
    vargBiniciYuzeyi = IMG_Load("varg_binicileri.png");
    if (!vargBiniciYuzeyi)
    {
        SDL_Log("varg_binicileri resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    vargBiniciTexture = SDL_CreateTextureFromSurface(renderer, vargBiniciYuzeyi);
    SDL_FreeSurface(vargBiniciYuzeyi);

    maxVargPerKare = 100;
    toplamVarg = (int)ceil(orkLegi.varg_binicileri.birlik_sayisi);
    kalanVarg = toplamVarg;
    mevcutKareVarg = 0;

    vargInKare[toplamKare];
    memset(vargInKare, 0, sizeof(vargInKare));
    for (int i = 0; i < toplamVarg && mevcutKareVarg < toplamKare; i++)
    {
        vargInKare[mevcutKareVarg]++;
        if (vargInKare[mevcutKareVarg] >= maxVargPerKare)
        {
            mevcutKareVarg++;
        }
    }
    mizrakciYuzeyi = IMG_Load("mizrakcilar.png");
    if (!mizrakciYuzeyi)
    {
        SDL_Log("mizrakcilar.png resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    mizrakciTexture = SDL_CreateTextureFromSurface(renderer, mizrakciYuzeyi);
    SDL_FreeSurface(mizrakciYuzeyi);

    maxMizrakciPerKare = 100;
    toplamMizrakci = (int)ceil(orkLegi.mizrakcilar.birlik_sayisi);
    kalanMizrakci = toplamMizrakci;
    mevcutKareMizrakci = 0;

    mizrakInKare[toplamKare];
    memset(mizrakInKare, 0, sizeof(mizrakInKare));

    for (int i = 0; i < toplamMizrakci && mevcutKareMizrakci < toplamKare; i++)
    {
        mizrakInKare[mevcutKareMizrakci]++;
        if (mizrakInKare[mevcutKareMizrakci] >= maxMizrakciPerKare)
        {
            mevcutKareMizrakci++;
        }
    }
    trollerYuzeyi = IMG_Load("troller.png");
    if (!trollerYuzeyi)
    {
        SDL_Log("Troller resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    trollerTexture = SDL_CreateTextureFromSurface(renderer, trollerYuzeyi);
    SDL_FreeSurface(trollerYuzeyi);

    maxTrollPerKare = 100;
    toplamTroll = (int)ceil(orkLegi.troller.birlik_sayisi);
    kalanTroll = toplamTroll;
    mevcutKareTroll = 0;

    trollInKare[toplamKare];
    memset(trollInKare, 0, sizeof(trollInKare));
    for (int i = 0; i < toplamTroll && mevcutKareTroll < toplamKare; i++)
    {
        trollInKare[mevcutKareTroll]++;
        if (trollInKare[mevcutKareTroll] >= maxTrollPerKare)
        {
            mevcutKareTroll++;
        }
    }
    ejderhaYuzeyi = IMG_Load("ejderha.png");
    if (!ejderhaYuzeyi)
    {
        SDL_Log("ejderha resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    ejderhaTexture = SDL_CreateTextureFromSurface(renderer, ejderhaYuzeyi);
    SDL_FreeSurface(ejderhaYuzeyi);

    tepegozYuzeyi = IMG_Load("tepegoz.png");
    if (!tepegozYuzeyi)
    {
        SDL_Log("Tepegoz resmi yüklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    tepegozTexture = SDL_CreateTextureFromSurface(renderer, tepegozYuzeyi);
    SDL_FreeSurface(tepegozYuzeyi);

    agriDagiDeviYuzeyi = IMG_Load("agri_dagi_devleri.png");
    if (!agriDagiDeviYuzeyi)
    {
        SDL_Log("Agri dagi devi resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    agriDagiDeviTexture = SDL_CreateTextureFromSurface(renderer, agriDagiDeviYuzeyi);
    SDL_FreeSurface(agriDagiDeviYuzeyi);

    karakurtYuzeyi = IMG_Load("karakurt.png");
    if (!karakurtYuzeyi)
    {
        SDL_Log("Karakurt  resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    karakurtTexture = SDL_CreateTextureFromSurface(renderer, karakurtYuzeyi);
    SDL_FreeSurface(karakurtYuzeyi);

    samurYuzeyi = IMG_Load("samur.png");
    if (!samurYuzeyi)
    {
        SDL_Log("samur  resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    samurTexture = SDL_CreateTextureFromSurface(renderer, samurYuzeyi);
    SDL_FreeSurface(samurYuzeyi);

    golgeKurtYuzeyi = IMG_Load("golge_kurtlari.png");
    if (!golgeKurtYuzeyi)
    {
        SDL_Log("golge_kurtlari  resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    golgeKurtTexture = SDL_CreateTextureFromSurface(renderer, golgeKurtYuzeyi);
    SDL_FreeSurface(golgeKurtYuzeyi);

    buzDeviYuzeyi = IMG_Load("buz_devi.png");
    if (!buzDeviYuzeyi)
    {
        SDL_Log("BUz devi resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    buzDeviTexture = SDL_CreateTextureFromSurface(renderer, buzDeviYuzeyi);
    SDL_FreeSurface(buzDeviYuzeyi);

    karaTrollYuzeyi = IMG_Load("kara_troll.png");
    if (!karaTrollYuzeyi)
    {
        SDL_Log("kara_troll resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    karaTrollTexture = SDL_CreateTextureFromSurface(renderer, karaTrollYuzeyi);
    SDL_FreeSurface(karaTrollYuzeyi);

    camurDevYuzeyi = IMG_Load("camur_devi.png");
    if (!camurDevYuzeyi)
    {
        SDL_Log("camur_devi resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    camurDevTexture = SDL_CreateTextureFromSurface(renderer, camurDevYuzeyi);
    SDL_FreeSurface(camurDevYuzeyi);

    atesIblisiYuzeyi = IMG_Load("ates_iblisi.png");
    if (!atesIblisiYuzeyi)
    {
        SDL_Log("ates_iblisi resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    atesIblisiTexture= SDL_CreateTextureFromSurface(renderer, atesIblisiYuzeyi);
    SDL_FreeSurface(atesIblisiYuzeyi);
    makrogYuzeyi = IMG_Load("makrog_savas_beyi.png");
    if (!makrogYuzeyi)
    {
        SDL_Log("makrog_savas_beyi resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    makrogTexture = SDL_CreateTextureFromSurface(renderer, makrogYuzeyi);
    SDL_FreeSurface(makrogYuzeyi);

    gorukVahsiYuzeyi = IMG_Load("Goruk_Vahsi.png");
    if (!gorukVahsiYuzeyi)
    {
        SDL_Log("Goruk_Vahsi resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    gorukVahsiTexture = SDL_CreateTextureFromSurface(renderer, gorukVahsiYuzeyi);
    SDL_FreeSurface(gorukVahsiYuzeyi);

    thrukYuzeyi = IMG_Load("Thruk_Kemikkiran.png");
    if (!thrukYuzeyi)
    {
        SDL_Log("Thruh_Kemikkiran resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    thrukTexture = SDL_CreateTextureFromSurface(renderer, thrukYuzeyi);
    SDL_FreeSurface(thrukYuzeyi);

    vrogYuzeyi = IMG_Load("Vrog_Kafakiran.png");
    if (!vrogYuzeyi)
    {
        SDL_Log("Vrog_Kafakiran resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    vrogTexture = SDL_CreateTextureFromSurface(renderer, vrogYuzeyi);
    SDL_FreeSurface(vrogYuzeyi);

    ugarYuzeyi = IMG_Load("Ugar_Zalim.png");
    if (!ugarYuzeyi)
    {
        SDL_Log("Ugar_Zalim resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    ugarTexture = SDL_CreateTextureFromSurface(renderer, ugarYuzeyi);
    SDL_FreeSurface(ugarYuzeyi);

    alparslanYuzeyi = IMG_Load("Alparslan.png");
    if (!alparslanYuzeyi)
    {
        SDL_Log("Alparslan resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    alparslanTexture = SDL_CreateTextureFromSurface(renderer, alparslanYuzeyi);
    SDL_FreeSurface(alparslanYuzeyi);

    yavuzSultanYuzeyi= IMG_Load("Yavuz_Sultan_Selim.png");
    if (!yavuzSultanYuzeyi)
    {
        SDL_Log("Yavuz_Sultan_Selim resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    yavuzSultanTexture = SDL_CreateTextureFromSurface(renderer, yavuzSultanYuzeyi);
    SDL_FreeSurface(yavuzSultanYuzeyi);

    fatihSultanYuzeyi = IMG_Load("Fatih_Sultan_Mehmet.png");
    if (!fatihSultanYuzeyi)
    {
        SDL_Log("Fatih_Sultan_Mehmet resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    fatihSultanTexture= SDL_CreateTextureFromSurface(renderer, fatihSultanYuzeyi);
    SDL_FreeSurface(fatihSultanYuzeyi);

    meteHanYuzeyi = IMG_Load("Mete_Han.png");
    if (!meteHanYuzeyi)
    {
        SDL_Log("Mete_Han resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    meteHanTexture = SDL_CreateTextureFromSurface(renderer, meteHanYuzeyi);
    SDL_FreeSurface(meteHanYuzeyi);

    tugrulBeyYuzeyi = IMG_Load("Tugrul_Bey.png");
    if (!tugrulBeyYuzeyi)
    {
        SDL_Log("Tugrul_Bey resmi yuklenemedi: %s", IMG_GetError());
        SDL_DestroyTexture(arkaPlanTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    tugrulBeyTexture = SDL_CreateTextureFromSurface(renderer, tugrulBeyYuzeyi);
    SDL_FreeSurface(tugrulBeyYuzeyi);
    srand(SDL_GetTicks());
    SDL_GetWindowSize(pencere, &ekran_genislik, &ekran_yukseklik);

    if (TTF_Init() == -1)
    {
        SDL_Log("SDL_ttf baslatılamadi: %s", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    if (!font)
    {
        SDL_Log("Font yuklenemedi: %s", TTF_GetError());
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(pencere);
        SDL_Quit();
        return 1;
    }
    running = 1;
    event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, arkaPlanTexture, NULL, NULL);
        for (int row = 0; row < GRID_ROWS; row++)
        {
            for (int col = 0; col < GRID_COLS; col++)
            {
                int x = baslangicX + col * SQUARE_SIZE;
                int y = baslangicY + row * SQUARE_SIZE;
                SDL_Color renk;
                if (row < GRID_ROWS / 2)
                {
                    renk = (SDL_Color)
                    {
                        0, 100, 0, 255
                    };
                }
                else
                {
                    renk = (SDL_Color)
                    {
                        0, 0, 139, 255
                    };
                }
                KareleriDoldur(renderer, x, y, SQUARE_SIZE, renk);
                SDL_Color kenarrenk = {0, 0, 0, 255};
                KareCiz(renderer, x, y, SQUARE_SIZE, kenarrenk);
            }
        }

        SDL_Color textrenk = {255, 255, 255};
        mevcutKare = 0;
        for (int row = 0; row < GRID_ROWS; row++)
        {
            for (int col = 0; col < GRID_COLS; col++)
            {
                int x = baslangicX + col * SQUARE_SIZE + 210 ;
                int y = baslangicY + row * SQUARE_SIZE;
                if (okcuInKare[mevcutKare] > 0)
                {
                    SDL_Rect okcukonum = {x, y, SQUARE_SIZE, SQUARE_SIZE};
                    SDL_RenderCopy(renderer, okcularTexture, NULL, &okcukonum);
                    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 11);
                    SDL_Color textrenk = {255, 255, 255};
                    SDL_Surface* textyuzeyisim = TTF_RenderText_Solid(font, "Okcular", textrenk);
                    if (textyuzeyisim)
                    {
                        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzeyisim);
                        SDL_FreeSurface(textyuzeyisim);

                        int textgenislik, textuzunluk;
                        SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                        SDL_Rect textRect = {x, y + SQUARE_SIZE - 13, textgenislik, textuzunluk};
                        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                        SDL_DestroyTexture(textTexture);
                    }
                    else
                    {
                        SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                    }
                    char text[10];
                    snprintf(text, sizeof(text), "%d", (int)ceil(okcuInKare[mevcutKare]));
                    SDL_Surface* textyuzey = TTF_RenderText_Solid(font, text, textrenk);
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);

                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum = {x + (SQUARE_SIZE - textgenislik) / 2, y + SQUARE_SIZE, textgenislik, textuzunluk};

                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);

                    int canbarigenislik = 30;
                    SDL_Rect canbarikonum = {x + (SQUARE_SIZE - canbarigenislik) / 2, y, canbarigenislik, 5};

                    int saglik = okcuInKare[mevcutKare]*80;
                    int canbaridoluluk = (saglik * canbarigenislik) / 100;

                    if(saglik > saglikOkcular*40/100)
                    {
                        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                        canbarikonum.w = 30;
                        SDL_RenderFillRect(renderer, &canbarikonum);
                    }
                    else if(saglik < saglikOkcular*20/100)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                        canbarikonum.w = 10;
                        SDL_RenderFillRect(renderer, &canbarikonum);
                    }
                    else if(saglik < saglikOkcular*40/100)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
                        canbarikonum.w = 15;
                        SDL_RenderFillRect(renderer, &canbarikonum);
                    }
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &canbarikonum);
                }
                mevcutKare++;
            }
        }
        mevcutKarePiyade = 0;
        for (int row = 0; row < GRID_ROWS; row++)
        {
            for (int col = 0; col < GRID_COLS; col++)
            {
                int x = baslangicX + col * SQUARE_SIZE + 210;
                int y = baslangicY + row+3 * SQUARE_SIZE;
                if (piyadeInKare[mevcutKarePiyade] > 0)
                {
                    SDL_Rect piyadekonum = {x, y, SQUARE_SIZE, SQUARE_SIZE};
                    SDL_RenderCopy(renderer, piyadelerTexture, NULL, &piyadekonum);
                    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                    SDL_Color textrenk = {255, 255, 255};
                    SDL_Surface* textyuzeyismi = TTF_RenderText_Solid(font, "Piyadeler", textrenk);
                    if (textyuzeyismi)
                    {
                        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzeyismi);
                        SDL_FreeSurface(textyuzeyismi);

                        int textgenislik, textuzunluk;
                        SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                        SDL_Rect textkonum = {x, y + SQUARE_SIZE - 13, textgenislik, textuzunluk};

                        SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                        SDL_DestroyTexture(textTexture);
                    }
                    else
                    {
                        SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                    }
                    char text[10];
                    snprintf(text, sizeof(text), "%d", (int)ceil(piyadeInKare[mevcutKarePiyade]));
                    SDL_Surface* textyuzey = TTF_RenderText_Solid(font, text, textrenk);
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);

                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum = {x + (SQUARE_SIZE - textgenislik) / 2, y + SQUARE_SIZE, textgenislik, textuzunluk};

                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);

                    int canbarigenislik = 30;
                    SDL_Rect canbarikonum = {x + (SQUARE_SIZE - canbarigenislik) / 2, y, canbarigenislik, 5};
                    int saglik = piyadeInKare[mevcutKarePiyade]*100;
                    int carbanidoluluk = (saglik * canbarigenislik) / 100;
                    if(saglik > saglikPiyadeler*40/100)
                    {
                        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                        canbarikonum.w = 30;
                        SDL_RenderFillRect(renderer, &canbarikonum);
                    }
                    else if(saglik < saglikPiyadeler*20/100)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                        canbarikonum.w = 10;
                        SDL_RenderFillRect(renderer, &canbarikonum);
                    }
                    else if(saglik < saglikPiyadeler*40/100)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
                        canbarikonum.w = 15;
                        SDL_RenderFillRect(renderer, &canbarikonum);
                    }
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &canbarikonum);
                }
                mevcutKarePiyade++;
            }
        }
        mevcutKareSuvari = 0;
        for (int row = 0; row < GRID_ROWS; row++)
        {
            for (int col = 0; col < GRID_COLS; col++)
            {
                int x = baslangicX + col * SQUARE_SIZE+210;
                int y = baslangicY + row+6 * SQUARE_SIZE;
                if (suvariInKare[mevcutKareSuvari] > 0)
                {
                    SDL_Rect suvarikonum = {x, y, SQUARE_SIZE, SQUARE_SIZE};
                    SDL_RenderCopy(renderer, suvarilerTexture, NULL, &suvarikonum);
                    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 11);
                    SDL_Color textrenk = {255, 255, 255};
                    SDL_Surface* textyuzeyismi = TTF_RenderText_Solid(font, "Suvariler", textrenk);
                    if (textyuzeyismi)
                    {
                        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzeyismi);
                        SDL_FreeSurface(textyuzeyismi);
                        int textgenislik, textuzunluk;
                        SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                        SDL_Rect textkonum= {x, y + SQUARE_SIZE - 13, textgenislik, textuzunluk};
                        SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                        SDL_DestroyTexture(textTexture);
                    }
                    else
                    {
                        SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                    }
                    char text[10];
                    snprintf(text, sizeof(text), "%d", (int)ceil(suvariInKare[mevcutKareSuvari]));
                    SDL_Surface* textyuzey = TTF_RenderText_Solid(font, text, textrenk);
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);
                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum= {x + (SQUARE_SIZE - textgenislik) / 2, y + SQUARE_SIZE, textgenislik, textuzunluk};

                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);

                    int canbarigenislik = 30;
                    SDL_Rect canbarikonum = {x + (SQUARE_SIZE - canbarigenislik) / 2, y, canbarigenislik, 5};

                    int saglik = suvariInKare[mevcutKareSuvari]*120;
                    int canbaridoluluk = (saglik * canbarigenislik) / 100;
                    if(saglik > saglikSuvariler*40/100)
                    {
                        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                        canbarikonum.w = 30;
                        SDL_RenderFillRect(renderer, &canbarikonum);
                    }
                    else if(saglik < saglikSuvariler *20/100)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                        canbarikonum.w = 10;
                        SDL_RenderFillRect(renderer, &canbarikonum);
                    }
                    else if(saglik < saglikSuvariler*40/100)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
                        canbarikonum.w = 15;
                        SDL_RenderFillRect(renderer, &canbarikonum);
                    }
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &canbarikonum);
                }
                mevcutKareSuvari++;
            }
        }
        mevcutKareKusatma = 0;
        for (int row = 0; row < GRID_ROWS; row++)
        {
            for (int col = 0; col < GRID_COLS; col++)
            {
                int x = baslangicX + col * SQUARE_SIZE+210;
                int y = baslangicY + row+9 * SQUARE_SIZE;
                if (kusatmaInKare[mevcutKareKusatma] > 0)
                {
                    SDL_Rect kusatmakonum = {x, y, SQUARE_SIZE, SQUARE_SIZE};
                    SDL_RenderCopy(renderer, kusatmaMakinesiTexture, NULL, &kusatmakonum);
                    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                    SDL_Color textrenk = {255, 255, 255};
                    SDL_Surface* textyuzeyismi = TTF_RenderText_Solid(font, "Kusatma M.", textrenk);
                    if (textyuzeyismi)
                    {
                        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzeyismi);
                        SDL_FreeSurface(textyuzeyismi);
                        int textgenislik, textuzunluk;
                        SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                        SDL_Rect textRect = {x, y + SQUARE_SIZE - 13, textgenislik, textuzunluk};
                        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                        SDL_DestroyTexture(textTexture);
                    }
                    else
                    {
                        SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                    }
                    char text[10];
                    snprintf(text, sizeof(text), "%d", (int)ceil(kusatmaInKare[mevcutKareKusatma]));
                    SDL_Surface* textyuzey = TTF_RenderText_Solid(font, text, textrenk);
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);

                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum = {x + (SQUARE_SIZE - textgenislik) / 2, y + SQUARE_SIZE, textgenislik, textuzunluk};

                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);

                    int canbarigenislik = 30;
                    SDL_Rect canbarikonum = {x + (SQUARE_SIZE - canbarigenislik) / 2, y, canbarigenislik, 5};

                    int saglik =kusatmaInKare[mevcutKareKusatma]*150;
                    int canbaridoluluk = (saglik * canbarigenislik) / 100;

                    if(saglik > saglikKusatma*40/100)
                    {
                        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                        canbarikonum.w = 30; //
                        SDL_RenderFillRect(renderer, &canbarikonum);
                    }
                    else if(saglik < saglikKusatma *20/100)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                        canbarikonum.w = 10;
                        SDL_RenderFillRect(renderer, &canbarikonum);
                    }
                    else if(saglik < saglikKusatma*40/100)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
                        canbarikonum.w = 15;
                        SDL_RenderFillRect(renderer, &canbarikonum);
                    }
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &canbarikonum);
                }
                mevcutKareKusatma++;
            }
        }
        mevcutKareOrk = 0;
        for (int row = 0; row < GRID_ROWS; row++)
        {
            for (int col = 0; col < GRID_COLS; col++)
            {
                int x = baslangicX + col * SQUARE_SIZE+210;
                int y = baslangicY + row+11 * SQUARE_SIZE;
                if (orkInKare[mevcutKareOrk] > 0)
                {
                    SDL_Rect orkkonum = {x, y, SQUARE_SIZE, SQUARE_SIZE};
                    SDL_RenderCopy(renderer, orkDovuscuTexture, NULL, &orkkonum);

                    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                    SDL_Color textrenk = {255, 255, 255};
                    SDL_Surface* textyuzeyismi = TTF_RenderText_Solid(font, "Ork", textrenk);
                    if (textyuzeyismi)
                    {
                        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzeyismi);
                        SDL_FreeSurface(textyuzeyismi);
                        int textgenislik, textuzunluk;
                        SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                        SDL_Rect textkonum = {x, y + SQUARE_SIZE - 13, textgenislik, textuzunluk};

                        SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                        SDL_DestroyTexture(textTexture);
                    }
                    else
                    {
                        SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                    }
                    SDL_Color textrenk2 = {255, 255, 255};
                    SDL_Surface* textyuzeyismi2 = TTF_RenderText_Solid(font, "Dovusculeri", textrenk2);
                    if (textyuzeyismi2)
                    {
                        SDL_Texture* textTexture2 = SDL_CreateTextureFromSurface(renderer, textyuzeyismi2);
                        SDL_FreeSurface(textyuzeyismi2);

                        int textgenislik, textuzunluk;
                        SDL_QueryTexture(textTexture2, NULL, NULL, &textgenislik, &textuzunluk);
                        SDL_Rect textkonum = {x, y + SQUARE_SIZE -3, textgenislik, textuzunluk};
                        SDL_RenderCopy(renderer, textTexture2, NULL, &textkonum);
                        SDL_DestroyTexture(textTexture2);
                    }
                    else
                    {
                        SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                    }
                    char text[10];
                    snprintf(text, sizeof(text), "%d", (int)ceil(orkInKare[mevcutKareOrk]));
                    SDL_Surface* textyuzey = TTF_RenderText_Solid(font, text, textrenk);
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);
                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum = {x + (SQUARE_SIZE - textgenislik) / 2, y + SQUARE_SIZE+7, textgenislik, textuzunluk};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);

                    int canbarigenislik = 30;
                    SDL_Rect canbarikonum = {x + (SQUARE_SIZE - canbarigenislik) / 2, y, canbarigenislik, 5};

                    int saglik = orkInKare[mevcutKareOrk]*100;
                    int canbaridoluluk = (saglik * canbarigenislik) / 100;

                    if(saglik > saglikOrk*40/100)
                    {
                        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                        canbarikonum.w = 30;
                        SDL_RenderFillRect(renderer, &canbarikonum);
                    }
                    else if(saglik < saglikOrk *20/100)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                        canbarikonum.w = 10;
                        SDL_RenderFillRect(renderer, &canbarikonum);
                    }
                    else if(saglik < saglikOrk*40/100)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
                        canbarikonum.w = 15;
                        SDL_RenderFillRect(renderer, &canbarikonum);
                    }
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &canbarikonum);
                }
                mevcutKareOrk++;
            }
        }
        mevcutKareVarg = 0;
        for (int row = 0; row < GRID_ROWS; row++)
        {
            for (int col = 0; col < GRID_COLS; col++)
            {
                int x = baslangicX + col * SQUARE_SIZE+210;
                int y = baslangicY + row+13 * SQUARE_SIZE;
                if (vargInKare[mevcutKareVarg] > 0)
                {
                    SDL_Rect vargkonum = {x, y, SQUARE_SIZE, SQUARE_SIZE};
                    SDL_RenderCopy(renderer, vargBiniciTexture, NULL, &vargkonum);

                    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                    SDL_Color textrenk = {255, 255, 255};
                    SDL_Surface* textyuzeyismi= TTF_RenderText_Solid(font, "Varg", textrenk);
                    if (textyuzeyismi)
                    {
                        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzeyismi);
                        SDL_FreeSurface(textyuzeyismi);
                        int textgenislik, textuzunluk;
                        SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                        SDL_Rect textkonum = {x+9, y + SQUARE_SIZE - 13, textgenislik, textuzunluk};
                        SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                        SDL_DestroyTexture(textTexture);
                    }
                    else
                    {
                        SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                    }
                    SDL_Color textrenk2 = {255, 255, 255};
                    SDL_Surface* textyuzeyismi2 = TTF_RenderText_Solid(font, "Binicileri", textrenk2);
                    if (textyuzeyismi2)
                    {
                        SDL_Texture* textTexture2 = SDL_CreateTextureFromSurface(renderer, textyuzeyismi2);
                        SDL_FreeSurface(textyuzeyismi2);
                        int textgenislik, textuzunluk;
                        SDL_QueryTexture(textTexture2, NULL, NULL, &textgenislik, &textuzunluk);
                        SDL_Rect textkonum = {x+1, y + SQUARE_SIZE, textgenislik, textuzunluk};
                        SDL_RenderCopy(renderer, textTexture2, NULL, &textkonum);
                        SDL_DestroyTexture(textTexture2);
                    }
                    else
                    {
                        SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                    }
                    char text[10];
                    snprintf(text, sizeof(text), "%d", (int)ceil(vargInKare[mevcutKareVarg]));
                    SDL_Surface* textyuzey = TTF_RenderText_Solid(font, text, textrenk2);
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);

                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum = {x + (SQUARE_SIZE - textgenislik) / 2, y + SQUARE_SIZE+8, textgenislik, textuzunluk};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);

                    int canbarigenislik = 30;
                    SDL_Rect canbarikonum = {x + (SQUARE_SIZE - canbarigenislik) / 2, y, canbarigenislik, 5};

                    int saglik = vargInKare[mevcutKareVarg]*130;
                    int canbaridoluluk = (saglik * canbarigenislik) / 100;
                    if(saglik > saglikVarg*40/100)
                    {
                        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                        canbarikonum.w = 30;
                        SDL_RenderFillRect(renderer, &canbarikonum);
                    }
                    else if(saglik < saglikVarg *20/100)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                        canbarikonum.w = 10;
                        SDL_RenderFillRect(renderer, &canbarikonum);
                    }
                    else if(saglik < saglikVarg*40/100)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
                        canbarikonum.w = 15;
                        SDL_RenderFillRect(renderer, &canbarikonum);
                    }
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &canbarikonum);
                }
                mevcutKareVarg++;
            }
        }
        mevcutKareMizrakci = 0;
        for (int row = 0; row < GRID_ROWS; row++)
        {
            for (int col = 0; col < GRID_COLS; col++)
            {
                int x = baslangicX + col * SQUARE_SIZE+210;
                int y = baslangicY + row+15 * SQUARE_SIZE;
                if (mizrakInKare[mevcutKareMizrakci] > 0)
                {
                    SDL_Rect mizrakkonum = {x, y, SQUARE_SIZE, SQUARE_SIZE};
                    SDL_RenderCopy(renderer, mizrakciTexture, NULL, &mizrakkonum);

                    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                    SDL_Color textrenk = {255, 255, 255};
                    SDL_Surface* textyuzeyismi = TTF_RenderText_Solid(font, "Mizrakcilar", textrenk);
                    if (textyuzeyismi)
                    {
                        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzeyismi);
                        SDL_FreeSurface(textyuzeyismi);
                        int textgenislik, textuzunluk;
                        SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                        SDL_Rect textkonum = {x, y + SQUARE_SIZE - 13, textgenislik, textuzunluk};
                        SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                        SDL_DestroyTexture(textTexture);
                    }
                    else
                    {
                        SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                    }
                    char text[10];
                    snprintf(text, sizeof(text), "%d",(int)ceil(mizrakInKare[mevcutKareMizrakci]));
                    SDL_Surface* textyuzey = TTF_RenderText_Solid(font, text, textrenk);
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);

                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum = {x + (SQUARE_SIZE - textgenislik) / 2, y + SQUARE_SIZE, textgenislik, textuzunluk};

                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);

                    int canbarigenislik = 30;
                    SDL_Rect canbarikonum = {x + (SQUARE_SIZE - canbarigenislik) / 2, y, canbarigenislik, 5};
                    int saglik = mizrakInKare[mevcutKareMizrakci]*90;
                    int canbaridoluluk = (saglik * canbarigenislik) / 100;
                    if(saglik > saglikMizrakcilar*40/100)
                    {
                        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                        canbarikonum.w = 30;
                        SDL_RenderFillRect(renderer, &canbarikonum);
                    }
                    else if(saglik < saglikMizrakcilar *20/100)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                        canbarikonum.w = 10;
                        SDL_RenderFillRect(renderer, &canbarikonum);
                    }
                    else if(saglik < saglikMizrakcilar*40/100)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
                        canbarikonum.w = 15;
                        SDL_RenderFillRect(renderer, &canbarikonum);
                    }
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &canbarikonum);
                }
                mevcutKareMizrakci++;
            }
        }
        mevcutKareTroll = 0;
        for (int row = 0; row < GRID_ROWS; row++)
        {
            for (int col = 0; col < GRID_COLS; col++)
            {
                int x = baslangicX + col * SQUARE_SIZE+210;
                int y = baslangicY + row+17 * SQUARE_SIZE;
                if (trollInKare[mevcutKareTroll] > 0)
                {
                    SDL_Rect trollkonum = {x, y, SQUARE_SIZE, SQUARE_SIZE};
                    SDL_RenderCopy(renderer, trollerTexture, NULL, &trollkonum);

                    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 13);
                    SDL_Color textrenk = {255, 255, 255};
                    SDL_Surface* textyuzeyismi = TTF_RenderText_Solid(font, "Troller", textrenk);
                    if (textyuzeyismi)
                    {
                        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzeyismi);
                        SDL_FreeSurface(textyuzeyismi);
                        int textgenislik, textuzunluk;
                        SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                        SDL_Rect textkonum = {x, y + SQUARE_SIZE - 13, textgenislik, textuzunluk};
                        SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                        SDL_DestroyTexture(textTexture);
                    }
                    else
                    {
                        SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                    }
                    char text[10];
                    snprintf(text, sizeof(text), "%d", (int)ceil(trollInKare[mevcutKareTroll]));
                    SDL_Surface* textyuzey = TTF_RenderText_Solid(font, text, textrenk);
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);
                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum = {x + (SQUARE_SIZE - textgenislik) / 2, y + SQUARE_SIZE, textgenislik, textuzunluk};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);

                    int canbarigenislik = 30;
                    SDL_Rect canbarikonum = {x + (SQUARE_SIZE - canbarigenislik) / 2, y, canbarigenislik, 5};
                    int saglik = trollInKare[mevcutKareTroll]*200;
                    int canbaridoluluk = (saglik * canbarigenislik) / 100;
                    if(saglik >saglikTroller*40/100)
                    {
                        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                        canbarikonum.w = 30;
                        SDL_RenderFillRect(renderer, &canbarikonum);
                    }
                    else if(saglik < saglikTroller *20/100)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                        canbarikonum.w = 10;
                        SDL_RenderFillRect(renderer, &canbarikonum);
                    }
                    else if(saglik < saglikTroller*40/100)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
                        canbarikonum.w = 15;
                        SDL_RenderFillRect(renderer, &canbarikonum);
                    }
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderDrawRect(renderer, &canbarikonum);
                }
                mevcutKareTroll++;
            }
        }
        if (OrklegiToplamTemelSaglikGucu<=0)
        {
            int heroRow = 7;
            int heroCol = 2;
            int alparslanX = baslangicX + heroCol * SQUARE_SIZE;
            int alparslanY = baslangicY + heroRow * SQUARE_SIZE;
            SDL_Rect alparslankonum = {alparslanX, alparslanY, SQUARE_SIZE, SQUARE_SIZE};
            if (strcmp(kahramanismiinsan, "Alparslan") == 0)
            {
                SDL_RenderCopy(renderer, alparslanTexture, NULL, &alparslankonum);
                TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                SDL_Color kirmizirenk = {255, 0, 0, 255};
                SDL_Surface* textyuzey = TTF_RenderText_Solid(font, " Alparslan", textrenk);
                if (textyuzey)
                {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);
                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum = {alparslanX, alparslanY + SQUARE_SIZE, textgenislik, textuzunluk};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);
                }
                else
                {
                    SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                }
            }
            if (strcmp(CanavarismiInsan,"Ejderha")==0)
            {
                SDL_RenderCopy(renderer, ejderhaTexture, NULL, &ejderhakonum);
                TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                SDL_Color textrenk = {255, 255, 255};
                SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Ejderha", textrenk);
                if (textyuzey)
                {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);

                    int  textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum = {ejderhaX, ejderhaY + SQUARE_SIZE, textgenislik, textuzunluk};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);
                }
                else
                {
                    SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                }
            }
            if (strcmp(CanavarismiInsan,"Tepegoz")==0)
            {
                SDL_RenderCopy(renderer, tepegozTexture, NULL, &tepegozkonum);
                TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                SDL_Color textrenk = {255, 255, 255};
                SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Tepegoz", textrenk);
                if (textyuzey)
                {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);
                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum = {tepegozX, tepegozY + SQUARE_SIZE, textgenislik, textuzunluk};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);
                }
                else
                {
                    SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                }
            }
            if (strcmp(CanavarismiInsan,"Agri_Dagi_Devleri")==0)
            {
                SDL_RenderCopy(renderer, agriDagiDeviTexture, NULL, &agriDagikonum);
                TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                SDL_Color textrenk = {255, 255, 255};
                SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Agri Dagi Devi", textrenk);
                if (textyuzey)
                {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);
                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum = {agriDagiX, agriDagiY + SQUARE_SIZE, textgenislik, textuzunluk};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);
                }
                else
                {
                    SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                }
            }
            if (strcmp(CanavarismiInsan,"Karakurt")==0)
            {
                SDL_RenderCopy(renderer, karakurtTexture, NULL, &karakurtkonum);
                TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                SDL_Color textrenk = {255, 255, 255};
                SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Karakurt", textrenk);
                if (textyuzey)
                {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);
                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum = {karakurtX, karakurtY + SQUARE_SIZE, textgenislik, textuzunluk};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);
                }
                else
                {
                    SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                }
            }
            if (strcmp(CanavarismiInsan,"Samur")==0)
            {
                SDL_RenderCopy(renderer, samurTexture, NULL, &samurkonum);
                TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                SDL_Color textrenk = {255, 255, 255};
                SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Samur", textrenk);
                if (textyuzey)
                {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);
                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum = {samurX, samurY + SQUARE_SIZE, textgenislik, textuzunluk};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);
                }
                else
                {
                    SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                }
            }
        }
        if (InsanToplamTemelSaglikGucu<=0)
        {
            if (strcmp(CanavarismiOrk,"Golge_Kurtlari")==0)
            {
                SDL_RenderCopy(renderer, golgeKurtTexture, NULL, &GolgeKurtKonum);
                TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                SDL_Color textrenk = {255, 255, 255};
                SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Golge Kurtlari", textrenk);
                if (textyuzey)
                {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);
                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum = {golgeKurtX, golgeKurtY + SQUARE_SIZE, textgenislik, textuzunluk};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);
                }
                else
                {
                    SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                }
            }
            if (strcmp(CanavarismiOrk,"Buz_Devi")==0)
            {
                SDL_RenderCopy(renderer, buzDeviTexture, NULL, &buzDeviKonum);
                TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                SDL_Color textrenk = {255, 255, 255};
                SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Buz Devi", textrenk);
                if (textyuzey)
                {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);
                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum= {buzDeviX, buzDeviY + SQUARE_SIZE, textgenislik, textuzunluk};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);
                }
                else
                {
                    SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                }
            }
            if (strcmp(CanavarismiOrk,"Kara_Troll")==0)
            {
                SDL_RenderCopy(renderer, karaTrollTexture, NULL, &karaTrollKonum);
                TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                SDL_Color textrenk = {255, 255, 255};
                SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Kara Troll", textrenk);
                if (textyuzey)
                {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);
                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum= {karaTrollX, karaTrollY + SQUARE_SIZE, textgenislik, textuzunluk};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);
                }
                else
                {
                    SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                }
            }
            if (strcmp(CanavarismiOrk,"Camur_Devleri")==0)
            {
                SDL_RenderCopy(renderer, camurDevTexture, NULL, &camurDevKonum);
                TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                SDL_Color textrenk= {255, 255, 255};
                SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Camur Devi", textrenk);
                if (textyuzey)
                {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);
                    int  textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum= {camurDevX, camurDevY + SQUARE_SIZE, textgenislik, textuzunluk};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);
                }
                else
                {
                    SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                }
            }
            if (strcmp(CanavarismiOrk,"Ates_Iblisi")==0)
            {
                SDL_RenderCopy(renderer, atesIblisiTexture, NULL, &atesIblisKonum);
                TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                SDL_Color textrenk = {255, 255, 255};
                SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Ates Iblisi", textrenk);
                if (textyuzey)
                {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);
                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum = {atesIblisX, atesIblisY + SQUARE_SIZE, textgenislik, textuzunluk};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);
                }
                else
                {
                    SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                }
            }
            if (strcmp(CanavarismiOrk,"Makrog_Savas_Beyi")==0)
            {
                SDL_RenderCopy(renderer, makrogTexture, NULL, &makrogKonum);
                TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                SDL_Color textrenk = {255, 255, 255};
                SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Makrog Savas\nBeyi", textrenk);
                if (textyuzey)
                {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);
                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum= {makrogX, makrogY + SQUARE_SIZE, textgenislik, textuzunluk};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);
                }
                else
                {
                    SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                }
            }
        }
        if (OrklegiToplamTemelSaglikGucu<=0)
        {
            if (strcmp(kahramanismiinsan,"Alparslan")==0)
            {
                SDL_RenderCopy(renderer, alparslanTexture, NULL, &alparslanKonum);
                TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                SDL_Color textrenk = {255, 255, 255};
                SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Alparslan", textrenk);
                if (textyuzey)
                {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);
                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum= {alparslanX, alparslanY + SQUARE_SIZE, textgenislik, textuzunluk};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);
                }
                else
                {
                    SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                }
            }
            if (strcmp(kahramanismiinsan,"Yavuz_Sultan_Selim")==0)
            {
                SDL_RenderCopy(renderer, yavuzSultanTexture, NULL, &yavuzSultanKonum);
                TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                SDL_Color textrenk = {255, 255, 255};
                SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Yavuz Sultan Selim", textrenk);
                if (textyuzey)
                {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);
                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum = {yavuzSultanX, yavuzSultanY + SQUARE_SIZE, textgenislik, textuzunluk};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);
                }
                else
                {
                    SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                }
            }
            if (strcmp(kahramanismiinsan,"Fatih_Sultan_Mehmet")==0)
            {
                SDL_RenderCopy(renderer, fatihSultanTexture, NULL, &fatihSultanKonum);
                TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                SDL_Color textrenk= {255, 255, 255};
                SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Fatih Sultan Mehmet", textrenk);
                if (textyuzey)
                {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);
                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum = {fatihSultanX, fatihSultanY + SQUARE_SIZE, textgenislik, textuzunluk};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);
                }
                else
                {
                    SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                }
            }
            if (strcmp(kahramanismiinsan,"Mete_Han")==0)
            {
                SDL_RenderCopy(renderer, meteHanTexture, NULL, &metehanKonum);
                TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                SDL_Color textrenk = {255, 255, 255};
                SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "MeteHan", textrenk);
                if (textyuzey)
                {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);
                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum = {metehanX, metehanY + SQUARE_SIZE, textgenislik, textuzunluk};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);
                }
                else
                {
                    SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                }
            }
            if (strcmp(kahramanismiinsan,"Tugrul_Bey")==0)
            {
                SDL_RenderCopy(renderer, tugrulBeyTexture, NULL, &tugrulBeyKonum);
                TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                SDL_Color textrenk = {255, 255, 255};
                SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Tugrul Bey", textrenk);
                if (textyuzey)
                {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);
                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum = {tugrulBeyX, tugrulBeyY + SQUARE_SIZE, textgenislik, textuzunluk};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);
                }
                else
                {
                    SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                }
            }
        }
        if (InsanToplamTemelSaglikGucu<=0)
        {
            if (strcmp(kahramanismiOrk,"Vrog_Kafakiran")==0)
            {
                SDL_RenderCopy(renderer, vrogTexture, NULL, &vrogKonum);
                TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                SDL_Color textrenk = {255, 255, 255};
                SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Vrog Kafakiran", textrenk);
                if (textyuzey)
                {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);
                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum = {vrogX, vrogY + SQUARE_SIZE, textgenislik, textuzunluk};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);
                }
                else
                {
                    SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                }
            }
            if (strcmp(kahramanismiOrk,"Goruk_Vahsi")==0)
            {
                SDL_RenderCopy(renderer, gorukVahsiTexture, NULL, &gorukKonum);
                TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                SDL_Color textrenk= {255, 255, 255};
                SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Goruk Vahsi", textrenk);
                if (textyuzey)
                {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);
                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum = {gorukX, gorukY + SQUARE_SIZE, textgenislik, textuzunluk};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);
                }
                else
                {
                    SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                }
            }
            if (strcmp(kahramanismiOrk,"Thruk_Kemikkiran")==0)
            {
                SDL_RenderCopy(renderer, thrukTexture, NULL, &thrukKonum);
                TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                SDL_Color textrenk= {255, 255, 255};
                SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Thruk Kemikkiran", textrenk);
                if (textyuzey)
                {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);
                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum = {thrukX, thrukY + SQUARE_SIZE, textgenislik, textuzunluk};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);
                }
                else
                {
                    SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                }
            }
            if (strcmp(kahramanismiOrk,"Ugar_Zalim")==0)
            {
                SDL_RenderCopy(renderer, ugarTexture, NULL, &ugarKonum);
                TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/ariali.ttf", 10);
                SDL_Color textrenk= {255, 255, 255};
                SDL_Surface* textyuzey = TTF_RenderText_Solid(font, "Ugar Zalim", textrenk);
                if (textyuzey)
                {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textyuzey);
                    SDL_FreeSurface(textyuzey);
                    int textgenislik, textuzunluk;
                    SDL_QueryTexture(textTexture, NULL, NULL, &textgenislik, &textuzunluk);
                    SDL_Rect textkonum = {ugarX, ugarY + SQUARE_SIZE, textgenislik, textuzunluk};
                    SDL_RenderCopy(renderer, textTexture, NULL, &textkonum);
                    SDL_DestroyTexture(textTexture);
                }
                else
                {
                    SDL_Log("Yazi yuzeyi olusturulamadi: %s", TTF_GetError());
                }
            }
        }
        if (OrkKazandi == 1)
        {
            TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/framd.ttf", 30);
            if (font == NULL)
            {
                printf("Font yuklenemedi: %s\n", TTF_GetError());
                return 1;
            }
            SDL_Color renk = {255, 255, 255};
            SDL_Surface* metinYuzeyi = TTF_RenderText_Solid(font, "ORK LEGI OYUNU KAZANDI !", renk);
            if (metinYuzeyi == NULL)
            {
                printf("Metin yuzeyi oluşturulamadi: %s\n", TTF_GetError());
                TTF_CloseFont(font);
                return 1;
            }
            SDL_Texture* metinTeksturu = SDL_CreateTextureFromSurface(renderer, metinYuzeyi);
            if (metinTeksturu == NULL)
            {
                printf("Texture olusturulamadi: %s\n", SDL_GetError());
                SDL_FreeSurface(metinYuzeyi);
                TTF_CloseFont(font);
                return 1;
            }
            SDL_Rect metinDikdortgen;
            metinDikdortgen.x = 785;
            metinDikdortgen.y = 525;
            SDL_QueryTexture(metinTeksturu, NULL, NULL, &metinDikdortgen.w, &metinDikdortgen.h);


            SDL_RenderCopy(renderer, metinTeksturu, NULL, &metinDikdortgen);

            SDL_RenderPresent(renderer);

            SDL_FreeSurface(metinYuzeyi);
            SDL_DestroyTexture(metinTeksturu);
            TTF_CloseFont(font);
        }
        if (insanKazandi == 1)
        {
            TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/framd.ttf", 30);
            if (font == NULL)
            {
                printf("Font yuklenemedi: %s\n", TTF_GetError());
                return 1;
            }
            SDL_Color renk = {255, 255, 255};

            SDL_Surface* metinYuzeyi = TTF_RenderText_Solid(font, "INSAN IMPARATORLUGU OYUNU KAZANDI !", renk);
            if (metinYuzeyi == NULL)
            {
                printf("Metin yuzeyi olusturulamadi: %s\n", TTF_GetError());
                TTF_CloseFont(font);
                return 1;
            }
            SDL_Texture* metinTeksturu = SDL_CreateTextureFromSurface(renderer, metinYuzeyi);
            if (metinTeksturu == NULL)
            {
                printf("Texture olusturulamadi: %s\n", SDL_GetError());
                SDL_FreeSurface(metinYuzeyi);
                TTF_CloseFont(font);
                return 1;
            }

            SDL_Rect metinDikdortgen;
            metinDikdortgen.x = 715;
            metinDikdortgen.y = 545;
            SDL_QueryTexture(metinTeksturu, NULL, NULL, &metinDikdortgen.w, &metinDikdortgen.h);

            SDL_RenderCopy(renderer, metinTeksturu, NULL, &metinDikdortgen);

            SDL_RenderPresent(renderer);

            SDL_FreeSurface(metinYuzeyi);
            SDL_DestroyTexture(metinTeksturu);
            TTF_CloseFont(font);
        }
        SDL_Delay(10000) ;
        break ;
    }
    printf("SDL dongusu bitti, diger kodlar calismaya devam ediyor...\n");
    TTF_CloseFont(font);
    TTF_Quit();
SDL_DestroyTexture(arkaPlanTexture);
SDL_DestroyTexture(okcularTexture);
SDL_DestroyTexture(piyadelerTexture);
SDL_DestroyTexture(suvarilerTexture);
SDL_DestroyTexture(kusatmaMakinesiTexture);
SDL_DestroyTexture(orkDovuscuTexture);
SDL_DestroyTexture(vargBiniciTexture);
SDL_DestroyTexture(mizrakciTexture);
SDL_DestroyTexture(trollerTexture);
SDL_DestroyTexture(ejderhaTexture);
SDL_DestroyTexture(agriDagiDeviTexture);
SDL_DestroyTexture(tepegozTexture);
SDL_DestroyTexture(karakurtTexture);
SDL_DestroyTexture(samurTexture);
SDL_DestroyTexture(golgeKurtTexture);
SDL_DestroyTexture(buzDeviTexture);
SDL_DestroyTexture(karaTrollTexture);
SDL_DestroyTexture(camurDevTexture);
SDL_DestroyTexture(atesIblisiTexture);
SDL_DestroyTexture(makrogTexture);
SDL_DestroyTexture(alparslanTexture);
SDL_DestroyTexture(yavuzSultanTexture);
SDL_DestroyTexture(fatihSultanTexture);
SDL_DestroyTexture(meteHanTexture);
SDL_DestroyTexture(tugrulBeyTexture);
SDL_DestroyTexture(gorukVahsiTexture);
SDL_DestroyTexture(thrukTexture);
SDL_DestroyTexture(vrogTexture);
SDL_DestroyTexture(ugarTexture);
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(pencere);
SDL_Quit();
    free(unitJsonVerisi);
    free(HeroJsonVerisi);
    free(CreaturesJsonVerisi);
    free(ResearchJsonVerisi);
    free(SenaryoJsonVerisi);
    return 0;


}

