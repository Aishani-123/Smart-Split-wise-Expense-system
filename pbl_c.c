#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reg_log.c"
#include "group.c"
#include "ex_cate.c"
#include "add_exp.c"
#include "settle_pay.c"
#include "history.c"
#include "split.c"
/*struct User {
    char username[20];
    char password[20];
    char contact[20];
    char upi[20]; };
struct Member {
    char name[20];
    char contact[20];
    char upi[20]; };
struct Group {
    char groupName[20];
    struct Member members[20];
    int memberCount; };
struct Category {
    char categoryName[20]; };
struct Expense {
    char groupName[20];
    char category[20];
    char payer[20];
    char participants[20];
    float amount;
    char date[10]; };
void register_user(const char *username, const char *password, const char *contact, const char *upi) {
    struct User u;
    FILE *fp = fopen("users.txt","a+");
    if (fp == NULL) {
        printf("Error opening user file.\n");
        return;
    }
    while (fread(&u, sizeof(struct User), 1, fp)) {
        if (strcmp(u.username, username) == 0) {
            fclose(fp);
            printf("User already exists.\n");
            return;
        }
    }
    strcpy(u.username, username);
    strcpy(u.password, password);
    strcpy(u.contact, contact);
    strcpy(u.upi, upi);

    fwrite(&u, sizeof(struct User), 1, fp);
    fclose(fp);
    printf("Registration successful.\n");
}
int login_user(const char *username, const char *password) {
    struct User u;
    FILE *fp = fopen("users.txt", "r");
    if (fp == NULL) {
        printf("No users found.\n");
        return 0;
    }
    while (fread(&u, sizeof(struct User), 1, fp)) {
        if (strcmp(u.username, username) == 0 && strcmp(u.password, password) == 0) {
            fclose(fp);
            return 1; 
        }
    }
    fclose(fp);
    return 0; 
}
void create_group(const char *groupName) {
    struct Group g;
    FILE *fp = fopen("groups.dat", "a+");
    if (fp == NULL) {
        printf("Error opening group file.\n");
        return;
    }
    while (fread(&g, sizeof(struct Group), 1, fp)) {
        if (strcmp(g.groupName, groupName) == 0) {
            fclose(fp);
            printf("Group already exists.\n");
            return;
        }
    }
    strcpy(g.groupName, groupName);
    g.memberCount = 0;
    fwrite(&g, sizeof(struct Group), 1, fp);
    fclose(fp);
    printf("Group created successfully.\n");
}
void list_groups() {
    struct Group g;
    FILE *fp = fopen("groups.dat", "r");
    if (fp == NULL) {
        printf("No groups found.\n");
        return;
    }
    printf("\n--- Groups ---\n");
    while (fread(&g, sizeof(struct Group), 1, fp)) {
        if (strcmp(g.groupName, "DELETED") != 0) {
            printf("Group: %s | Members: %d\n", g.groupName, g.memberCount);
        }
    }
    fclose(fp);
}
void add_member(const char *groupName, const char *memberName, const char *contact, const char *upi) {
    struct Group g;
    FILE *fp = fopen("groups.dat", "r+");
    if (fp == NULL) {
        printf("Error opening group file.\n");
        return;
    }
    while (fread(&g, sizeof(struct Group), 1, fp)) {
        if (strcmp(g.groupName, groupName) == 0) {
            if (g.memberCount >= 20) {
                printf("Group is full!\n");
                fclose(fp);
                return;
            }
            strcpy(g.members[g.memberCount].name, memberName);
            strcpy(g.members[g.memberCount].contact, contact);
            strcpy(g.members[g.memberCount].upi, upi);
            g.memberCount++;
            fseek(fp, -(long)sizeof(struct Group), SEEK_CUR);
            fwrite(&g, sizeof(struct Group), 1, fp);
            fclose(fp);
            printf("Member added successfully.\n");
            return;
        }
    }
    fclose(fp);
    printf("Group not found.\n");
}
void delete_group(const char *groupName) {
    struct Group g;
    FILE *fp = fopen("groups.dat", "r");
    FILE *temp = fopen("temp.dat", "w"); 
    int found = 0;
    if (!fp || !temp) {
        printf("Error opening file.\n");
        return;
    }
    while (fread(&g, sizeof(struct Group), 1, fp)) {
        if (strcmp(g.groupName, groupName) != 0) {
            fwrite(&g, sizeof(struct Group), 1, temp);
        } else {
            found = 1;
        }
    }
    fclose(fp);
    fclose(temp);
    remove("groups.txt");
    rename("temp.txt", "groups.txt");
    if (found)
        printf("Group deleted successfully.\n");
    else
        printf("Group not found.\n");
}
void add_category(const char *categoryName) {
    struct Category c;
    FILE *fp = fopen("categories.dat", "a+");
    if (fp == NULL) {
        printf("Error opening category file.\n");
        return;
    }
    while (fread(&c, sizeof(struct Category), 1, fp)) {
        if (strcmp(c.categoryName, categoryName) == 0) {
            fclose(fp);
            printf("Category '%s' already exists.\n", categoryName);
            return;
        }
    }
    strcpy(c.categoryName, categoryName);
    fwrite(&c, sizeof(struct Category), 1, fp);
    fclose(fp);
    printf("Category '%s' added successfully.\n", categoryName);
}
void list_categories() {
    struct Category c;
    FILE *fp = fopen("categories.dat", "r");
    if (fp == NULL) {
        printf("No categories found.\n");
        return;
    }
    printf("\n--- Expense Categories ---\n");
    int count = 0;
    while (fread(&c, sizeof(struct Category), 1, fp)) {
        printf("%d. %s\n", ++count, c.categoryName);
    }

    if (count == 0)
        printf("No categories available.\n");

    fclose(fp);
}
void add_expense(const char *groupName, const char *category, const char *payer,
                 const char *participants, float amount, const char *date) {
    struct Expense e;
    FILE *fp = fopen("expenses.dat", "a");
    if (fp == NULL) {
        printf("Error opening expense file.\n");
        return;
    }
    strcpy(e.groupName, groupName);
    strcpy(e.category, category);
    strcpy(e.payer, payer);
    strcpy(e.participants, participants);
    e.amount = amount;
    strcpy(e.date, date);
    fwrite(&e, sizeof(struct Expense), 1, fp);
    fclose(fp);
}
void list_expenses() {
    struct Expense e;
    FILE *fp = fopen("expenses.dat", "r");
    if (fp == NULL) {
        printf("No expenses found.\n");
        return;
    }
    int count = 0;
    while (fread(&e, sizeof(struct Expense), 1, fp)) {
        printf("%d. Group: %s | Category: %s | Payer: %s | Participants: %s | Amount: %.2f | Date: %s\n",
               ++count, e.groupName, e.category, e.payer, e.participants, e.amount, e.date);
    }
    fclose(fp);
}
void settle(const char *groupName, const char *payer, const char *receiver) {
    struct Expense e;
    FILE *fp = fopen("expenses.dat", "r");
    if (!fp) {
        fp = fopen("expenses.dat", "a+");
        if (!fp) {
            printf("Error creating expense file.\n");
            return;
        }
    }
    FILE *temp = fopen("temp.dat", "w");
    int found = 0;
    if (!fp || !temp) {
        printf("Error opening expense files.\n");
        return;
    }
    while (fread(&e, sizeof(struct Expense), 1, fp)) {
        if (strcmp(e.groupName, groupName) == 0 &&
            strstr(e.participants, receiver) != NULL &&
            strcmp(e.payer, payer) == 0) {
            if (strstr(e.date, "(Settled)") == NULL) {
                strcat(e.date, " (Settled)");
                found = 1;
            }
        }
        fwrite(&e, sizeof(struct Expense), 1, temp);
    }
    fclose(fp);
    fclose(temp);
    remove("expenses.dat");
    rename("temp.dat", "expenses.dat");
    if (found)
        printf("Settlement successful.\n");
    else
        printf(" No unpaid expense found.\n");
}
*/