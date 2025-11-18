struct Category { 
    char categoryName[20];
};
void add_category(const char *categoryName) {
    struct Category c;
    FILE *fp = fopen("categories.txt", "a+"); 
    if (fp == NULL) {
        printf("Error opening category file.\n");
        return;
    }
    int exists = 0;
    while (fscanf(fp, "%s", c.categoryName) == 1) {
        if (strcmp(c.categoryName, categoryName) == 0) {
            exists = 1;
            break;
        }
    }
    if (exists) {
        fclose(fp);
        printf("Category '%s' already exists.\n", categoryName);
        return;
    }
    strcpy(c.categoryName, categoryName);
    fprintf(fp, "%s\n", c.categoryName);
    fclose(fp);
    printf("Category '%s' added successfully.\n", categoryName);
}
void list_categories() {
    struct Category c;
    FILE *fp = fopen("categories.txt", "r");  
    if (fp == NULL) {
        printf("No categories found.\n");
        return;
    }
    printf("\n--- Expense Categories ---\n");
    int count = 0;
    while (fscanf(fp, "%s", c.categoryName) == 1) {
        printf("%d. %s\n", ++count, c.categoryName);
    }
    if (count == 0)
        printf("No categories available.\n");
    fclose(fp);
}
