from tkinter import *
import ctypes, os, io, sys, tempfile
lib = ctypes.CDLL(os.path.abspath("pbl_c.dll"))
lib.register_user.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p]
lib.register_user.restype = None
lib.login_user.argtypes = [ctypes.c_char_p, ctypes.c_char_p]
lib.login_user.restype = ctypes.c_int
lib.create_group.argtypes = [ctypes.c_char_p]
lib.create_group.restype = None
lib.delete_group.argtypes = [ctypes.c_char_p]
lib.delete_group.restype = None
lib.add_member.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p]
lib.add_member.restype = None
lib.list_groups.argtypes = []
lib.list_groups.restype = None
lib.add_category.argtypes = [ctypes.c_char_p]
lib.add_category.restype = None
lib.list_categories.argtypes = []
lib.list_categories.restype = None
lib.add_expense.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_float, ctypes.c_char_p]
lib.add_expense.restype = None
lib.list_expenses.argtypes = []
lib.list_expenses.restype = None
lib.settle.argtypes = [ctypes.c_char_p,ctypes.c_char_p,ctypes.c_char_p]
lib.settle.restype = None
lib.ex_hist.argtypes = None
lib.ex_hist.restype = None
lib.sp_ex.argtypes =  [ctypes.c_char_p,ctypes.c_char_p,ctypes.c_char_p,ctypes.c_char_p,ctypes.c_float,ctypes.c_char_p]
lib.sp_ex.restype = None
logged_user = ""
def show(frame):
    frame.tkraise()

def clear_frame(frame):
    for i in frame.winfo_children():
        i.destroy()

def register():
    user = uval.get()
    pwd = pval.get()
    contact = cval.get()
    upi = upival.get()
    if not (user and pwd and contact and upi):
        Label(p3, text="All fields required!").grid(row=6, column=0, columnspan=2, pady=10)
        return
    lib.register_user(user.encode(), pwd.encode(), contact.encode(), upi.encode())
    clear_frame(p3)
    Label(p3, text="Registration Successful!", fg="green").grid(row=0, column=0, columnspan=2, pady=10)
    Button(p3, text="Login", command=lambda: show(p2)).grid(row=1, column=0, padx=10, pady=10)
    Button(p3, text="Exit", command=root.destroy).grid(row=1, column=1, padx=10, pady=10)

def login():
    global logged_user
    user = userval.get()
    pwd = passval.get()
    res = lib.login_user(user.encode(), pwd.encode())
    if res:
        logged_user = user
        show(p4)
    else:
        Label(p2, text="Invalid Credentials!", fg="red").grid(row=4, column=0, columnspan=2, pady=10)

def groups_page():
    clear_frame(p5)
    Label(p5, text=f"{logged_user}'s Groups", font=("Arial", 14)).pack(pady=10)
    text = Text(p5, width=60, height=15)
    text.pack()
    buffer = io.StringIO()
    sys.stdout = buffer
    lib.list_groups()
    sys.stdout = sys.__stdout__
    text.insert(END, buffer.getvalue())
    with open("groups.txt", "r", encoding="utf-8", errors="ignore") as f:
        content = f.read()
    text.delete(1.0, END)
    text.insert(END, content)
    Button(p5, text="Create Group", command=create_group_ui).pack(pady=5)
    Button(p5, text="Add Member", command=add_member_ui).pack(pady=5)
    Button(p5, text="Delete Group", command=delete_group_ui).pack(pady=5)
    Button(p5, text="Back", command=lambda: show(p4)).pack(pady=10)
    show(p5)

def create_group_ui():
    top = Toplevel(root)
    top.title("Create Group")
    Label(top, text="Group Name").pack()
    gname = Entry(top)
    gname.pack()
    Button(top, text="Submit", command=lambda: lib.create_group(gname.get().encode()) or Label(top, text="Group Created!", fg="green").pack()).pack()

def delete_group_ui():
    top = Toplevel(root)
    top.title("Delete Group")
    Label(top, text="Group Name").pack()
    gname = Entry(top)
    gname.pack()
    Button(top, text="Delete", command=lambda: lib.delete_group(gname.get().encode()) or Label(top, text="Group Deleted!", fg="green").pack()).pack()

def add_member_ui():
    top = Toplevel(root)
    top.title("Add Member")
    Label(top, text="Group Name").pack()
    gname = Entry(top)
    gname.pack()
    Label(top, text="Member Name").pack()
    mname = Entry(top)
    mname.pack()
    Label(top, text="Contact").pack()
    mcontact = Entry(top)
    mcontact.pack()
    Label(top, text="UPI").pack()
    mupi = Entry(top)
    mupi.pack()
    Button(top, text="Submit", command=lambda: lib.add_member(gname.get().encode(), mname.get().encode(), mcontact.get().encode(), mupi.get().encode()) or Label(top, text="Member Added!", fg="green").pack()).pack()

def ex_cat_page():
    clear_frame(p6)
    Label(p6, text="Categories", font=("Arial", 14)).pack(pady=10)
    text = Text(p6, width=60, height=15)
    text.pack()
    buffer = io.StringIO()
    sys.stdout = buffer
    lib.list_categories()
    sys.stdout = sys.__stdout__
    content = buffer.getvalue()
    #text.insert(END, content if content.strip() else "No categories found.\n")
    Button(p6, text="Add Category", command=add_category_ui).pack(pady=5)
    Button(p6, text="Back", command=lambda: show(p4)).pack(pady=10)
    show(p6)

def add_category_ui():
    clear_frame(p6)
    Label(p6, text="Add New Category", font=("Arial", 14)).pack(pady=10)
    Label(p6, text="Category Name:").pack()
    category_entry = Entry(p6, width=30)
    category_entry.pack(pady=5)
    Button(p6, text="Add", command=lambda: lib.add_category(category_entry.get().encode()) or ex_cat_page()).pack(pady=5)
    Button(p6, text="Back", command=ex_cat_page).pack(pady=10)

def split_page():
    clear_frame(p10)
    Label(p10, text="Split Expenses",font=("Arial", 16, "bold")).pack(pady=10)
    def create_label_entry(text):
        Label(p10, text=text, bg="lavenderblush", fg="darkmagenta").pack()
        entry = Entry(p10, width=50)
        entry.pack(pady=3)
        return entry
    group_entry = create_label_entry("Group Name:")
    category_entry = create_label_entry("Category:")
    payer_entry = create_label_entry("Payer Name:")
    participants_entry = create_label_entry("Participants (comma separated):")
    amount_entry = create_label_entry("Amount:")
    date_entry = create_label_entry("Date (DD/MM/YYYY):")
    Label(p10, text="Result:", bg="lavenderblush", fg="darkmagenta").pack(pady=(10, 2))
    text = Text(p10, width=90, height=18, wrap="word",fg="black")
    text.pack(padx=10, pady=5)
    def call_split():
        text.delete("1.0", END)
        group = group_entry.get().strip()
        category = category_entry.get().strip()
        payer = payer_entry.get().strip()
        participants = participants_entry.get().strip()
        amount = amount_entry.get().strip()
        date = date_entry.get().strip()
        if not (group and category and payer and participants and amount and date):
            text.insert(END, "Please fill in all fields.\n")
            return
        try:
            amount = float(amount)
        except ValueError:
            text.insert(END, "Amount must be a valid number.\n")
            return
        buffer = io.StringIO()
        sys.stdout = buffer
        lib.sp_ex(group.encode('utf-8'),category.encode('utf-8'),
                  payer.encode('utf-8'),participants.encode('utf-8'),ctypes.c_float(amount),date.encode('utf-8'))
        sys.stdout = sys.__stdout__
        text.insert(END, buffer.getvalue())
    Button(p10, text="Split Expense",command=call_split,bg="plum", fg="white",relief="raised", width=18).pack(pady=10)
    Button(p10, text="Back",command=lambda: show(p4),bg="lightpink", fg="black",width=18).pack(pady=(0, 10))
    show(p10)

def view_page():
    pass
def settle_page():
    clear_frame(p8)
    Label(p8, text="Settle Payments", font=("Arial", 14)).pack(pady=10)

    text = Text(p8, width=90, height=15)
    text.pack()

    buffer = io.StringIO()
    sys.stdout = buffer
    lib.list_expenses() 
    sys.stdout = sys.__stdout__
    content = buffer.getvalue()
    #text.insert(END, content if content.strip() else "No expenses found.\n")

    Button(p8, text="Settle Payment", command=settle_ui).pack(pady=5)
    Button(p8, text="Back", command=lambda: show(p4)).pack(pady=10)
    show(p8)
def settle_ui():
    clear_frame(p8)
    Label(p8, text="Settle Payment", font=("Arial", 14)).pack(pady=10)

    Label(p8, text="Group Name:").pack()
    group_entry = Entry(p8, width=30)
    group_entry.pack(pady=5)

    Label(p8, text="Payer Name:").pack()
    payer_entry = Entry(p8, width=30)
    payer_entry.pack(pady=5)

    Label(p8, text="Receiver Name:").pack()
    receiver_entry = Entry(p8, width=30)
    receiver_entry.pack(pady=5)

    def process_settlement():
        group = group_entry.get().strip()
        payer = payer_entry.get().strip()
        receiver = receiver_entry.get().strip()
        if not (group and payer and receiver):
            print("Please fill all fields.")
            return
        lib.settle(group.encode(), payer.encode(), receiver.encode())
        settle_page()  

    Button(p8, text="Settle", command=process_settlement).pack(pady=5)
    Button(p8, text="Back", command=settle_page).pack(pady=10)

def hist_page():
    clear_frame(p9)
    Label(p9, text="Expense History", font=("Arial", 14)).pack(pady=10)
    text = Text(p9, width=90, height=15)
    text.pack()
    buffer = io.StringIO()
    sys.stdout = buffer
    lib.ex_hist() 
    sys.stdout = sys.__stdout__
    content = buffer.getvalue()
    Button(p9, text="Back", command=lambda: show(p4)).pack(pady=10)
    show(p9)

def report_page():
    pass

def add_page():
    clear_frame(p7)
    Label(p7, text="Expenses", font=("Arial", 14)).pack(pady=10)
    text = Text(p7, width=90, height=15)
    text.pack()
    buffer = io.StringIO()
    sys.stdout = buffer
    lib.list_expenses()
    sys.stdout = sys.__stdout__
    content = buffer.getvalue()
    #text.insert(END, content if content.strip() else "No expenses found.\n")
    Button(p7, text="Add Expense", command=add_expense_ui).pack(pady=5)
    Button(p7, text="Back", command=lambda: show(p4)).pack(pady=10)
    show(p7)

def add_expense_ui():
    clear_frame(p7)
    Label(p7, text="Add New Expense", font=("Arial", 14)).pack(pady=10)
    entries = {}
    fields = ["Group Name", "Category", "Payer", "Participants (comma-separated)", "Amount", "Date (YYYY-MM-DD)"]
    for field in fields:
        Label(p7, text=field + ":").pack()
        entry = Entry(p7, width=40)
        entry.pack(pady=3)
        entries[field] = entry
    Button(p7, text="Add Expense", command=lambda: lib.add_expense(
        entries["Group Name"].get().encode(),
        entries["Category"].get().encode(),
        entries["Payer"].get().encode(),
        entries["Participants (comma-separated)"].get().encode(),
        float(entries["Amount"].get()),
        entries["Date (YYYY-MM-DD)"].get().encode()
    ) or add_page()).pack(pady=5)
    Button(p7, text="Back", command=add_page).pack(pady=10)

root = Tk()
root.geometry(f"{root.winfo_screenwidth()}x{root.winfo_screenheight()}+0+0")
root.title("Smart Splitwise System")
cont = Frame(root)
cont.pack(fill="both", expand=True)
cont.grid_rowconfigure(0, weight=1)
cont.grid_columnconfigure(0, weight=1)

p1 = Frame(cont, bg="lightpink")
p2 = Frame(cont, bg="lightyellow")
p3 = Frame(cont, bg="lavender")
p4 = Frame(cont, bg="lightblue")
p5 = Frame(cont, bg="thistle")
p6 = Frame(cont, bg="lavender")
p7 = Frame(cont, bg="plum")
p8 = Frame(cont, bg="lavenderblush")
p9 = Frame(cont, bg="lightpink")
p10 = Frame(cont, bg="lavender")
for f in (p1, p2, p3, p4, p5, p6, p7, p8, p9, p10):
    f.grid(row=0, column=0, sticky="nsew")

Label(p1, text="Login and Register", font=("Arial", 16)).pack(pady=20)
Button(p1, text="Login", command=lambda: show(p2)).pack(pady=5)
Button(p1, text="Register", command=lambda: show(p3)).pack(pady=5)
Button(p1, text="Exit", command=root.destroy).pack(pady=5)

Label(p2, text="Login", font=("Arial", 14)).grid(row=0, column=0, columnspan=2, pady=10)
Label(p2, text="Username").grid(row=1, column=0)
Label(p2, text="Password").grid(row=2, column=0)
userval = StringVar()
passval = StringVar()
Entry(p2, textvariable=userval).grid(row=1, column=1)
Entry(p2, textvariable=passval).grid(row=2, column=1)
Button(p2, text="Submit", command=login).grid(row=3, column=0, columnspan=2, pady=10)
Button(p2, text="Back", command=lambda: show(p1)).grid(row=5, column=0, columnspan=2, pady=10)

Label(p3, text="Register", font=("Arial", 14)).grid(row=0, column=0, columnspan=2)
Label(p3, text="Username").grid(row=1, column=0)
Label(p3, text="Password").grid(row=2, column=0)
Label(p3, text="Contact").grid(row=3, column=0)
Label(p3, text="UPI").grid(row=4, column=0)
uval, pval, cval, upival = StringVar(), StringVar(), StringVar(), StringVar()
Entry(p3, textvariable=uval).grid(row=1, column=1)
Entry(p3, textvariable=pval).grid(row=2, column=1)
Entry(p3, textvariable=cval).grid(row=3, column=1)
Entry(p3, textvariable=upival).grid(row=4, column=1)
Button(p3, text="Submit", command=register).grid(row=5, column=0, columnspan=2, pady=10)
Button(p3, text="Back", command=lambda: show(p1)).grid(row=6, column=0, columnspan=2, pady=10)

Label(p4, text="Dashboard", font=("Arial", 16)).pack(pady=20)
buttons = ["Groups", "Expense Categorization", "Add Expenses", "Split Expenses","View Balance", "Settle Payments", "Expense History", "Basic Report", "Exit"]
for b in buttons:
    if b == "Groups":
        Button(p4, text=b, width=25, command=groups_page).pack(pady=5)
    elif b == "Expense Categorization":
        Button(p4, text=b, width=25, command=ex_cat_page).pack(pady=5)
    elif b == "Add Expenses":
        Button(p4, text=b, width=25, command=add_page).pack(pady=5)
    elif b == "Split Expenses":
        Button(p4, text=b, width=25, command=split_page).pack(pady=5)
    #elif b == "View Balance":
        #Button(p4, text=b, width=25, command=view_page).pack(pady=5)
    elif b == "Settle Payments":
        Button(p4, text=b, width=25, command=settle_page).pack(pady=5)
    elif b == "Expense History":
        Button(p4, text=b, width=25, command=hist_page).pack(pady=5)
    #elif b == "Basic Report":
        #Button(p4, text=b, width=25, command=report_page).pack(pady=5)
    elif b == "Exit":
        Button(p4, text=b, width=25, command=root.destroy).pack(pady=5)

show(p1)
root.mainloop()
