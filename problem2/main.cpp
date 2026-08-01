#include <iostream>
#include <unordered_map>
#include <memory>
class Vehicle{
    protected:
        std::string licensePlate;

    public:
        Vehicle(std::string plate) : licensePlate(plate){}
        virtual ~Vehicle() = default;
        virtual double calculateFee(int hoursStayed) const = 0;
        std::string getPlate() const {return licensePlate;}
};  

class Bike : public Vehicle{
    public:
        Bike(std::string plate) : Vehicle(plate) {}
        double calculateFee(int hoursStayed) const override{
            if(hoursStayed<=1) return 20.0;
            return 20.0 + ((hoursStayed-1)*10.0);
        }
};

class Car : public Vehicle{
    public:
        Car(std::string plate) : Vehicle(plate){}
        double calculateFee(int hoursStayed) const override{
            if(hoursStayed<=1) return 50.0;
            return 50.0 + ((hoursStayed-1)*30.0);
        }
};

class ElectricCar : public Car{
    public:
        ElectricCar(std::string plate) : Car(plate){}
        double calculateFee(int hoursStayed) const override{
            return Car::calculateFee(hoursStayed) * 0.8;
        }
};

class Ticket{
    private:
        std::unique_ptr<Vehicle> vehicle;
        int entryHour;
    public:
        Ticket(std::unique_ptr<Vehicle> v, int entry) : vehicle(std::move(v)), entryHour(entry){}
        double processExit(int exitHour) const{
            int duration = std::max(1,exitHour-entryHour);
            return vehicle->calculateFee(duration);
        }
        int getEntryHour() const{ return entryHour; }
};

class ParkingGate{
    private:
        std::unordered_map<std::string, std::unique_ptr<Ticket>> activeTickets;
    public:
        void handleEntry(std::unique_ptr<Vehicle> v, int entryHour){
            std::string plate = v->getPlate();
            activeTickets[plate] = std::make_unique<Ticket>(std::move(v),entryHour);
            std::cout << "[Entry] Vehicle " << plate << "' entered at hour " << entryHour << ". Ticket issued. \n"; 
        }
        void handleExit(const std::string &plate, int exitHour){
            auto it = activeTickets.find(plate);
            if(it==activeTickets.end()){
                std::cout << "Error: No active ticket founf for this plate " << plate << " .\n";
                return;
            }
            int duration = std::max(1,exitHour-it->second->getEntryHour());
            double totalBill = it->second->processExit(exitHour);
            std::cout << "\n[Exit] Vehicle '" << plate << "' exiting at hour " << exitHour << ".\n";
            std::cout << "-> Stay duration: " << duration << " hour(s).\n";
            std::cout << "-> Total Bill: $" << totalBill << "\n";
            activeTickets.erase(it);
        }
};

int main(){
    ParkingGate gate;

    // Vehicles enter. Gate takes ownership using std::move via make_unique
    gate.handleEntry(std::make_unique<Bike>("MH-01-AB-1234"), 10);
    gate.handleEntry(std::make_unique<Car>("MH-04-XX-9999"), 11);
    
    // Testing the extensibility requirement
    gate.handleEntry(std::make_unique<ElectricCar>("MH-12-EV-7777"), 11);

    // Vehicles exit
    gate.handleExit("MH-01-AB-1234", 13); // Bike: 3 hours = $40
    gate.handleExit("MH-04-XX-9999", 12); // Car: 1 hour = $50
    gate.handleExit("MH-12-EV-7777", 13); // EV: 2 hours (50 + 30) * 0.8 = $64
    return 0;
}
