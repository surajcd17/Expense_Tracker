# 💰 Expense Tracker (C Language)

A simple command-line **Expense Tracker** written in C, designed to help users manage and track their daily expenses.  
It supports adding, viewing, deleting, filtering, generating summaries, setting a monthly budget, and creating automatic backups.

---

## 📜 Features
- **Add Expense** with description, amount, date, and category
- **View All Expenses** in a tabular format
- **Delete Expense** by selecting its index
- **Filter** by category or date
- **Summary Report** (total & average expenses)
- **Set Monthly Budget** with warnings if exceeded
- **Save & Load** expenses from a file
- **Automatic Backup** on every save

---

## 🗂️ File Structure
- `expense_tracker.c` → Main source code
- `expenses_data.txt` → Data file storing expenses
- `backups/` → Folder storing timestamped backups (created automatically)

---

## 📦 How to Compile & Run
### **Compile**
```bash
gcc expense_tracker.c -o expense_tracker

### **Run**
./expense_tracker


###Sample output

========= Expense Tracker =========
1. Add Expense
2. View All Expenses
3. Delete an Expense
4. Filter by Category
5. Filter by Date
6. Display Summary
7. Save & Backup
8. Set Monthly Budget
9. Exit
Enter choice:

🛠️ Requirements
GCC Compiler (or any C compiler)

Basic terminal/command prompt knowledge