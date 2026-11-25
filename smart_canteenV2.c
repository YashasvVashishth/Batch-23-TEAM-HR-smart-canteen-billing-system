
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ITEM_COUNT 5
#define GST_RATE 5.0

// Food item details
struct Food {
    char name[30];
    float price;
    int stock;
};

// Food menu with prices and stock
struct Food menu[ITEM_COUNT] = {
    {"Tea", 10.00, 100},
    {"Coffee", 20.00, 100},
    {"Sandwich", 40.00, 50},
    {"Burger", 50.00, 50},
    {"Momos", 60.00, 40}
};

// Global variables
int quantities[ITEM_COUNT] = {0};
int totalOrders = 0;
int nextOrderID = 1001;

float dailySales = 0.0;

// Function declarations
int getNumber(void);
void showMenu(void);
void addItem(void);
void viewCart(void);
void generateBill(void);
void clearOrder(void);
void dailySalesReport(void);
void showStock(void);


// Read and validate integer input
int getNumber(void) {
    char input[100];
    char *end;
    long value;

    if (fgets(input, sizeof(input), stdin) == NULL)
        return -1;

    value = strtol(input, &end, 10);

    while (*end == ' ' || *end == '\t')
        end++;

    if (end == input || (*end != '\n' && *end != '\0'))
        return -1;

    if (value < 0 || value > 100000)
        return -1;

    return (int)value;
}


// Display food menu
void showMenu(void) {
    int i;

    printf("\n");
    printf("============================================\n");
    printf("              FOOD MENU                    \n");
    printf("============================================\n");

    printf("%-5s %-18s %-12s %-8s\n",
           "No.", "Food Item", "Price", "Stock");

    printf("--------------------------------------------\n");

    for (i = 0; i < ITEM_COUNT; i++) {
        printf("%-5d %-18s Rs.%-9.2f %-8d\n",
               i + 1,
               menu[i].name,
               menu[i].price,
               menu[i].stock);
    }

    printf("============================================\n");
}


// Add food item to cart
void addItem(void) {
    int item, qty;

    showMenu();

    printf("\nEnter item number (1-5): ");
    item = getNumber();

    if (item < 1 || item > ITEM_COUNT) {
        printf("Invalid item selection!\n");
        return;
    }

    printf("Enter quantity: ");
    qty = getNumber();

    if (qty <= 0 || qty > 100) {
        printf("Invalid quantity!\n");
        return;
    }

    if (qty > menu[item - 1].stock -
              quantities[item - 1]) {
        printf("Sorry! Insufficient stock available.\n");
        return;
    }

    quantities[item - 1] += qty;

    printf("\n%d x %s added successfully!\n",
           qty, menu[item - 1].name);
}


// View current shopping cart
void viewCart(void) {
    int i;
    float subtotal = 0.0;

    printf("\n");
    printf("============= YOUR CART =============\n");

    printf("%-18s %-8s %-10s\n",
           "Item", "Quantity", "Amount");

    printf("-------------------------------------\n");

    for (i = 0; i < ITEM_COUNT; i++) {

        if (quantities[i] > 0) {

            float amount =
                quantities[i] * menu[i].price;

            printf("%-18s %-8d Rs.%.2f\n",
                   menu[i].name,
                   quantities[i],
                   amount);

            subtotal += amount;
        }
    }

    if (subtotal == 0) {
        printf("Your cart is empty!\n");
    }

    printf("-------------------------------------\n");
    printf("Subtotal: Rs.%.2f\n", subtotal);
    printf("=====================================\n");
}


// Generate final customer bill
void generateBill(void) {
    int i, payment;

    float subtotal = 0.0;
    float discount = 0.0;
    float taxableAmount;
    float gst;
    float finalAmount;

    char customerName[50];

    // Check whether cart is empty
    int empty = 1;

    for (i = 0; i < ITEM_COUNT; i++) {
        if (quantities[i] > 0)
            empty = 0;
    }

    if (empty) {
        printf("\nYour cart is empty!\n");
        return;
    }

    // Customer details
    printf("\nEnter customer name: ");

    if (fgets(customerName, sizeof(customerName),
               stdin) == NULL) {
        printf("Invalid customer name!\n");
        return;
    }

    customerName[strcspn(customerName, "\n")] = '\0';

    if (strlen(customerName) == 0) {
        printf("Customer name cannot be empty!\n");
        return;
    }

    // Calculate subtotal
    for (i = 0; i < ITEM_COUNT; i++) {
        subtotal += quantities[i] * menu[i].price;
    }

    // Apply discount
    if (subtotal >= 500) {
        discount = subtotal * 0.10;
    }
    else if (subtotal >= 300) {
        discount = subtotal * 0.05;
    }

    taxableAmount = subtotal - discount;
    gst = taxableAmount * GST_RATE / 100.0;

   
    finalAmount = taxableAmount + gst;

   
    printf("\nSelect Payment Method:\n");
    printf("1. Cash\n");
    printf("2. UPI\n");
    printf("Enter choice: ");

    payment = getNumber();

    if (payment != 1 && payment != 2) {
        printf("Invalid payment method!\n");
        return;
    }

    // Print receipt
    printf("\n");
    printf("============================================\n");
    printf("          SMART CANTEEN RECEIPT             \n");
    printf("============================================\n");

    printf("Order ID      : %d\n", nextOrderID);
    printf("Customer Name : %s\n", customerName);

    printf("--------------------------------------------\n");

    printf("%-15s %-5s %-8s %-10s\n",
           "Item", "Qty", "Price", "Amount");

    printf("--------------------------------------------\n");

    for (i = 0; i < ITEM_COUNT; i++) {

        if (quantities[i] > 0) {

            float amount =
                quantities[i] * menu[i].price;

            printf("%-15s %-5d %-8.2f Rs.%.2f\n",
                   menu[i].name,
                   quantities[i],
                   menu[i].price,
                   amount);
        }
    }

    printf("--------------------------------------------\n");

    printf("Subtotal          : Rs.%.2f\n", subtotal);
    printf("Discount          : Rs.%.2f\n", discount);
    printf("Taxable Amount    : Rs.%.2f\n", taxableAmount);
    printf("GST (5%%)          : Rs.%.2f\n", gst);

    printf("--------------------------------------------\n");

    printf("FINAL AMOUNT      : Rs.%.2f\n", finalAmount);

    printf("Payment Method    : %s\n",
           payment == 1 ? "Cash" : "UPI");

    printf("============================================\n");
    printf("       THANK YOU! VISIT AGAIN!              \n");
    printf("============================================\n");

   
    for (i = 0; i < ITEM_COUNT; i++) {
        menu[i].stock -= quantities[i];
        quantities[i] = 0;
    }

    
    dailySales += finalAmount;
    totalOrders++;
    nextOrderID++;

    printf("\nOrder completed successfully!\n");
}


// Clear current order
void clearOrder(void) {
    int i;

    for (i = 0; i < ITEM_COUNT; i++) {
        quantities[i] = 0;
    }

    printf("\nCart cleared successfully!\n");
}


// Display daily sales report
void dailySalesReport(void) {

    printf("\n");
    printf("=====================================\n");
    printf("          DAILY SALES REPORT         \n");
    printf("=====================================\n");

    printf("Total Orders : %d\n", totalOrders);
    printf("Total Sales  : Rs.%.2f\n", dailySales);

    printf("=====================================\n");
}


// Display stock information
void showStock(void) {
    int i;

    printf("\n========== INVENTORY REPORT ==========\n");

    for (i = 0; i < ITEM_COUNT; i++) {
        printf("%-18s Remaining: %d\n",
               menu[i].name,
               menu[i].stock);
    }

    printf("======================================\n");
}


// Main function
int main(void) {

    int choice;

    printf("\n");
    printf("****************************************\n");
    printf("*                                      *\n");
    printf("*       SMART CANTEEN BILLING SYSTEM   *\n");
    printf("*                                      *\n");
    printf("****************************************\n");

    do {

        printf("\n");
        printf("============= MAIN MENU =============\n");

        printf("1. Display Food Menu\n");
        printf("2. Add Item to Cart\n");
        printf("3. View Cart\n");
        printf("4. Generate Final Bill\n");
        printf("5. Clear Cart\n");
        printf("6. Daily Sales Report\n");
        printf("7. View Inventory\n");
        printf("8. Exit\n");

        printf("=====================================\n");
        printf("Enter your choice: ");

        choice = getNumber();

        switch (choice) {

            case 1:
                showMenu();
                break;

            case 2:
                addItem();
                break;

            case 3:
                viewCart();
                break;

            case 4:
                generateBill();
                break;

            case 5:
                clearOrder();
                break;

            case 6:
                dailySalesReport();
                break;

            case 7:
                showStock();
                break;

            case 8:
                printf("\nThank you for using Smart Canteen!\n");
                break;

            default:
                printf("\nInvalid choice! Please try again.\n");
        }

    } while (choice != 8);

    return 0;
}
