#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_HOUSES 50
#define MAX_BOOKINGS 100

struct House {
    int id;
    char location[50];
    int price;
    int capacity;
    int status;
};

struct Booking {
    int house_id;
    char customer_name[50];
    char phone[15];
    char date[20];
    int booking_id;
};

struct House houses[MAX_HOUSES];
struct Booking bookings[MAX_BOOKINGS];
int house_count = 0;
int booking_count = 0;
int next_booking_id = 1;
const char* ADMIN_PASSWORD = "Admin"; //You can change this password as needed

void saveHousesToFile();
void saveBookingsToFile();
void loadHousesFromFile();
void loadBookingsFromFile();
int checkAdminPassword();
void addHouse();
void viewAllHouses();
void viewAvailableHouses();
void searchHouses();
void bookHouse();
void viewAllBookings();
void viewMyBookings(char* customer_name);
void cancelBooking();
void deleteHouse();
void modifyHouse();
void adminMenu();
void customerMenu();
int findHouseById(int id);

int main() {
    loadHousesFromFile();
    loadBookingsFromFile();

    printf("\n=== HOUSE BOOKING MANAGEMENT SYSTEM ===\n");

    int choice, role;
    while(1) {
        printf("\n--- SELECT ROLE ---\n");
        printf("1. Admin Login\n");
        printf("2. Customer Login\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &role);

        switch(role) {
            case 1: adminMenu(); break;
            case 2: customerMenu(); break;
            case 3:
                printf("Saving data...\n");
                saveHousesToFile();
                saveBookingsToFile();
                printf("Thank you! Goodbye!\n");
                exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}

void saveHousesToFile() {
    FILE *f = fopen("houses.dat", "wb");
    if(f) {
        fwrite(&house_count, sizeof(int), 1, f);
        fwrite(houses, sizeof(struct House), house_count, f);
        fclose(f);
        printf("Houses saved to file!\n");
    }
}

void saveBookingsToFile() {
    FILE *f = fopen("bookings.dat", "wb");
    if(f) {
        fwrite(&booking_count, sizeof(int), 1, f);
        fwrite(&next_booking_id, sizeof(int), 1, f);
        fwrite(bookings, sizeof(struct Booking), booking_count, f);
        fclose(f);
        printf("Bookings saved to file!\n");
    }
}

void loadHousesFromFile() {
    FILE *f = fopen("houses.dat", "rb");
    if(f) {
        fread(&house_count, sizeof(int), 1, f);
        if(house_count > 0 && house_count <= MAX_HOUSES)
            fread(houses, sizeof(struct House), house_count, f);
        fclose(f);
        printf("Houses loaded from file!\n");
    }
}

void loadBookingsFromFile() {
    FILE *f = fopen("bookings.dat", "rb");
    if(f) {
        fread(&booking_count, sizeof(int), 1, f);
        fread(&next_booking_id, sizeof(int), 1, f);
        if(booking_count > 0 && booking_count <= MAX_BOOKINGS)
            fread(bookings, sizeof(struct Booking), booking_count, f);
        fclose(f);
        printf("Bookings loaded from file!\n");
    }
}

// ========== ADMIN PASSWORD ==========
int checkAdminPassword() {
    char password[20];
    int attempts = 5;

    printf("Admin Login Required\n");
    while(attempts > 0) {
        printf("Enter Password: ");
        scanf("%s", password);

        if(strcmp(password, ADMIN_PASSWORD) == 0) {
            printf("Login successful!\n");
            return 1;
        }
        attempts--;
        printf("Wrong password! %d attempts left.\n", attempts);
    }
    printf("Access denied! Returning to main menu...\n");
    return 0;
}

// ========== ADMIN MENU ==========
void adminMenu() {
    if(!checkAdminPassword()) return;

    int choice;
    printf("\n=== ADMIN PANEL ===\n");
    while(1) {
        printf("\n  1. Add House\n");
        printf("  2. View All Houses\n");
        printf("  3. View All Bookings\n");
        printf("  4. Delete House\n");
        printf("  5. Modify House\n");
        printf("  6. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: addHouse(); saveHousesToFile(); break;
            case 2: viewAllHouses(); break;
            case 3: viewAllBookings(); break;
            case 4: deleteHouse(); saveHousesToFile(); break;
            case 5: modifyHouse(); saveHousesToFile(); break;
            case 6: printf("Logged out!\n"); return;
            default: printf("Invalid choice!\n");
        }
    }
}

// ========== CUSTOMER MENU ==========
void customerMenu() {
    char customer_name[50];
    printf("\nEnter your name: ");
    scanf("%s", customer_name);

    int choice;
    printf("\n=== CUSTOMER PANEL (Welcome %s) ===\n", customer_name);
    while(1) {
        printf("\n  1. View Available Houses\n");
        printf("  2. Search Houses\n");
        printf("  3. Book House\n");
        printf("  4. View My Bookings\n");
        printf("  5. Cancel Booking\n");
        printf("  6. Back to Main\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: viewAvailableHouses(); break;
            case 2: searchHouses(); break;
            case 3: bookHouse(); saveBookingsToFile(); saveHousesToFile(); break;
            case 4: viewMyBookings(customer_name); break;
            case 5: cancelBooking(); saveBookingsToFile(); saveHousesToFile(); break;
            case 6: return;
            default: printf("Invalid choice!\n");
        }
    }
}

void addHouse() {
    if(house_count >= MAX_HOUSES) {
        printf("Max houses limit reached!\n");
        return;
    }

    struct House h;
    h.id = (house_count > 0) ? houses[house_count-1].id + 1 : 1;
    printf("Enter location: ");
    scanf("%s", h.location);
    printf("Enter price per month: ");
    scanf("%d", &h.price);
    printf("Enter capacity (people): ");
    scanf("%d", &h.capacity);
    h.status = 0;

    houses[house_count] = h;
    house_count++;
    printf("House added! ID: %d\n", h.id);
}

void viewAllHouses() {
    if(house_count == 0) {
        printf("No houses available!\n");
        return;
    }

    printf("\nALL HOUSES\n");
    printf("%-5s %-20s %-8s %-8s %-10s\n", "ID", "Location", "Price", "Capacity", "Status");
    printf("------------------------------------------------------\n");

    for(int i = 0; i < house_count; i++) {
        printf("%-5d %-20s %-8d %-8d %-10s\n",
               houses[i].id, houses[i].location,
               houses[i].price, houses[i].capacity,
               houses[i].status ? "Booked" : "Available");
    }
}

void viewAvailableHouses() {
    int found = 0;
    printf("\nAVAILABLE HOUSES\n");
    printf("%-5s %-20s %-8s %-8s\n", "ID", "Location", "Price", "Capacity");
    printf("---------------------------------------------\n");

    for(int i = 0; i < house_count; i++) {
        if(houses[i].status == 0) {
            printf("%-5d %-20s %-8d %-8d\n",
                   houses[i].id, houses[i].location,
                   houses[i].price, houses[i].capacity);
            found = 1;
        }
    }
    if(!found) printf("No houses available!\n");
}

void searchHouses() {
    char location[50];
    int max_price;
    printf("Enter location (or '0' to skip): ");
    scanf("%s", location);
    printf("Enter max price (or 0 for any): ");
    scanf("%d", &max_price);

    printf("\nSEARCH RESULTS\n");
    printf("%-5s %-20s %-8s %-8s\n", "ID", "Location", "Price", "Capacity");
    printf("---------------------------------------------\n");

    int found = 0;
    for(int i = 0; i < house_count; i++) {
        if(houses[i].status == 0 &&
           (strcmp(location, "0") == 0 || strstr(houses[i].location, location)) &&
           (max_price == 0 || houses[i].price <= max_price)) {
            printf("%-5d %-20s %-8d %-8d\n",
                   houses[i].id, houses[i].location,
                   houses[i].price, houses[i].capacity);
            found = 1;
        }
    }
    if(!found) printf("No houses match your criteria!\n");
}

void bookHouse() {
    int house_id;
    char customer_name[50], phone[15], date[20];

    printf("Enter house ID: ");
    scanf("%d", &house_id);

    int index = findHouseById(house_id);
    if(index == -1) {
        printf("House not found!\n");
        return;
    }
    if(houses[index].status == 1) {
        printf("House already booked!\n");
        return;
    }

    printf("Enter your name: ");
    scanf("%s", customer_name);
    printf("Enter phone: ");
    scanf("%s", phone);
    printf("Enter booking date (DD/MM/YYYY): ");
    scanf("%s", date);

    bookings[booking_count].house_id = house_id;
    strcpy(bookings[booking_count].customer_name, customer_name);
    strcpy(bookings[booking_count].phone, phone);
    strcpy(bookings[booking_count].date, date);
    bookings[booking_count].booking_id = next_booking_id++;

    houses[index].status = 1;
    booking_count++;

    printf("Booking successful! Booking ID: %d\n", bookings[booking_count-1].booking_id);
}

void viewAllBookings() {
    if(booking_count == 0) {
        printf("No bookings yet!\n");
        return;
    }

    printf("\nALL BOOKINGS\n");
    printf("%-8s %-8s %-20s %-15s %-12s\n", "BookID", "HouseID", "Customer", "Phone", "Date");
    printf("--------------------------------------------------------------------\n");

    for(int i = 0; i < booking_count; i++) {
        printf("%-8d %-8d %-20s %-15s %-12s\n",
               bookings[i].booking_id, bookings[i].house_id,
               bookings[i].customer_name, bookings[i].phone,
               bookings[i].date);
    }
}

void viewMyBookings(char* customer_name) {
    int found = 0;
    printf("\nYOUR BOOKINGS\n");
    printf("%-8s %-8s %-15s %-12s\n", "BookID", "HouseID", "Phone", "Date");
    printf("----------------------------------------------------\n");

    for(int i = 0; i < booking_count; i++) {
        if(strcmp(bookings[i].customer_name, customer_name) == 0) {
            printf("%-8d %-8d %-15s %-12s\n",
                   bookings[i].booking_id, bookings[i].house_id,
                   bookings[i].phone, bookings[i].date);
            found = 1;
        }
    }
    if(!found) printf("No bookings found!\n");
}

void cancelBooking() {
    char customer_name[50];
    int booking_id;

    printf("Enter your name: ");
    scanf("%s", customer_name);
    printf("Enter booking ID: ");
    scanf("%d", &booking_id);

    for(int i = 0; i < booking_count; i++) {
        if(bookings[i].booking_id == booking_id &&
           strcmp(bookings[i].customer_name, customer_name) == 0) {

            int house_index = findHouseById(bookings[i].house_id);
            if(house_index != -1) {
                houses[house_index].status = 0;
            }

            for(int j = i; j < booking_count - 1; j++) {
                bookings[j] = bookings[j + 1];
            }
            booking_count--;
            printf("Booking cancelled successfully!\n");
            return;
        }
    }
    printf("Booking not found!\n");
}

void deleteHouse() {
    int id;
    printf("Enter house ID to delete: ");
    scanf("%d", &id);

    int index = findHouseById(id);
    if(index == -1) {
        printf("House not found!\n");
        return;
    }

    for(int i = index; i < house_count - 1; i++) {
        houses[i] = houses[i + 1];
    }
    house_count--;
    printf("House deleted successfully!\n");
}

void modifyHouse() {
    int id;
    printf("Enter house ID to modify: ");
    scanf("%d", &id);

    int index = findHouseById(id);
    if(index == -1) {
        printf("House not found!\n");
        return;
    }

    printf("New location: ");
    scanf("%s", houses[index].location);
    printf("New price: ");
    scanf("%d", &houses[index].price);
    printf("New capacity: ");
    scanf("%d", &houses[index].capacity);

    printf("House modified successfully!\n");
}

int findHouseById(int id) {
    for(int i = 0; i < house_count; i++) {
        if(houses[i].id == id) return i;
    }
    return -1;
}
