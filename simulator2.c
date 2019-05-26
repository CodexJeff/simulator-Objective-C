#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

typedef struct { // Structure that represents an hour and minute amount
  int hours;
  int minute;
} Time;

typedef struct { // Structure that represents car details
  char *plateNumber;
  char hasAPermit;
  Time enteringTime;
  int lotParkedIn;
} Car;

typedef struct { // Structure that represents parking lot details
  int lotNumber;
  double hourlyRate;
  double maxCharge;
  int capacity;
  int currentCarCount;
  double revenue;
} ParkingLot;

// Sets the hours and minutes amount for the given time t based
// on the specified hours h.  (e.g., 1.25 hours would be 1 hour
// and 15 minutes)
void setHours(Time *t, double h) {
  double n = h - (int) h;
  if(h >= 1){
    t -> hours = (int) h;
    t -> minute = (int)(n * 60);
  } else{
    t -> minute = (int)(h * 60);
  }
}

// Takes two Time objects (not pointers) and computes the difference
// in time from t1 to t2 and then stores that difference in the diff
// Time (which must be a pointer)
void difference(Time t1, Time t2, Time *diff) {
  double set = (t2.hours + ((double) t2.minute/60)) - (t1.hours + ((double) t1.minute/60));
  setHours(diff, set);
}

// Initialize the car pointed to by c to have the given plate and
// hasPermit status.  The car should have it’s lotParkedIn set to
// 0 and enteringTime to be -1 hours and -1 minutes.
void initializeCar(Car *c, char *plate, char hasPermit) {
  c -> plateNumber = plate;
  c -> hasAPermit = hasPermit;
  c -> lotParkedIn = 0;
  c -> enteringTime.hours = -1;
  c -> enteringTime.minute = -1;
}

// Initialize the lot pointed to by p to have the given number,
// capacity, hourly rate and max charge values.  The currentCarCount
// and revenue should be at 0.
void initializeLot(ParkingLot *p, int num, int cap, double rate, double max) {
  p -> lotNumber= num;
  p -> capacity = cap;
  p -> hourlyRate = rate;
  p -> maxCharge = max;
  p -> currentCarCount = 0;
  p -> revenue = 0;
}

// Print out the parking lot parameters so that is displays as
// follows:   Parking Lot #2 - rate = $3.00, capacity 6, current cars 5
void printLotInfo(ParkingLot p) {
  printf("Parking Lot #%d - rate = $%4.2f, capacity %d, current cars %d\n", p.lotNumber, p.hourlyRate, p.capacity, p.currentCarCount);
}

// Simulate a car entering the parking lot
// Setting car details
void carEnters(ParkingLot *p, Car *c, int hour, int min){
  if (p -> currentCarCount < p -> capacity){
    p -> currentCarCount += 1;
    c -> enteringTime.hours = hour;
    c -> enteringTime.minute = min;
    c -> lotParkedIn = p -> lotNumber;
    printf("Car %s enters Lot %d at %d:%.2d.\n", c -> plateNumber, c -> lotParkedIn, hour, min);
  } else{
    printf("Car %s arrives at Lot %d at %d:%.2d, but the lot is full.\nCar %s cannot get in.\n", c -> plateNumber, p -> lotNumber, hour, min, c -> plateNumber);
  }
}

// Simulate a car leaving the parking lot
void carLeaves(ParkingLot *p, Car *c, int hour, int min){
  Time t1; // Creating first time object
  t1.hours = c -> enteringTime.hours;
  t1.minute = c -> enteringTime.minute;

  Time t2; // Creating second time object
  t2.hours = hour;
  t2.minute = min;

  Time *diff = malloc(sizeof(Time)); // Creating dynamically allocated Time pointer

  difference(t1, t2, diff); // Call + finds differnece in the two times

  if (!(c -> hasAPermit)){ // Does not have permit
    if (diff -> minute != 0){ // Checks if extra hour needs to be added
      if (((diff -> hours) + 1) * (p -> hourlyRate) > p -> maxCharge){ // Finding best rate for the car
        printf("Car %s leaves Lot %d at %d:%.2d paid $%4.2f.\n", c -> plateNumber, p -> lotNumber, hour, min, p -> maxCharge);
        p -> revenue += p -> maxCharge; // Adding to revenue
      } else{
        double cost = ((diff -> hours) + 1) * (p -> hourlyRate);
        printf("Car %s leaves Lot %d at %d:%.2d paid $%4.2f.\n", c -> plateNumber, p -> lotNumber, hour, min, cost);
        p -> revenue += cost;
      }
    } else{
      if ((diff -> hours) * (p -> hourlyRate) > p -> maxCharge){
        printf("Car %s leaves Lot %d at %d:%.2d paid $%4.2f.\n", c -> plateNumber, p -> lotNumber, hour, min, p -> maxCharge);
        p -> revenue += p -> maxCharge;
      } else{
        printf("Car %s leaves Lot %d at %d:%.2d paid $%4.2f.\n", c -> plateNumber, p -> lotNumber, hour, min, (diff -> hours) * (p -> hourlyRate));
        p -> revenue += (((diff -> hours)) * (p -> hourlyRate));
      }
    }
  } else{
    printf("Car %s leaves Lot %d at %d:%.2d.\n", c -> plateNumber, p -> lotNumber, hour, min);
  }
  p -> currentCarCount -= 1; // Decreasing car count in lot
}

// Generates a random car plate
char *randomPlate(){
  char *plate = NULL;
  plate = malloc(sizeof(char) * 7); // Creates dynamically allocated plate pointer

  for (int i = 0; i < 4; i++) { // Creates first 3 letters of plate
    plate[i] = 65 + (rand() % 26);
  }
  plate[3] = ' ';
  for (int j = 4; j < 7; j++) { // Creates last 3 numbers of plate
    plate[j] = 48 + (rand() % 10);
  }
  plate[7] = '\0'; // Stops symbols from appearing
  return plate;
}

// Generates a random plated car
Car *randomCar(){
  Car *randCar = malloc(sizeof(Car)); // Creates dynamically allocated randCar Car
  initializeCar(randCar, randomPlate(), rand() % 2); // Initializes the car
  return randCar;
}

int main() {

  srand((unsigned int)time(NULL)); // Random based on time
  Car *cars [50];
  ParkingLot **parking;
  parking = malloc(sizeof(ParkingLot)); // Creates dynamically allocated parking ParkingLot
  ParkingLot p1, p2, p3, p4, p5; // Creates parking lot objects

  for (int i = 0; i < 50; i++){ // Generating 50 random cars
    cars[i] = randomCar();
    printf("Car %s with permit %d\n", cars[i] -> plateNumber, (int) cars[i] -> hasAPermit);
  }

  // Set up five parking lots
  initializeLot(&p1, 1, 5, 4.0, 12.0);
  initializeLot(&p2, 2, 10, 5.0, 14.0);
  initializeLot(&p3, 3, 15, 6.0, 16.0);
  initializeLot(&p4, 4, 20, 7.0, 18.0);
  initializeLot(&p5, 5, 25, 8.0, 20.0);

  // Sets the parking lot objects to the array
  parking[0] = &p1;
  parking[1] = &p2;
  parking[2] = &p3;
  parking[3] = &p4;
  parking[4] = &p5;

  // Prints parking lot info for each lot
  printf("\n");
  printLotInfo(p1);
  printLotInfo(p2);
  printLotInfo(p3);
  printLotInfo(p4);
  printLotInfo(p5);
  printf("\n");

  int m = 0, h = 6;
  for (int i = 0; i < 50; i++){ // Puts random cars in random parking spots
    carEnters(parking[rand() % 5], cars[i], h, m);
    m += 5;
    if (m == 60){
      m = 0;
      h += 1;
    }
  }

  // Prints parking lot info for each lot
  printf("\n");
  printLotInfo(p1);
  printLotInfo(p2);
  printLotInfo(p3);
  printLotInfo(p4);
  printLotInfo(p5);
  printf("\n");

  int min = 0, hour = 11;
  for (int i = 0; i < 50; i++){
    if (cars[i] -> lotParkedIn !=0){  // Checks fors cars in the lot
      carLeaves(parking[(cars[i] -> lotParkedIn) -1], cars[i], hour, min); // Simulates cars leaving in parking lots from all parking lots
      min += 5;
      if (min == 60){
        min = 0;
        hour += 1;
      }
    }
  }

  // Prints parking lot info for each lot
  printf("\n");
  printLotInfo(p1);
  printLotInfo(p2);
  printLotInfo(p3);
  printLotInfo(p4);
  printLotInfo(p5);
  printf("\n");

  // Displays the total revenue for each lot
  printf("Total revenue of Lot 1 is $%4.2f\n", p1.revenue);
  printf("Total revenue of Lot 2 is $%4.2f\n", p2.revenue);
  printf("Total revenue of Lot 3 is $%4.2f\n", p3.revenue);
  printf("Total revenue of Lot 4 is $%4.2f\n", p4.revenue);
  printf("Total revenue of Lot 5 is $%4.2f\n", p5.revenue);

  free (parking); //Free in HEAP
}
