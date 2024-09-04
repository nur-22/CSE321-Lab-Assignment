#include <stdio.h>

typedef struct {
    int quantity;
    float unit_price;
} Item;

int main() {
    Item paratha, vegetable, mineral_water;
    
    printf("Quantity Of Paratha: ");
    scanf("%d", &paratha.quantity);
    printf("Unit Price: ");
    scanf("%f", &paratha.unit_price);
    printf("Quantity Of Vegetables: ");
    scanf("%d", &vegetable.quantity);
    printf("Unit Price: ");
    scanf("%f", &vegetable.unit_price);
    printf("Quantity Of Mineral Water: ");
    scanf("%d", &mineral_water.quantity);
    printf("Unit Price: ");
    scanf("%f", &mineral_water.unit_price);
    
    int num_people;
    printf("Number of People: ");
    scanf("%d", &num_people);
    
    float total_bill = (paratha.quantity * paratha.unit_price) + 
                       (vegetable.quantity * vegetable.unit_price) + 
                       (mineral_water.quantity * mineral_water.unit_price);
    float per_person_bill = total_bill / num_people;
    
    printf("Individual people will pay: %.2f tk\n", per_person_bill);

    return 0;
}
