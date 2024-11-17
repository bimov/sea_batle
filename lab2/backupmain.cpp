#include <iostream>
#include <vector>
using namespace std;
#include "field.h"
#include "ship.h"
#include "manager.h"
#include "ability_manager.h"


int main(){
    int size;
    while(1){
        try{
        cout << "Введите размер поля: ";
        cin >> size;
        if (cin.fail() || size < 2 || size > 20) {
            throw IncorrectFieldSize("Ошибка ввода! Размер поля - это число от 2 до 20.");
        }
        break;
        } catch(IncorrectFieldSize& e){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << e.what() << '\n';
        }
    }
    int ships[4];
    int x, y;
    int orientation;
    int flag_error;
    string flag_ability;
    cout << "Игра морской бой начинается." << '\n';
    for(int i = 0; i < 4; i++){
        while(1){
            try{
            cout << "Введите количество кораблей. Длины " << i + 1 << ":"<< '\n';
            cin >> ships[i];
            if (cin.fail() || ships[i] < 0 || ships[i] > 10) {
                throw IncorrectQuantity("Ошибка ввода! Количество кораблей число от 0 до 10.");
            }
            break;
            } catch(IncorrectQuantity& e){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                cout << e.what() << '\n';
            }
        }
    }
    int qantity_ship = ships[0] + ships[1] + ships[2] + ships[3];
    Manager managers(qantity_ship, ships);
    Field userfield(size, &managers);
    userfield.create_field();
    for(int j = 0; j < 4; j++){
        if(ships[j] > 0){
        cout << "Введите координаты и ориентацию кораблей в формате x y 0, где 0 обозначет горизатльное расположение, а 1 вертикальное. " << "Длина: " << j + 1 << '\n';
        }
        for(int i = 0; i < ships[j]; i++){
            cout << "x y orientation: ";
            cin >> x >> y >> orientation;
            flag_error = 0;
            while(1){
                try{
                    if (cin.fail()) {
                        throw invalid_argument("Ошибка ввода! Пожалуйста, введите целые числа.");
                    }
                    if(flag_error == 0){
                        managers.create_ship(j+1, orientation, x, y);
                        userfield.put_ship();
                    }
                    else{
                        Ship& current_ship = managers.getship();
                        current_ship.setorientation(orientation);
                        current_ship.input_coordinates(x, y);
                        userfield.put_ship();
                    }
                    break;
                } catch(IncorrectCoordinatesException& e){
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << e.what() << '\n';
                    cout << "x y orientation: ";
                    cin >> x >> y >> orientation;
                    flag_error = 1;
                }
                catch(PlaceShipException& e){
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << e.what() << e.getxerror() << ' ' << e.getyerror() << '\n';
                    cout << "x y orientation: ";
                    cin >> x >> y >> orientation;
                    flag_error = 1;
                }
                catch(invalid_argument& e){
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << e.what() << '\n';
                    cout << "x y orientation: ";
                    cin >> x >> y >> orientation;
                }
            }
        }
    }
    AbilityManager abilitymanager(&userfield, &managers);
    int number_ships = managers.getnumbership();
    while(1){
        if (abilitymanager.is_empty() == false){
            cout << "Чтобы активировать спобность введите Y\n";
            cin >> flag_ability;
            if(flag_ability == "Y"){
                cout << "Активирована способность.\n";
                abilitymanager.apply_ability();
            }
            else{
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            }
        }
        cout << "Введите координаты для атаки." << '\n';
        cout << "x y: ";
        cin >> x >> y;
        while(1){
            try{
                if (cin.fail()) {
                    throw IncorrectCoordinatesException("Ошибка ввода! Пожалуйста, введите целые числа.");
                }
                userfield.attack_segment(x, y);
                break;
            } catch(IncorrectCoordinatesException& e){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << e.what() << '\n';
                cout << "x y: ";
                cin >> x >> y;
            }
        }
        userfield.print_field_user();
        cout << endl;
        userfield.print_field_opponent();
        if(number_ships - managers.getnumbership() >= 1){
            abilitymanager.add_ability();
            number_ships = managers.getnumbership();
        }
    }
    return 0;
}



//нужно считывать x y в main, а не в catch, переделать setcoordinates из virtual в обычный метод, считывать x y в main, а не abilitymanager