/* Akın Günaydın  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --- Dosya Isimleri Tanimlari ---
#define ARAC_DOSYASI "araclar.txt"
#define MUSTERI_DOSYASI "musteriler.txt"
#define KIRA_DOSYASI "kiralamalar.txt"

// --- Struct Tanimlari ---
typedef struct {
    int ID;
    char brand[50];
    char model[50];
    int year;
    int status; // 1: müsait, 0: kirada
} Car;

typedef struct {
    char ID[12]; // 11 haneli TC
    char name[100];
    char phone[15];
} Customer;

typedef struct {
    int carID;
    char customerID[12];
    char rentDate[11];
    char returnDate[11];
} Rental;

// --- Fonksiyon Prototipleri ---
void menu();
void clearBuffer();
int getSafeInt(char* prompt);
int isNumeric(char* str);
int isIDValid(char id[]);
void setupFiles();
int countFileLines(const char* filename);
int isCustomerRegistered(char id[]);

Car* loadCars(int* carCount);
void saveCars(Car* cars, int carCount);
void addCar(Car** cars, int* carCount);
void deleteCar(Car* cars, int* carCount);
void searchCar(Car* cars, int carCount);
void listCars(Car* cars, int carCount);

void addCustomer();
void listCustomers();
void searchCustomer();

void rentCar(Car* cars, int carCount);
void returnCar(Car* cars, int carCount);
void listRentals();

// --- Main Fonksiyonu ---
int main() {
    setupFiles();
    int carCount = 0, choice = -1;
    Car* cars = loadCars(&carCount); 

    do {
        menu();
        choice = getSafeInt("\nSeciminiz: "); 

        switch (choice) {
            case 1: addCar(&cars, &carCount); break;
            case 2: deleteCar(cars, &carCount); break;
            case 3: searchCar(cars, carCount); break;
            case 4: listCars(cars, carCount); break;
            case 5: addCustomer(); break;
            case 6: searchCustomer(); break;
            case 7: listCustomers(); break;
            case 8: rentCar(cars, carCount); break;
            case 9: returnCar(cars, carCount); break;
            case 10: listRentals(); break;
            case 0: printf("Programdan cikiliyor...\n"); break;
            default: printf("Hata: Gecersiz menü secenegi!\n");
        }
        if(choice != 0 && cars != NULL) saveCars(cars, carCount);
    } while (choice != 0);

    if(cars) free(cars);
    return 0;
}

// --- Guvenli Girdi ve Kontrol Fonksiyonlari ---

int getSafeInt(char* prompt) {
    int value;
    printf("%s", prompt);
    while (scanf("%d", &value) != 1) {
        printf("Hata: Lutfen sadece tam sayi giriniz: ");
        clearBuffer();
    }
    clearBuffer();
    return value;
}

int isNumeric(char* str) {
    for (int i = 0; i < (int)strlen(str); i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

int isIDValid(char id[]) {
    if (strlen(id) != 11 || !isNumeric(id)) return 0;
    return 1;
}

int isCustomerRegistered(char id[]) {
    FILE* fp = fopen(MUSTERI_DOSYASI, "r");
    if (!fp) return 0;
    char fid[15], name[100], ph[20];
    while (fscanf(fp, "%s %[^0-9] %s", fid, name, ph) != EOF) {
        if (strcmp(id, fid) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void clearBuffer() {
    int c; while ((c = getchar()) != '\n' && c != EOF);
}

// --- Islevsel Fonksiyonlar ---

void setupFiles() {
    FILE* f1 = fopen(ARAC_DOSYASI, "a"); if(f1) fclose(f1);
    FILE* f2 = fopen(MUSTERI_DOSYASI, "a"); if(f2) fclose(f2);
    FILE* f3 = fopen(KIRA_DOSYASI, "a"); if(f3) fclose(f3);
}

int countFileLines(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return 0;
    int count = 0; char buf[256];
    while (fgets(buf, sizeof(buf), fp)) {
        if(strlen(buf) > 1) count++;
    }
    fclose(fp);
    return count;
}

Car* loadCars(int* carCount) {
    *carCount = countFileLines(ARAC_DOSYASI);
    Car* arr = (Car*)malloc((*carCount + 1) * sizeof(Car));
    FILE* fp = fopen(ARAC_DOSYASI, "r");
    if (fp) {
        for (int i = 0; i < *carCount; i++)
            fscanf(fp, "%d %s %s %d %d", &arr[i].ID, arr[i].brand, arr[i].model, &arr[i].year, &arr[i].status);
        fclose(fp);
    }
    return arr;
}

void saveCars(Car* cars, int carCount) {
    FILE* fp = fopen(ARAC_DOSYASI, "w");
    if(!fp) return;
    for (int i = 0; i < carCount; i++)
        fprintf(fp, "%d %s %s %d %d\n", cars[i].ID, cars[i].brand, cars[i].model, cars[i].year, cars[i].status);
    fclose(fp);
}

void addCar(Car** cars, int* carCount) {
    *cars = realloc(*cars, (*carCount + 1) * sizeof(Car));
    (*cars)[*carCount].ID = getSafeInt("Arac ID: ");
    printf("Marka: "); scanf("%s", (*cars)[*carCount].brand);
    printf("Model: "); scanf("%s", (*cars)[*carCount].model);
    (*cars)[*carCount].year = getSafeInt("Uretim Yili: ");
    (*cars)[*carCount].status = 1;
    (*carCount)++;
    printf("Arac eklendi.\n");
}

void listCars(Car* cars, int carCount) {
    printf("\nID\tMarka\tModel\tYil\tDurum\n");
    printf("--------------------------------------------\n");
    for (int i = 0; i < carCount; i++)
        printf("%d\t%s\t%s\t%d\t%s\n", cars[i].ID, cars[i].brand, cars[i].model, cars[i].year, cars[i].status ? "Musait" : "Kirada");
}

void deleteCar(Car* cars, int* carCount) {
    int sid = getSafeInt("Silinecek Arac ID: ");
    int found = -1;
    for (int i = 0; i < *carCount; i++) {
        if (cars[i].ID == sid) { found = i; break; }
    }
    if (found != -1) {
        for (int i = found; i < *carCount - 1; i++) cars[i] = cars[i+1];
        (*carCount)--;
        printf("Arac silindi.\n");
    } else printf("ID bulunamadi.\n");
}

void searchCar(Car* cars, int carCount) {
    int sid = getSafeInt("Aranacak Arac ID: ");
    int found = 0;
    for (int i = 0; i < carCount; i++) {
        if (cars[i].ID == sid) {
            printf("Bulundu: %s %s (%d) - %s\n", cars[i].brand, cars[i].model, cars[i].year, cars[i].status ? "Musait" : "Kirada");
            found = 1; break;
        }
    }
    if (!found) printf("Arac bulunamadi.\n");
}

void addCustomer() {
    Customer c;
    printf("TC ID (11 hane): "); scanf("%s", c.ID);
    if (!isIDValid(c.ID)) {
        printf("Hata: Gecersiz TC!\n");
        return;
    }
    printf("Ad Soyad: "); scanf(" %[^\n]s", c.name);
    printf("Telefon: "); scanf("%s", c.phone);
    
    FILE* fp = fopen(MUSTERI_DOSYASI, "a"); 
    if (fp) {
        fprintf(fp, "%s %s %s\n", c.ID, c.name, c.phone);
        fclose(fp);
        printf("Musteri eklendi.\n");
    }
}

void listCustomers() {
    FILE* fp = fopen(MUSTERI_DOSYASI, "r");
    if (!fp) return;
    char ID[15], name[100], ph[20];
    printf("\n%-15s %-30s %-15s\n", "TC ID", "Ad Soyad", "Telefon");
    printf("------------------------------------------------------------\n");
    while (fscanf(fp, "%s %[^0-9] %s", ID, name, ph) != EOF) {
        printf("%-15s %-30s %-15s\n", ID, name, ph);
    }
    fclose(fp);
}

void searchCustomer() {
    char sid[15], id[15], name[100], ph[20];
    int found = 0;
    printf("Aranacak TC: "); scanf("%s", sid);
    FILE* fp = fopen(MUSTERI_DOSYASI, "r");
    if(!fp) return;
    while (fscanf(fp, "%s %[^0-9] %s", id, name, ph) != EOF) {
        if (strcmp(sid, id) == 0) {
            printf("Bulundu: %s - %s\n", name, ph);
            found = 1; break;
        }
    }
    if (!found) printf("Musteri bulunamadi.\n");
    fclose(fp);
}

void rentCar(Car* cars, int carCount) {
    char tid[15];
    printf("Musteri TC: "); scanf("%s", tid);
    if (!isCustomerRegistered(tid)) {
        printf("Hata: Kayitli olmayan musteri!\n");
        return;
    }
    int cid = getSafeInt("Kiralanacak Arac ID: ");
    for (int i = 0; i < carCount; i++) {
        if (cars[i].ID == cid && cars[i].status == 1) {
            FILE* fp = fopen(KIRA_DOSYASI, "a");
            if(fp) {
                fprintf(fp, "%d %s %s %s\n", cid, tid, "01-01-2026", "N/A");
                fclose(fp);
            }
            cars[i].status = 0;
            printf("Kiralama basarili.\n");
            return;
        }
    }
    printf("Hata: Arac bulunamadi veya kirada.\n");
}

void returnCar(Car* cars, int carCount) {
    int cid = getSafeInt("Teslim Alinacak Arac ID: ");
    for (int i = 0; i < carCount; i++) {
        if (cars[i].ID == cid && cars[i].status == 0) {
            cars[i].status = 1;
            printf("Arac teslim alindi.\n");
            return;
        }
    }
    printf("Hata: Bu ID ile kiralanmis bir arac bulunamadi.\n");
}

void listRentals() {
    FILE* fp = fopen(KIRA_DOSYASI, "r");
    if (!fp) return;
    int cid; char tid[15], d1[15], d2[15];
    printf("\nAracID\tMusteriTC\tAlis Tarihi\n");
    printf("--------------------------------------------\n");
    while (fscanf(fp, "%d %s %s %s", &cid, tid, d1, d2) != EOF)
        printf("%d\t%s\t%s\n", cid, tid, d1);
    fclose(fp);
}

void menu() {
    printf("\n--- ARAC KIRALAMA YONETIM SISTEMI ---");
    printf("\n1. Arac Ekle\n2. Arac Sil\n3. Arac Ara\n4. Arac Listele");
    printf("\n5. Musteri Ekle\n6. Musteri Ara\n7. Musteri Listele");
    printf("\n8. Arac Kirala\n9. Arac Teslim Al\n10. Kiralama Listesi\n0. Cikis");
}