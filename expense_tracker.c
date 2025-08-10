#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DESC_LEN 100
#define DATE_LEN 12
#define CAT_LEN 30
#define INITIAL_CAPACITY 100
#define FILENAME "expenses_data.txt"
#define BACKUP_FOLDER "backups"

double calculate_total();
//added as we have used this before declaration(line 221) in add_exp fn line 153

typedef struct {
    char description[DESC_LEN];
    double amount;
    char date[DATE_LEN];
    char category[CAT_LEN];
} Expense;

Expense *expenses = NULL;
int count = 0;
int capacity = 0;
double monthly_budget = 0.0;

// Utility Functions
void trim_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') str[len - 1] = '\0';
}

void input_string(const char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    trim_newline(buffer);
}

int validate_date(const char *date) {
    if (strlen(date) != 10) return 0;
    return (date[4] == '-' && date[7] == '-');
}

int validate_positive_amount(double amt) {
    return amt >= 0;
}

// Backup File Creation
void create_backup_file() {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char backup_filename[150];
    strftime(backup_filename, sizeof(backup_filename), BACKUP_FOLDER"/backup_%Y-%m-%d_%H-%M-%S.txt", tm_info);

    FILE *fp = fopen(backup_filename, "w");
    if (!fp) {
        printf("Failed to create backup file.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s,%.2lf,%s,%s\n",
                expenses[i].description,
                expenses[i].amount,
                expenses[i].date,
                expenses[i].category);
    }
    fclose(fp);
}

// File Handling
void save_to_file(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error saving file.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s,%.2lf,%s,%s\n",
                expenses[i].description,
                expenses[i].amount,
                expenses[i].date,
                expenses[i].category);
    }

    fclose(fp);
    create_backup_file();
    printf("Expenses saved and backup created.\n");
}

void load_from_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("No previous data found.\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (count >= capacity) {
            capacity *= 2;
            expenses = realloc(expenses, capacity * sizeof(Expense));
        }

        sscanf(line, " %99[^,],%lf,%11[^,],%29s",
               expenses[count].description,
               &expenses[count].amount,
               expenses[count].date,
               expenses[count].category);
        count++;
    }
    fclose(fp);
    printf("Loaded %d expenses from file.\n", count);
}

// Expense Management
void add_expense() {
    if (count >= capacity) {
        capacity *= 2;
        expenses = realloc(expenses, capacity * sizeof(Expense));
    }

    char buffer[DESC_LEN];

    input_string("Enter description: ", expenses[count].description, DESC_LEN);

    double amt;
    do {
        printf("Enter amount: ");
        scanf("%lf", &amt);
        getchar();
        if (!validate_positive_amount(amt)) {
            printf("Amount must be positive.\n");
        }
    } while (!validate_positive_amount(amt));
    expenses[count].amount = amt;

    do {
        input_string("Enter date (YYYY-MM-DD): ", buffer, DATE_LEN);
        if (!validate_date(buffer)) {
            printf("Invalid date format!\n");
        }
    } while (!validate_date(buffer));
    strcpy(expenses[count].date, buffer);

    input_string("Enter category: ", expenses[count].category, CAT_LEN);

    count++;
    printf("Expense added!\n");

    if (monthly_budget > 0 && calculate_total() > monthly_budget) {
        printf("WARNING: You have exceeded your monthly budget limit!\n");
    }
}

void view_expenses() {
    printf("\n%-20s %-10s %-12s %-15s\n", "Description", "Amount", "Date", "Category");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-20s %-10.2lf %-12s %-15s\n",
               expenses[i].description,
               expenses[i].amount,
               expenses[i].date,
               expenses[i].category);
    }
}

void delete_expense() {
    int idx;
    view_expenses();
    printf("Enter the index (1 to %d) of the expense to delete: ", count);
    scanf("%d", &idx);
    getchar();

    if (idx < 1 || idx > count) {
        printf("Invalid index!\n");
        return;
    }

    for (int i = idx - 1; i < count - 1; i++) {
        expenses[i] = expenses[i + 1];
    }
    count--;
    printf("Expense deleted.\n");
}

void filter_by_category() {
    char cat[CAT_LEN];
    input_string("Enter category to filter: ", cat, CAT_LEN);

    printf("\n%-20s %-10s %-12s %-15s\n", "Description", "Amount", "Date", "Category");
    for (int i = 0; i < count; i++) {
        if (strcmp(expenses[i].category, cat) == 0) {
            printf("%-20s %-10.2lf %-12s %-15s\n",
                   expenses[i].description,
                   expenses[i].amount,
                   expenses[i].date,
                   expenses[i].category);
        }
    }
}

void filter_by_date() {
    char date[DATE_LEN];
    input_string("Enter date to filter (YYYY-MM-DD): ", date, DATE_LEN);

    printf("\n%-20s %-10s %-12s %-15s\n", "Description", "Amount", "Date", "Category");
    for (int i = 0; i < count; i++) {
        if (strcmp(expenses[i].date, date) == 0) {
            printf("%-20s %-10.2lf %-12s %-15s\n",
                   expenses[i].description,
                   expenses[i].amount,
                   expenses[i].date,
                   expenses[i].category);
        }
    }
}

double calculate_total() {
    double total = 0;
    for (int i = 0; i < count; i++) {
        total += expenses[i].amount;
    }
    return total;
}

void display_summary() {
    double total = calculate_total();
    double avg = count ? total / count : 0;

    printf("\n--- Summary Report ---\n");
    printf("Total Expenses: %.2lf\n", total);
    printf("Average Expense: %.2lf\n", avg);
    printf("----------------------\n");
}

void set_monthly_budget() {
    printf("Set monthly budget: ");
    scanf("%lf", &monthly_budget);
    getchar();
    printf("Budget set to %.2lf\n", monthly_budget);
}

// Main Program
int main() {
    capacity = INITIAL_CAPACITY;
    expenses = malloc(capacity * sizeof(Expense));

    load_from_file(FILENAME);

    int choice;
    do {
        printf("\n========= Expense Tracker =========\n");
        printf("1. Add Expense\n");
        printf("2. View All Expenses\n");
        printf("3. Delete an Expense\n");
        printf("4. Filter by Category\n");
        printf("5. Filter by Date\n");
        printf("6. Display Summary\n");
        printf("7. Save & Backup\n");
        printf("8. Set Monthly Budget\n");
        printf("9. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: add_expense(); break;
            case 2: view_expenses(); break;
            case 3: delete_expense(); break;
            case 4: filter_by_category(); break;
            case 5: filter_by_date(); break;
            case 6: display_summary(); break;
            case 7: save_to_file(FILENAME); break;
            case 8: set_monthly_budget(); break;
            case 9:
                save_to_file(FILENAME);
                free(expenses);
                printf("Exiting...\n");
                break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 9);

    return 0;
}
