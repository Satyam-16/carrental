#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

class CarRentalSystem {
public:
    CarRentalSystem() {
        driver = get_driver_instance();
        conn = driver->connect("tcp://127.0.0.1:3306", "your_username", "your_password");
        conn->setSchema("car_rental");
    }

    ~CarRentalSystem() {
        delete conn;
    }

    void addCar(const std::string& make, const std::string& model, int year) {
        sql::PreparedStatement *prep_stmt = conn->prepareStatement("INSERT INTO Cars (make, model, year) VALUES (?, ?, ?)");
        prep_stmt->setString(1, make);
        prep_stmt->setString(2, model);
        prep_stmt->setInt(3, year);
        prep_stmt->execute();
        delete prep_stmt;
        std::cout << "Car added: " << make << " " << model << " (" << year << ")" << std::endl;
    }

    void rentCar(int carId) {
        sql::PreparedStatement *prep_stmt = conn->prepareStatement("UPDATE Cars SET isRented = 1 WHERE id = ?");
        prep_stmt->setInt(1, carId);
        prep_stmt->execute();
        delete prep_stmt;
        std::cout << "Car with ID " << carId << " rented." << std::endl;
    }

    void returnCar(int carId) {
        sql::PreparedStatement *prep_stmt = conn->prepareStatement("UPDATE Cars SET isRented = 0 WHERE id = ?");
        prep_stmt->setInt(1, carId);
        prep_stmt->execute();
        delete prep_stmt;
        std::cout << "Car with ID " << carId << " returned." << std::endl;
    }

    void listAvailableCars() {
        sql::Statement *stmt = conn->createStatement();
        sql::ResultSet *res = stmt->executeQuery("SELECT * FROM Cars WHERE isRented = 0");
        
        std::cout << "Available Cars:" << std::endl;
        while (res->next()) {
            int id = res->getInt("id");
            std::string make = res->getString("make");
            std::string model = res->getString("model");
            int year = res->getInt("year");
            std::cout << "ID: " << id << ", Make: " << make << ", Model: " << model << ", Year: " << year << std::endl;
        }

        delete res;
        delete stmt;
    }

private:
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *conn;
};

int main() {
    CarRentalSystem rentalSystem;
    
    rentalSystem.addCar("Toyota", "Camry", 2020);
    rentalSystem.addCar("Honda", "Accord", 2019);
    
    rentalSystem.listAvailableCars();
    
    rentalSystem.rentCar(1);
    
    rentalSystem.listAvailableCars();
    
    rentalSystem.returnCar(1);
    
    rentalSystem.listAvailableCars();

    return 0;
}
