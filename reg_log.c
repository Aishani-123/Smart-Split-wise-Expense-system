struct User {
    char username[50];
    char password[50];
    char contact[20];
    char upi[50]; };
void register_user(const char *username, const char *password, const char *contact, const char *upi) {
    struct User u;
    FILE *fp = fopen("users.txt","ab+");
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
}
int login_user(const char *username, const char *password) {
    struct User u;
    FILE *fp = fopen("users.txt", "rb");
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