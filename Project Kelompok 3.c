#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <conio.h>
#include <windows.h>
#include <MMsystem.h>

struct data1 {
    int noUrut;
    char idNo[18];
    char nama[30];
    int status; //1. Mahasiswa
                //2. Dosen/Pegawai
    char password[20];
    struct data1 *next;
};
typedef struct data1 *data_anggota;

struct data2 {
    char nip[18];
    char nama[30];
    char password[20];
    struct data2 *next;
};
typedef struct data2 *data_petugas;

struct buku {
    int idBuku;
    char judul[30];
    char penulis[30];
    char penerbit[30];
    int tahun;
    int stok;
    struct buku *next;
};
typedef struct buku *data_buku;

data_anggota head;
data_petugas headP;
data_buku headBuku;

void login_member();
void login_petugas();
void view_member();
void view_petugas();
void clear_memory();
int cari_buku(int kode);

void bersih()
{
    printf("\nTekan sembarang tombol untuk melanjutkan. ");
    printf("%c", getch());
    Beep(600, 200);
    system("cls");
}

void delay_print(char *str)
{
    for (char *p = str; *p; p++) {
        putchar(*p);
        fflush(stdout);
        usleep(50000);
    }
    putchar('\n');
}

data_anggota insert_member(char line[])
{
    char* token;
    token = strtok(line, ";");

    data_anggota p;
    p = (data_anggota)malloc(sizeof(struct data1));
    int fieldno = 0;

    while (token != NULL) {
        if (fieldno == 0)
            p->noUrut = atoi(token);
        else if(fieldno == 1) {
            strcpy(p->idNo, token);
            p->idNo[strcspn(p->idNo, "\n")] = 0;
        }
        else if (fieldno == 2) {
            strcpy(p->nama, token);
            p->nama[strcspn(p->nama, "\n")] = 0;
        }
        else if (fieldno == 3)
            p->status = atoi(token);
        else if (fieldno == 4) {
            strcpy(p->password, token);
            p->password[strcspn(p->password, "\n")] = 0;
        }
        fieldno++;
        token = strtok(NULL, ";");
    }
    p->next = NULL;

    data_anggota temp = head;
    if (head == NULL) {
        head = p;
    }
    else {
        while (temp->next != NULL)
            temp = temp->next;

        temp->next = p;
    }
}

void read_dataMember()
{
    FILE* file = fopen("dataMember.txt", "r");
    char baris[1024];

    while (fgets(baris, 1024, file) != NULL) {
        char* tmp = strdup(baris);
        insert_member(tmp);

        free(tmp);
    }
    fclose(file);
}

void write_dataMember(int noUrut, char idNo[18], char nama[30], int status, char password[20])
{
    FILE* file = fopen("dataMember.txt", "a");

    fprintf(file, "%d;%s;%s;%d;%s\n", noUrut, idNo, nama, status, password);
    fclose(file);
}

data_anggota insert_memberNode()
{
    data_anggota newMember = (data_anggota)malloc(sizeof(struct data1));
    data_anggota cursor = head;

    while (cursor->next!=NULL) {
        cursor = cursor->next;
    }
    newMember->noUrut = cursor->noUrut + 1;
Identitas:
    printf("|=======================|\n"
           "| LENGKAPI DATA ANGGOTA |\n"
           "|=======================|\n");
    fflush(stdin);
    printf("NIM/NIP : ");
    gets(newMember->idNo);
    while (cursor != NULL) {
        if (strcmp(cursor->idNo, newMember->idNo) == 0) {
            printf("NIM/NIP sudah digunakan!\n");
            bersih();
            goto Identitas;
        }
        cursor = cursor->next;
    }

    fflush(stdin);
    printf("Nama    : ");
    gets(newMember->nama);

Status:
    printf("Status Anggota\n");
    printf("1. Mahasiswa\n");
    printf("2. Dosen/Pegawai\n");
    printf("Status Anda : ");
    scanf("%d", &newMember->status);
    if (newMember->status < 1 || newMember->status > 2) {
        printf("Pilihan yang Anda masukkan salah.\n");
        goto Status;
    }

Password:
    fflush(stdin);
    printf("Password (minimal 4 karakter) : ");
    int i = 0;
    char ch;
    while ((ch = getch()) != 13) {
        newMember->password[i] = ch;
        i++;
        printf("*");
    }
    newMember->password[i] = '\0';
    printf("\n");

    if (strlen(newMember->password) < 4) {
        printf("Password minimal terdiri dari 4 karakter!\n\n");
        goto Password;
    }
    newMember->next = NULL;

    data_anggota temp;
    if (head == NULL) {
        head = newMember;
    }
    else {
        temp = head;
        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newMember;
    }
    write_dataMember(newMember->noUrut, newMember->idNo, newMember->nama, newMember->status, newMember->password);
    printf("\nPendaftaran akun berhasil!\n");
    return (head);
}

data_anggota masuk()
{
    char idNo[18];
    char password[20];
    int ulang;

Login:
    printf("|=======================|\n"
           "| MASUK KE AKUN ANGGOTA |\n"
           "|=======================|\n");
    fflush(stdin);
    printf("NIM/NIP  : ");
    gets(idNo);
    fflush(stdin);
    printf("Password : ");
    int i = 0;
    char ch;
    while ((ch = getch()) != 13) {
        password[i] = ch;
        i++;
        printf("*");
    }
    password[i] = '\0';
    fflush(stdin);
    printf("\n");

    data_anggota cursor = head;
    while (cursor != NULL) {
        if (strcmp(cursor->idNo, idNo) == 0 && strcmp(cursor->password, password) == 0) {
                printf("\nAnda berhasil masuk.\nSelamat datang %s!\n", cursor->nama);
                return cursor;
        }
        else if (strcmp(cursor->idNo, idNo) == 0 && strcmp(cursor->password, password) != 0) {
                printf("Password salah!\n\n");
                printf("Tekan 1 untuk mengulangi. ");
                scanf("%d", &ulang);
                if (ulang == 1) {
                    system("cls");
                    goto Login;
                }
                else {
                    bersih();
                    login_member();
                }
        }
        else
            cursor = cursor->next;
    }
    printf("\nNIM/NIP tidak ditemukan!\nSilakan daftar akun terlebih dahulu.\n");
    return NULL;
}

void login_member()
{
    int pilih;
    data_anggota user;

    printf("|=======================|\n"
           "| MASUK SEBAGAI ANGGOTA |\n"
           "|=======================|\n"
           "| 1. Masuk              |\n"
           "| 2. Daftar Akun        |\n"
           "| 3. Kembali            |\n"
           "|=======================|\n");
    printf("  Masukkan pilihan: ");
    scanf("%d", &pilih);
    Beep(600, 200);
    system("cls");

    switch (pilih)
    {
        case 1:
            user = masuk();
            if (user != NULL) {
                bersih();
                view_member(user);
            }
            else {
                bersih();
                login_member();
            }
            break;

        case 2:
            insert_memberNode();
            bersih();
            login_member();
            break;

        case 3:
            system("cls");
            break;
    }
}

data_anggota insert_petugas(char line[])
{
    char* token;
    token = strtok(line, ";");

    data_petugas p;
    p = (data_petugas)malloc(sizeof(struct data2));
    int fieldno = 0;

    while (token != NULL) {
        if(fieldno == 0) {
            strcpy(p->nip, token);
            p->nip[strcspn(p->nip, "\n")] = 0;
        }
        else if (fieldno == 1) {
            strcpy(p->nama, token);
            p->nama[strcspn(p->nama, "\n")] = 0;
        }
        else if (fieldno == 2) {
            strcpy(p->password, token);
            p->password[strcspn(p->password, "\n")] = 0;
        }
        fieldno++;
        token = strtok(NULL, ";");
    }
    p->next = NULL;

    data_petugas temp = headP;
    if (headP == NULL) {
        headP = p;
    }
    else {
        while (temp->next != NULL)
            temp = temp->next;

        temp->next = p;
    }
}

void read_dataPetugas()
{
    FILE* file = fopen("dataPetugas.txt", "r");
    char baris[1024];

    while (fgets(baris, 1024, file) != NULL) {
        char* tmp = strdup(baris);
        insert_petugas(tmp);

        free(tmp);
    }
    fclose(file);
}

void write_dataPetugas(char nip[18], char nama[30], char password[20])
{
    FILE* file = fopen("dataPetugas.txt", "a");

    fprintf(file, "%s;%s;%s\n", nip, nama, password);
    fclose(file);
}

data_petugas insert_petugasNode()
{
    data_petugas newPetugas = (data_petugas)malloc(sizeof(struct data2));

Identitas:
    printf("|=======================|\n"
           "| LENGKAPI DATA PETUGAS |\n"
           "|=======================|\n");
    fflush(stdin);
    printf("NIP   : ");
    gets(newPetugas->nip);

    data_petugas cursor = headP;
    while (cursor != NULL) {
        if (strcmp(cursor->nip, newPetugas->nip) == 0) {
            printf("NIP sudah digunakan!\n");
            bersih();
            goto Identitas;
        }
        cursor = cursor->next;
    }

    fflush(stdin);
    printf("Nama   : ");
    gets(newPetugas->nama);

Password:
    fflush(stdin);
    printf("Password (minimal 4 karakter) : ");
    int i = 0;
    char ch;
    while ((ch = getch()) != 13) {
        newPetugas->password[i] = ch;
        i++;
        printf("*");
    }
    newPetugas->password[i] = '\0';
    printf("\n");

    if (strlen(newPetugas->password) < 4) {
        printf("Password minimal terdiri dari 4 karakter!\n\n");
        goto Password;
    }
    newPetugas->next = NULL;

    data_petugas temp;
    if (headP == NULL) {
        headP = newPetugas;
    }
    else {
        temp = headP;
        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newPetugas;
    }

    write_dataPetugas(newPetugas->nip, newPetugas->nama, newPetugas->password);
    printf("\nPendaftaran akun berhasil!\n");
    return (headP);
}

data_petugas masuk_petugas()
{
    char nip[18];
    char password[20];
    int ulang;

Login:
    printf("|=======================|\n"
           "| MASUK KE AKUN PETUGAS |\n"
           "|=======================|\n");
    fflush(stdin);
    printf("NIP      : ");
    gets(nip);
    fflush(stdin);
    printf("Password : ");
    int i = 0;
    char ch;
    while ((ch = getch()) != 13) {
        password[i] = ch;
        i++;
        printf("*");
    }
    password[i] = '\0';
    fflush(stdin);
    printf("\n");

    data_petugas cursor = headP;
    while (cursor != NULL) {
        if (strcmp(cursor->nip, nip) == 0 && strcmp(cursor->password, password) == 0) {
                printf("\nAnda berhasil masuk.\nSelamat datang %s!\n", cursor->nama);
                bersih();
                return cursor;
        }
        else if (strcmp(cursor->nip, nip) == 0 && strcmp(cursor->password, password) != 0) {
                printf("Password salah!\n\n");
                printf("Tekan 1 untuk mengulangi. ");
                scanf("%d", &ulang);
                if (ulang == 1) {
                    system("cls");
                    goto Login;
                }
                else {
                    bersih();
                    login_petugas();
                }
        }
        else
            cursor = cursor->next;
    }
    printf("\nNIP tidak ditemukan!\nSilakan daftar akun terlebih dahulu.\n");
    return NULL;
}

void login_petugas()
{
    int pilih;
    data_petugas user;

    printf("|=======================|\n"
           "| MASUK SEBAGAI PETUGAS |\n"
           "|=======================|\n"
           "| 1. Masuk              |\n"
           "| 2. Daftar Akun        |\n"
           "| 3. Kembali            |\n"
           "|=======================|\n");
    printf("  Masukkan pilihan: ");
    scanf("%d", &pilih);
    Beep(600, 200);
    system("cls");

    switch (pilih)
    {
        case 1:
            user = masuk_petugas();
            if (user != NULL) {
                view_petugas(user);
            }
            else {
                bersih();
                login_petugas();
            }
            break;

        case 2:
            insert_petugasNode();
            bersih();
            login_petugas();
            break;

        case 3:
            system("cls");
            break;
    }
}

void swap_anggota (data_anggota head, data_anggota j)
{
    char tempAnggota[30];
    char tempId[18];
    int tempStatus;
    int tempNo;
    strcpy(tempAnggota, j->nama);
    strcpy(tempId, j->idNo);
    tempStatus = j->status;
    tempNo = j->noUrut;
    strcpy(j->nama, j->next->nama);
    strcpy(j->idNo, j->next->idNo);
    j->status = j->next->status;
    j->noUrut = j->next->noUrut;
    strcpy(j->next->nama, tempAnggota);
    strcpy (j->next->idNo, tempId);
    j->next->status = tempStatus;
    j->next->noUrut = tempNo;
}

void sort_anggota_nama (data_anggota head)
{
    data_anggota i= head, j=head;
    while(i!=NULL)
    {
        while (j->next != NULL)
        {
            if(strcasecmp(j->nama, j->next->nama)>0)
            {
                swap_anggota(head, j);
            }
            j=j->next;
        }
        j=head;
        i=i->next;
    }

}

void sort_anggota_no (data_anggota head)
{
    data_anggota i = head, j= head;
    while (i!=NULL){
        while(j->next!=NULL){
            if(j->noUrut > j->next->noUrut){
                swap_anggota(head, j);
            }
            j=j->next;
        }
        j=head;
        i=i->next;
    }
}

void tampil_member()
{
    int pilih, i;
    data_anggota cursor;

    system("cls");
    printf("Pilihan Tampilan\n");
    printf("1. Urutan Sesuai Nama\n");
    printf("2. Urutan Sesuai No Pendaftaran\n");
    printf("Pilihan Anda: ");
    scanf("%d", &pilih);

    system ("cls");
    printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 DAFTAR ANGGOTA \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\n");
    printf("|------+------------------+-------------+-------------------------------+-----------------|\n");
    printf("| %-4s | %-16s | %-11s | %-29s | %-15s |", " No", " No Pendaftaran", "  NIM/NIP", "             Nama", "    Status");
    printf("\n|------+------------------+-------------+-------------------------------+-----------------|\n");
    cursor = head;
    i=1;
    switch (pilih)
    {
        case 1:
            sort_anggota_nama(head);
            break;
        case 2:
            sort_anggota_no(head);
            break;
    }
         while (cursor != NULL) {
            printf("| %-4d | %-16d | %-11s | %-29s | ",i, cursor->noUrut, cursor->idNo, cursor->nama);
            if (cursor->status == 1)
                printf(" Mahasiswa      |\n");
            else
                printf(" Dosen/Pegawai  |\n");
            cursor = cursor->next;
            i++;
        }
}

data_buku insert_buku(char line[])
{
    char* token;
    token = strtok(line, ";");

    data_buku p;
    p = (data_buku)malloc(sizeof(struct buku));
    int fieldno = 0;

    while (token != NULL) {
        if(fieldno == 0)
            p->idBuku = atoi(token);
        else if (fieldno == 1) {
            strcpy(p->judul, token);
            p->judul[strcspn(p->judul, "\n")] = 0;
        }
        else if (fieldno == 2) {
            strcpy(p->penulis, token);
            p->penulis[strcspn(p->penulis, "\n")] = 0;
        }
        else if (fieldno == 3) {
            strcpy(p->penerbit, token);
            p->penerbit[strcspn(p->penerbit, "\n")] = 0;
        }
        else if (fieldno == 4)
            p->tahun = atoi(token);
        else if (fieldno == 5)
            p->stok = atoi(token);
        fieldno++;
        token = strtok(NULL, ";");
    }
    p->next = NULL;

    data_buku temp = headBuku;
    if (headBuku == NULL) {
        headBuku = p;
    }
    else {
        while (temp->next != NULL)
            temp = temp->next;

        temp->next = p;
    }
}

void read_dataBuku()
{
    FILE* file = fopen("dataBuku.txt", "r");
    char baris[1024];

    while (fgets(baris, 1024, file) != NULL) {
        char* tmp = strdup(baris);
        insert_buku(tmp);

        free(tmp);
    }
    fclose(file);
}

void write_dataBuku(int idBuku, char judul[30], char penulis[30], char penerbit[30], int tahun, int stok)
{
    FILE* file = fopen("dataBuku.txt", "a");

    fprintf(file, "%d;%s;%s;%s;%d;%d\n", idBuku, judul, penulis, penerbit, tahun, stok);
    fclose(file);
}

data_buku insert_bukuNode()
{
    data_buku newBuku = (data_buku)malloc(sizeof(struct buku));

    system("cls");
    printf("******** Tambahkan Data Buku ********");

    data_buku cursor = headBuku;
    while (cursor->next != NULL) {
        cursor = cursor->next;
    }
    newBuku->idBuku = cursor->idBuku + 1;
    printf("\nID Buku\t\t : %d", newBuku->idBuku);

    fflush(stdin);
    printf("\nJudul Buku\t : ");
    gets(newBuku->judul);
    fflush(stdin);
    printf("Nama Penulis\t : ");
    gets(newBuku->penulis);
    fflush(stdin);
    printf("Nama Penerbit\t : ");
    gets(newBuku->penerbit);
    printf("Tahun Terbit\t : ");
    scanf("%d", &newBuku->tahun);
    printf("Stok Buku\t : ");
    scanf("%d", &newBuku->stok);

    data_buku temp;
    if (headBuku == NULL) {
        headBuku = newBuku;
    }
    else {
        temp = headBuku;
        while(temp->next != NULL)
            temp = temp->next;

        temp->next = newBuku;
    }
    write_dataBuku(newBuku->idBuku, newBuku->judul, newBuku->penulis, newBuku->penerbit, newBuku->tahun, newBuku->stok);
    printf("\nBuku berhasil ditambahkan!\n");
    return(headBuku);
}

void delete_buku(int line)
{
    FILE* file = fopen("dataBuku.txt", "r");

    rewind(file);

    FILE *tempFile = fopen("temp-file.tmp", "w");
    char buffer[1024];
    int count = 1;
    while ((fgets(buffer, 1024, file)) != NULL) {
        if (line != count)
            fprintf(tempFile, buffer);

        count++;
    }

    fclose(tempFile);
    fclose(file);

    remove("dataBuku.txt");
    rename("temp-file.tmp", "dataBuku.txt");
}

void delete_buku_byid(int idBuku)
{
    int line = 1;
    data_buku cursor = headBuku;

    if (headBuku->idBuku == idBuku) {
        data_buku first = headBuku;
        headBuku = headBuku->next;
        first->next = NULL;
        free (first);
        delete_buku(1);
        printf("\nBuku dengan ID %d berhasil dihapus!\n", idBuku);
        bersih();
    }
    else {
        while (cursor != NULL) {
            if (cursor->next->idBuku == idBuku)
                break;
            cursor = cursor->next;
            line++;
        }
        line++;

        if (cursor != NULL) {
            data_buku temp = cursor->next;
            cursor->next = temp->next;
            temp->next = NULL;
            free(temp);
            delete_buku(line);
            printf("\nBuku dengan ID %d berhasil dihapus!", idBuku);
            bersih();
        }
    }
}

void update_buku(int line, struct buku Buku)
{
    FILE* file = fopen("dataBuku.txt", "r");

    rewind(file);

    FILE *tempFile = fopen("temp-file.tmp", "w");
    char buffer[1024];
    int count = 1;
    while ((fgets(buffer, 1024, file)) != NULL) {
        if (line == count)
            fprintf(tempFile, "%d;%s;%s;%s;%d;%d\n", Buku.idBuku, Buku.judul, Buku.penulis, Buku.penerbit, Buku.tahun, Buku.stok);
        else
            fprintf(tempFile, buffer);

        count++;
    }

    fclose(tempFile);
    fclose(file);

    remove("dataBuku.txt");
    rename("temp-file.tmp", "dataBuku.txt");
}

void update_buku_byid (struct buku Buku)
{
    int line = 1;
    data_buku cursor = headBuku;
    while (cursor != NULL && cursor->idBuku != Buku.idBuku) {
        line++;
        cursor = cursor->next;
    }

    cursor->idBuku = Buku.idBuku;
    strcpy(cursor->judul, Buku.judul);
    strcpy(cursor->penulis, Buku.penulis);
    strcpy(cursor->penerbit, Buku.penerbit);
    cursor->tahun = Buku.tahun;
    cursor->stok = Buku.stok;

    update_buku(line, Buku);
}

void update_stok_buku(data_buku buku)
{
    struct buku Buku;
    Buku.idBuku = buku->idBuku;
    strcpy(Buku.judul, buku->judul);
    strcpy(Buku.penulis, buku->penulis);
    strcpy(Buku.penerbit, buku->penerbit);
    Buku.tahun = buku->tahun;
    Buku.stok = buku->stok;
    update_buku_byid(Buku);
}

void input_edit_buku()
{
    struct buku edit;
    int idEdit, pilih;
    char judul[30];
    char penulis[30];
    char penerbit[30];
    int tahun;
    int stok;

    system("cls");
    printf("ID Buku yang ingin diedit: ");
    scanf("%d", &idEdit);
    data_buku cursor = headBuku;
    while (cursor != NULL && cursor->idBuku <= idEdit) {
        if (cursor->idBuku == idEdit)
            break;
        else
            cursor = cursor->next;
    }
    if (cursor == NULL || cursor->idBuku > idEdit) {
        printf("\nBuku tidak ditemukan!\n");
    }
    else {
        printf("Informasi Buku dengan ID %d\n"
               "Judul Buku    : %s\n"
               "Nama Penulis  : %s\n"
               "Nama Penerbit : %s\n"
               "Tahun Terbit  : %d\n"
               "Stok Buku     : %d\n\n", cursor->idBuku, cursor->judul, cursor->penulis, cursor->penerbit, cursor->tahun, cursor->stok);

        printf("Informasi apakah yang ingin Anda edit?\n"
               "1. Judul Buku\n"
               "2. Nama Penulis\n"
               "3. Nama Penerbit\n"
               "4. Tahun Terbit\n"
               "5. Stok Buku\n");
        printf("Pilihan Anda: ");
        scanf("%d", &pilih);
        printf("\n");

        switch (pilih)
        {
        case 1:
            fflush(stdin);
            printf("Judul Buku Perbaikan: ");
            gets(judul);
            edit.idBuku = cursor->idBuku;
            strcpy(edit.judul, judul);
            strcpy(edit.penulis, cursor->penulis);
            strcpy(edit.penerbit, cursor->penerbit);
            edit.tahun = cursor->tahun;
            edit.stok = cursor->stok;
            update_buku_byid(edit);
            break;
        case 2:
            fflush(stdin);
            printf("Perbaikan Nama Penulis: ");
            gets(penulis);
            edit.idBuku = cursor->idBuku;
            strcpy(edit.judul, cursor->judul);
            strcpy(edit.penulis, penulis);
            strcpy(edit.penerbit, cursor->penerbit);
            edit.tahun = cursor->tahun;
            edit.stok = cursor->stok;
            update_buku_byid(edit);
            break;
        case 3:
            fflush(stdin);
            printf("Perbaikan Nama Penerbit: ");
            gets(penerbit);
            edit.idBuku = cursor->idBuku;
            strcpy(edit.judul, cursor->judul);
            strcpy(edit.penulis, cursor->penulis);
            strcpy(edit.penerbit, penerbit);
            edit.tahun = cursor->tahun;
            edit.stok = cursor->stok;
            update_buku_byid(edit);
            break;
        case 4:
            printf("Perbaikan Tahun Terbit: ");
            scanf("%d",&tahun);
            edit.idBuku = cursor->idBuku;
            strcpy(edit.judul, cursor->judul);
            strcpy(edit.penulis, cursor->penulis);
            strcpy(edit.penerbit, cursor->penerbit);
            edit.tahun = tahun;
            edit.stok = cursor->stok;
            update_buku_byid(edit);
            break;
        case 5:
            printf("Perbaikan Stok Buku: ");
            scanf("%d",&stok);
            edit.idBuku = cursor->idBuku;
            strcpy(edit.judul, cursor->judul);
            strcpy(edit.penulis, cursor->penulis);
            strcpy(edit.penerbit, cursor->penerbit);
            edit.tahun = cursor->tahun;
            edit.stok = stok;
            update_buku_byid(edit);
            break;
        }
      	printf("\nBuku berhasil diedit!\n");
    }
}

struct data_pinjam {
    char idNo[18];
    int idBuku;
    int flag;   //0. Belum ada data peminjaman
                //1. Buku sedang dipinjam
                //2. Buku sudah dikembalikan
};
typedef struct data_pinjam pinjam;

pinjam list_pinjam[100];

void init_pinjam()
{
    for (int i = 0; i < 100; i++)
        list_pinjam[i].flag = 0;
}

void insert_peminjaman(char line[], int recno)
{
    char* token;
    token = strtok(line, ";");

    int fieldno = 0;
    while( token != NULL ) {
        if(fieldno == 0) {
            strcpy(list_pinjam[recno].idNo, token);
            list_pinjam[recno].idNo[strcspn(list_pinjam[recno].idNo, "\n")] = 0;
        }
        else if(fieldno == 1)
            list_pinjam[recno].idBuku = atoi(token);
        else if(fieldno == 2)
            list_pinjam[recno].flag = atoi(token);
        fieldno++;
        token = strtok(NULL, ";");
    }
}

void read_dataPeminjaman()
{
    FILE* file = fopen("dataPeminjaman.txt", "r");
    char baris[1024];
    int recno = 0;

    while (fgets(baris, 1024, file) != NULL) {
        char* tmp = strdup(baris);
        insert_peminjaman(tmp, recno);

        free(tmp);
        recno++;
    }
    fclose(file);
}

void write_dataPeminjaman(char idNo[18], int idBuku, int flag)
{
    FILE* file = fopen("dataPeminjaman.txt", "a");

    fprintf(file, "%s;%d;%d\n", idNo, idBuku, flag);
    fclose(file);
}

void update_peminjaman(int line, struct data_pinjam pinjam)
{
    FILE* file = fopen("dataPeminjaman.txt", "r");

    rewind(file);

    FILE *tempFile = fopen("temp-file.tmp", "w");
    char buffer[1024];
    int count = 1;
    while ((fgets(buffer, 1024, file)) != NULL)
    {
        if (line == count)
            fprintf(tempFile, "%s;%d;%d\n", pinjam.idNo, pinjam.idBuku, pinjam.flag);
        else
            fprintf(tempFile, buffer);

        count++;
    }

    fclose(tempFile);
    fclose(file);

    remove("dataPeminjaman.txt");
    rename("temp-file.tmp", "dataPeminjaman.txt");
}

data_buku peminjaman(int buku, char id[])
{
    data_buku cursor = headBuku;
    while (cursor != NULL) {
        if (cursor->idBuku == buku)
            break;
        else
            cursor = cursor->next;
    }

    if (cursor == NULL) {
        printf("\nMaaf ID Buku yang Anda masukkan tidak valid.\n");
        return 0;
    }
    else {
        if (cursor->stok == 0) {
            printf("\nMaaf, buku tidak bisa dipinjam karena stok buku sedang kosong.\n");
            return 0;
        }
        else {
            for (int i = 0; i < 100; i++) {
                if (list_pinjam [i].flag == 0) {
                    list_pinjam[i].idBuku = buku;
                    strcpy(list_pinjam[i].idNo, id);
                    list_pinjam[i].flag = 1;
                    write_dataPeminjaman(id, buku, list_pinjam[i].flag);
                    cursor->stok--;
                    update_stok_buku(cursor);
                    delay_print("\n============ Berhasil Meminjam Buku ============\n");
                    printf("ID Buku    : %d \n", cursor->idBuku);
                    printf("Judul Buku : %s \n", cursor->judul);
                    printf("Dengan Waktu Peminjaman Maksimal 7 Hari\n");
                    break;
                }
            }
        }
    }
}

void denda(int hari)
{
    int duit = 0;

    if (hari <= 7) {
        duit = 0;
        printf("Anda tidak dikenai denda.\n");
    }
    else {
        hari = hari - 7;
        duit = hari * 1000;
        printf("Anda dikenai denda sebesar Rp%d.\n", duit);
    }
}

data_buku pengembalian(int i, int buku, char id[])
{
    int hari;
    data_buku cursor= headBuku;
    while (cursor != NULL) {
        if (cursor->idBuku == buku)
            break;
        else
            cursor = cursor->next;
    }

    cursor->stok++;
    printf("Berapa lama Anda meminjam buku (hari)? ");
    scanf("%d", &hari);
    denda(hari);
    list_pinjam[i].flag = 2;
    update_stok_buku(cursor);
    struct data_pinjam pinjam;
    strcpy(pinjam.idNo, list_pinjam[i].idNo);
    pinjam.idBuku = list_pinjam[i].idBuku;
    pinjam.flag = list_pinjam[i].flag;
    update_peminjaman(i+1,pinjam);
    delay_print("\n============ Berhasil Mengembalikan Buku ============");
}

int cari_buku(int kode)
{
    data_buku cursor = headBuku;
    while (cursor != NULL && cursor->idBuku <= kode) {
        if (cursor->idBuku == kode)
            return 1;
        else
            cursor = cursor->next;
    }
    return 0;
}

void cari_IdBuku()
{
    int id;
    printf("Masukkan ID Buku yang dicari: ");
    scanf("%d", &id);
    data_buku cursor = headBuku;
    while (cursor != NULL && cursor->idBuku <= id) {
        if (cursor->idBuku == id)
        {
            system("cls");
            printf("ID Buku ditemukan.");
            printf("\n\nRincian Buku\n");
            printf("ID Buku      : %d\n", cursor->idBuku);
            printf("Judul Buku   : %s\n", cursor->judul);
            printf("Nama Penulis : %s\n", cursor->penulis);
            printf("Penerbit     : %s\n", cursor->penerbit);
            printf("Tahun Terbit : %d\n", cursor->tahun);
            printf("Stok buku    : %d\n", cursor->stok);
            return;
        }
        else
            cursor = cursor->next;
    }
    system("cls");
    printf("ID buku yang Anda cari tidak ada.\n");
}

void cari_judulBuku()
{
    char judul[30];
    fflush(stdin);
    printf("Masukkan Judul Buku yang dicari: ");
    gets(judul);
    data_buku cursor = headBuku;
    while (cursor != NULL) {
        if (strcasecmp(cursor->judul, judul)==0){
            system("cls");
            printf("Judul buku ditemukan.");
            printf("\n\nRincian Buku\n");
            printf("ID Buku      : %d\n", cursor->idBuku);
            printf("Judul Buku   : %s\n", cursor->judul);
            printf("Nama Penulis : %s\n", cursor->penulis);
            printf("Penerbit     : %s\n", cursor->penerbit);
            printf("Tahun Terbit : %d\n", cursor->tahun);
            printf("Stok buku    : %d\n", cursor->stok);
            return;
        }
        else
            cursor = cursor->next;
    }
    system("cls");
    printf("Judul buku yang Anda cari tidak ada.\n");
}

void search_buku()
{
    system("cls");
    printf("\n\n\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 CARI BUKU \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
    int pilih;
    printf("\n\nPilihan pencarian berdasarkan\n1. Id Buku\n2. Judul buku\n");
    printf("Pilihan Anda: ");scanf("%d", &pilih);
    if(pilih == 1)
    {
        cari_IdBuku();
    }
    else if(pilih == 2)
    {
        cari_judulBuku();
    }
}

void swap_buku(data_buku headBuku, data_buku j)
{
    char tempJudul[30];
    char tempPenulis[30];
    char tempPenerbit[30];
    int tempTahun;
    int tempStok;
    int tempID;
    strcpy(tempJudul, j->judul);
    strcpy(tempPenulis, j->penulis);
    strcpy(tempPenerbit, j->penerbit);
    tempTahun = j->tahun;
    tempStok = j->stok;
    tempID = j->idBuku;
    strcpy(j->judul, j->next->judul);
    strcpy(j->penulis, j->next->penulis);
    strcpy(j->penerbit, j->next->penerbit);
    j->tahun = j->next->tahun;
    j->stok = j->next->stok;
    j->idBuku = j->next->idBuku;
    strcpy(j->next->judul, tempJudul);
    strcpy(j->next->penulis, tempPenulis);
    strcpy(j->next->penerbit, tempPenerbit);
    j->next->tahun = tempTahun;
    j->next->stok = tempStok;
    j->next->idBuku = tempID;
}

void sort_judulAsc(data_buku head){
    data_buku i= head ,j=head;
    while(i!=NULL){
        while(j->next!=NULL){
            if(strcasecmp(j->judul,j->next->judul)>0){
              swap_buku(head, j);
            }
            j=j->next;
        }
        j=head;
        i=i->next;
    }
}

void sort_penulisAsc(data_buku headBuku){
    data_buku head = headBuku;
    data_buku i= head ,j=head;
    while(i!=NULL){
        while(j->next!=NULL){
            if(strcasecmp(j->penulis,j->next->penulis)>0){
                swap_buku(head, j);
            }
            j=j->next;
        }
        j=head;
        i=i->next;
    }
}

void sort_judulDesc(data_buku headBuku){
    data_buku head = headBuku;
    data_buku i= head ,j=head;
    while(i!=NULL){
        while(j->next!=NULL){
            if(strcasecmp(j->judul,j->next->judul)<0){
                swap_buku(head, j);
            }
            j=j->next;
        }
        j=head;
        i=i->next;
    }
}

void sort_penulisDesc(data_buku headBuku){
    data_buku head = headBuku;
    data_buku i= head ,j=head;
    while(i!=NULL){
        while(j->next!=NULL){
            if(strcasecmp(j->penulis,j->next->penulis)<0){
                swap_buku(head,j);
            }
            j=j->next;
        }
        j=head;
        i=i->next;
    }
}

void sort_IdDesc(data_buku headBuku){
    data_buku head = headBuku;
    data_buku i= head ,j=head;
    while(i!=NULL){
        while(j->next!=NULL){
            if(j->idBuku < j->next->idBuku){
                swap_buku(head, j);
            }
            j=j->next;
        }
        j=head;
        i=i->next;
    }
}

void default_buku(data_buku headBuku){
    data_buku head = headBuku;
    data_buku i= head ,j=head;
    while(i!=NULL){
        while(j->next!=NULL){
            if(j->idBuku > j->next->idBuku){
                swap_buku(head, j);
            }
            j=j->next;
        }
        j=head;
        i=i->next;
    }
}

void bukuAsc(data_buku head, int pilih)
{
    if (pilih == 1){
        return;
    }
    else if(pilih == 2){
        sort_judulAsc(headBuku);
    }
    else if(pilih == 3){
        sort_penulisAsc(headBuku);
    }
}

void bukuDesc(data_buku head, int pilih)
{
    if (pilih == 1){
        sort_IdDesc(headBuku);
    }
    else if(pilih == 2){
        sort_judulDesc(headBuku);
    }
    else if(pilih == 3){
        sort_penulisDesc(headBuku);
    }
}

void tampil_buku()
{
    int pilih;
    system("cls");
    printf("Pilihan pengurutan:\n");
    printf("1. Naik\n");
    printf("2. Turun\n");
    printf("Pilihan Anda: ");
    scanf("%d", &pilih);
    data_buku cursor = headBuku;
    int pilih2;
    printf("\nTampilkan berdasarkan:\n");
    printf("1. ID Buku\n");
    printf("2. Judul Buku\n");
    printf("3. Nama Penulis\n");
    printf("Pilihan Anda: ");
    scanf("%d", &pilih2);
    system("cls");
    printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 DAFTAR BUKU \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
    printf("\n\n|---------+--------------------------------+----------------------+-----------------+--------------+-----------|\n");
    printf("| %-7s | %-30s | %-20s | %-15s | %s | %s |", "ID BUKU", "          JUDUL BUKU", "      PENULIS", "    PENERBIT", "TAHUN TERBIT", "STOK BUKU");
    printf("\n|---------+--------------------------------+----------------------+-----------------+--------------+-----------|");
    if (pilih == 1) {
        bukuAsc(headBuku, pilih2);
    }
    else if(pilih == 2){
        bukuDesc(headBuku, pilih2);
    }
    if (cursor == NULL){
        return;
    }
    while(cursor!=NULL) {
        printf("\n|    %-4d | %-30s | %-20s | %-15s |     %-8d |     %-5d |", cursor->idBuku, cursor->judul,cursor->penulis, cursor->penerbit, cursor->tahun, cursor->stok);
        cursor=cursor->next;
    }
    default_buku(headBuku);
}

const char* copy_judul_buku(int idBuku)
{
    data_buku cursor = headBuku;
    while (cursor != NULL) {
        if (cursor->idBuku == idBuku) {
            return cursor->judul;
            break;
        }
        else
            cursor = cursor->next;
    }
}

void riwayat_peminjaman(data_anggota user)
{
    system("cls");
    printf("Profil Anda\n");
    printf("NIP/NIM  : %s\n"
           "Nama     : %s\n\n", user->idNo, user->nama);
    printf("             Riwayat Peminjaman Buku\n");
    printf("|-----+--------------------------+--------------------|\n");
    printf("| %-2s | %-24s | %-18s |", " No", "       Judul Buku", "      Status");
    printf("\n|-----+--------------------------+--------------------|\n");
    int no = 0;
    for (int i = 0; i < 100; i++) {
        if (strcmp(list_pinjam[i].idNo, user->idNo) == 0) {
            no++;
            printf("| %-3d | %-24s | ", no, copy_judul_buku(list_pinjam[i].idBuku));
            if (list_pinjam[i].flag == 1)
                printf(" Sedang Dipinjam   |\n");
            else
                printf("Sudah Dikembalikan |\n");
        }
    }
}

void tampil_peminjaman()
{
    system("cls");
    printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 Daftar Peminjaman Buku \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\n");
    printf("|------+--------------------------+--------------------+---------------------|\n");
    printf("| %-4s | %-24s | %-18s | %-19s |", " No", "       Judul Buku", " NIP/NIM Peminjam", " Status Peminjaman");
    printf("\n|------+--------------------------+--------------------+---------------------|\n");
    int i = 0;
    while (list_pinjam[i].flag != 0) {
        printf("| %-4d | %-24s | %-18s | ", i+1, copy_judul_buku(list_pinjam[i].idBuku), list_pinjam[i].idNo);
        if (list_pinjam[i].flag == 1)
            printf("Belum Dikembalikan  |\n");
        else
            printf("Sudah Dikembalikan  |\n");
        i++;
    }
}

void view_member(data_anggota user)
{
    int pilih;

Menu:
    do
    {
        system("cls");
        printf(" \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 DAFTAR MENU ANGGOTA \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\n\n"
               " \xDB\xDB\xDB\xDB\xB2 1. Tampilkan Daftar Buku      \n\n"
               " \xDB\xDB\xDB\xDB\xB2 2. Cari Buku                  \n\n"
               " \xDB\xDB\xDB\xDB\xB2 3. Peminjaman Buku            \n\n"
               " \xDB\xDB\xDB\xDB\xB2 4. Pengembalian Buku          \n\n"
               " \xDB\xDB\xDB\xDB\xB2 5. Riwayat Peminjaman Buku    \n\n"
               " \xDB\xDB\xDB\xDB\xB2 6. Keluar                     \n\n"
               " \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\n");
        printf(" Pilihan Anda: ");
        scanf("%d", &pilih);
        Beep(600, 200);

        switch (pilih)
        {
            case 1:
                tampil_buku();
                printf("\n");
                bersih();
                break;

            case 2:
                search_buku();
            	bersih();
                break;

            case 3:
                system("cls");
                printf("ID Buku yang ingin Anda pinjam: ");
                int idBuku;
                scanf("%d", &idBuku);
                printf("\nIdentitas Anda\n");
                printf("NIM/NIP    : %s\n", user->idNo);
                printf("Nama       : %s\n", user->nama);
                peminjaman(idBuku, user->idNo);
                bersih();
                break;

            case 4:
                system("cls");
                printf("ID Buku yang ingin Anda kembalikan : ");
                scanf("%d", &idBuku);
                int i = 0;
                while (i < 100) {
                    if (strcmp(list_pinjam[i].idNo, user->idNo) == 0 && list_pinjam[i].idBuku == idBuku && list_pinjam[i].flag == 1) {
                        pengembalian(i, idBuku, user->idNo);
                        break;
                    }
                    else if (i == 99 && list_pinjam[i].idNo != user->idNo && list_pinjam[i].idBuku != idBuku && list_pinjam[i].flag != 1) {
                        printf("\nAnda tidak meminjam buku tersebut.\n");
                        break;
                    }
                    i++;
                }
                bersih();
                break;

            case 5:
                riwayat_peminjaman(user);
                bersih();
                break;

            case 6:
                return;

            default:
                printf("Pilihan yang Anda masukkan salah.\n");
                break;
                goto Menu;
        }
    } while (1);
}

void view_petugas(data_petugas user)
{
    int pilih, hapus, satu;

Menu:
    do {
        system("cls");
        printf(" \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 DAFTAR MENU PETUGAS \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\n\n"
               " \xDB\xDB\xDB\xB2 1. Tampilkan Daftar Buku       \n\n"
               " \xDB\xDB\xDB\xB2 2. Tambah Buku                 \n\n"
               " \xDB\xDB\xDB\xB2 3. Edit Buku                   \n\n"
               " \xDB\xDB\xDB\xB2 4. Hapus Buku                  \n\n"
               " \xDB\xDB\xDB\xB2 5. Tampilkan Daftar Anggota    \n\n"
               " \xDB\xDB\xDB\xB2 6. Tampilkan Daftar Peminjaman \n\n"
               " \xDB\xDB\xDB\xB2 7. Lihat Profil Anda           \n\n"
               " \xDB\xDB\xDB\xB2 8. Keluar                      \n\n"
               " \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\n");
        printf(" Pilihan Anda: ");
        scanf("%d", &pilih);
        Beep(600, 200);

        switch (pilih)
        {
            case 1:
                tampil_buku();
                printf("\n");
                bersih();
                break;

            case 2:
                insert_bukuNode();
                bersih();
                break;

            case 3:
                input_edit_buku();
                bersih();
                break;

            case 4:
                system("cls");
                printf("ID Buku yang ingin dihapus: ");
                scanf("%d", &hapus);
                int cari = cari_buku(hapus);
                if (cari == 1) {
                    printf("\nApakah Anda yakin ingin menghapus buku dengan ID %d?", hapus);
                    printf("\nTekan 1 untuk menghapus. ");
                    scanf("%d", &satu);
                    if (satu == 1)
                        delete_buku_byid(hapus);
                    else {
                        bersih();
                        view_petugas(user);
                    }
                }
                else {
                    printf("\nBuku tidak ditemukan!\n");
                    bersih();
                    view_petugas(user);
                }
                break;

            case 5:
                tampil_member();
                bersih();
                break;

            case 6:
                tampil_peminjaman();
                bersih();
                break;

            case 7:
                system("cls");
                printf("Profil Anda\n\n");
                printf("NIP  : %s\n", user->nip);
                printf("Nama : %s\n", user->nama);
                bersih();
                break;

            case 8:
                return;

            default:
                printf("Pilihan yang Anda masukkan salah.\n");
                break;
                goto Menu;
        }
    } while (1);
}

void clear_memory()
{
    while (head != NULL) {
        data_anggota temp1 = head;
        head = head->next;
        temp1->next = NULL;
        free(temp1);
    }

    while (headP != NULL) {
        data_petugas temp2 = headP;
        headP = headP->next;
        temp2->next = NULL;
        free(temp2);
    }

    while (headBuku != NULL) {
        data_buku temp3 = headBuku;
        headBuku = headBuku->next;
        temp3->next = NULL;
        free(temp3);
    }
}

void changestr(char str[], int n)
{
    str[n] = '|';
}

void loader()
{
    int i, n = 0;
    char str[] = "                                                                                                    ";
    changestr(str, n);
    for(i = 0 ; i < 30 ; i++)
    {
        changestr(str, n);
        n++;
        system("cls");
        printf("\n\n\n\n             LOADING\n");
        for (char *p = str; *p; p++) {
            putchar(*p);
            usleep(0);
        }
        puts("\n");
    }
}

int main()
{
    head = NULL;
    headP = NULL;
    headBuku = NULL;
    int pilih;

    read_dataMember();
    read_dataPetugas();
    read_dataBuku();
    init_pinjam();
    read_dataPeminjaman();

    //Alamat file backsound disesuaikan dengan alamat pada laptop masing-masing
    PlaySound(TEXT("C:\\Users\\hp14s\\OneDrive\\Documents\\Semester 3\\Struktur Data (P)\\C\\Final Project\\Project with Sound\\welcome.wav"), NULL, SND_ASYNC);
    loader();

    do {
        system("cls");
        printf( "  _____ _______ _____  _____   _      _____ ____  _____            _______     __\n"
                " / ____|__   __|_   _|/ ____| | |    |_   _|  _ \\|  __ \\     /\\   |  __ \\ \\   / /\n"
                "| (___    | |    | | | (___   | |      | | | |_) | |__) |   /  \\  | |__) \\ \\_/ / \n"
                " \\___ \\   | |    | |  \\___ \\  | |      | | |  _ <|  _  /   / /\\ \\ |  _  / \\   /  \n"
                " ____) |  | |   _| |_ ____) | | |____ _| |_| |_) | | \\ \\  / ____ \\| | \\ \\  | |   \n"
                "|_____/   |_|  |_____|_____/  |______|_____|____/|_|  \\_\\/_/    \\_\\_|  \\_\\ |_|   \n\n");
        printf("                \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 Masuk Sebagai \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\n\n"
               "                \xDB\xDB\xDB\xB2 1. Anggota                   \n\n"
               "                \xDB\xDB\xDB\xB2 2. Petugas                   \n\n"
               "                \xDB\xDB\xDB\xB2 3. Keluar Program            \n\n"
               "                \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\n"
               "                Pilihan Anda: ");

        scanf("%d", &pilih);
        Beep(600, 200);
        system("cls");

        switch (pilih)
        {
            case 1:
                login_member();
                break;

            case 2:
                login_petugas();
                break;

            case 3:
                system("cls");
                printf("\xDB=============================================\xDB\n"
                       " \xDB TERIMA KASIH TELAH MENGUNJUNGI STIS LIBRARY \xDB\n"
                       "  \xDB               HAVE A NICE DAY!              \xDB\n"
                       "   \xDB=============================================\xDB\n");
                //Alamat file backsound disesuaikan dengan alamat pada laptop masing-masing
                PlaySound(TEXT("C:\\Users\\hp14s\\OneDrive\\Documents\\Semester 3\\Struktur Data (P)\\C\\Final Project\\Project with Sound\\Thank you for visit.wav"), NULL, SND_SYNC);
                PlaySound(TEXT("C:\\Users\\hp14s\\OneDrive\\Documents\\Semester 3\\Struktur Data (P)\\C\\Final Project\\Project with Sound\\nice day.wav"), NULL, SND_SYNC);
                clear_memory();
                return 0;

            default:
                printf("Pilihan yang Anda masukkan salah.\n");
                break;
        }
    } while(1);

    return 0;
}
