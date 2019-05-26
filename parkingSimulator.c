#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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
  Time t1; // Creats first time object
  t1.hours = c -> enteringTime.hours;
  t1.minute = c -> enteringTime.minute;

  Time t2; // Creates second time object
  t2.hours = hour;
  t2.minute = min;

  Time *diff = malloc(sizeof(Time)); // Creates dynamically allocated Time pointer

  difference(t1, t2, diff); // Calls + finds differnece in the two times

  if (!(c -> hasAPermit)){ // Does not have permit
    if (diff -> minute != 0){ // Checks if extra hour needs to be added
      if (((diff -> hours) + 1) * (p -> hourlyRate) > p -> maxCharge){ // Finding best rate for the car
        printf("Car %s leaves Lot %d at %d:%.2d paid $%4.2f.\n", c -> plateNumber, p -> lotNumber, hour, min, p -> maxCharge);
        p -> revenue += p -> maxCharge; // Adds to revenue
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
  p -> currentCarCount -= 1; // Decreaes car count in lot
}

int main() {
  Car  car1, car2, car3, car4, car5, car6, car7, car8, car9;
  ParkingLot p1, p2;

  // Set up 9 cars
  initializeCar(&car1, "ABC 123", 0);
  initializeCar(&car2, "ABC 124", 0);
  initializeCar(&car3, "ABD 314", 0);
  initializeCar(&car4, "ADE 901", 0);
  initializeCar(&car5, "AFR 304", 0);
  initializeCar(&car6, "AGD 888", 0);
  initializeCar(&car7, "AAA 111", 0);
  initializeCar(&car8, "ABB 001", 0);
  initializeCar(&car9, "XYZ 678", 1);

  // Set up two parking lots
  initializeLot(&p1, 1, 4, 5.5, 20.0);
  initializeLot(&p2, 2, 6, 3.0, 12.0);

  printLotInfo(p1);
  printLotInfo(p2);
  printf("\n");

  // Simulate cars entering the lots
  carEnters(&p1, &car1, 7, 15);
  carEnters(&p1, &car2, 7, 25);
  carEnters(&p2, &car3, 8,  0);
  carEnters(&p2, &car4, 8, 10);
  carEnters(&p1, &car5, 8, 15);
  carEnters(&p1, &car6, 8, 20);
  carEnters(&p1, &car7, 8, 30);
  carEnters(&p2, &car7, 8, 32);
  carEnters(&p2, &car8, 8, 50);
  carEnters(&p2, &car9, 8, 55);

  printf("\n");
  printLotInfo(p1);
  printLotInfo(p2);
  printf("\n");

  // Simulate cars leaving the lots
  carLeaves(&p2, &car4, 9, 0);
  carLeaves(&p1, &car2, 9, 5);
  carLeaves(&p1, &car6, 10, 0);
  carLeaves(&p1, &car1, 10, 30);
  carLeaves(&p2, &car8, 13, 0);
  carLeaves(&p2, &car9, 15, 15);
  carEnters(&p1, &car8, 17, 10);
  carLeaves(&p1, &car5, 17, 50);
  carLeaves(&p2, &car7, 18, 0);
  carLeaves(&p2, &car3, 18, 15);
  carLeaves(&p1, &car8, 20, 55);

  printf("\n");
  printLotInfo(p1);
  printLotInfo(p2);
  printf("\n");

  // Display the total revenue
  printf("Total revenue of Lot 1 is $%4.2f\n", p1.revenue);
  printf("Total revenue of Lot 2 is $%4.2f\n", p2.revenue);
}
