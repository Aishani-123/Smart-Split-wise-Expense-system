struct Category {
    char categoryName[20]; };
void add_category(const char *categoryName) {
    struct Category c;
    FILE *fp = fopen("categories.txt", "ab+");
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
    FILE *fp = fopen("categories.txt", "rb");
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