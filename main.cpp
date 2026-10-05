#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>

#define DEBUG_MODE

class Cinema {
    private:
        std::string name;
        int numberRoom;
        std::string timeStart;
        int countPlace;
        int countBuyTickets;
        int costTickets;

    public:
        std::string getName () {return name;}
        void setName (std::string _name)
        {
            if (!_name.empty())
            {
                name = _name;
            } else
            {
                throw std::invalid_argument (
                    "Строка не может быть пустой"
                );
            }
        }

        int getNumberRoom () {return numberRoom;}
        void setNumberRoom (int _numberRoom)
        {
            if (_numberRoom > 0)
            {
                numberRoom = _numberRoom;
            } else
            {
                throw std::invalid_argument (
                    "Номер зала должен быть больше 0"
                );
            }
        }

        std::string getTimeStart () {return timeStart;}
        void setTimeStart (std::string _timeStart)
        {
            if (!_timeStart.empty())
            {
                timeStart = _timeStart;
            } else
            {
                throw std::invalid_argument (
                    "Время не может быть пустым"
                );
            }
        }

        int getCountPlace () {return countPlace;}
        void setCountPlace (int _countPlace)
        {
            if (_countPlace > 0)
            {
                countPlace = _countPlace;
            } else
            {
                throw std::invalid_argument (
                    "Количество мест должно быть больше 0"
                );
            }
        }

        int getCountBuyTickets () {return countBuyTickets;}
        void setCountBuyTickets (int _countBuyTickets)
        {
            if (_countBuyTickets >= 0 && _countBuyTickets <= countPlace)
            {
                countBuyTickets = _countBuyTickets;
            } else
            {
                throw std::out_of_range (
                    "Количество проданных билетов некорректно"
                );
            }
        }

        int getCostTickets () {return costTickets;}
        void setCostTickets (int _costTickets)
        {
            if (_costTickets > 0)
            {
                costTickets = _costTickets;
            } else
            {
                throw std::invalid_argument (
                    "Стоимость билета должна быть больше 0"
                );
            }
        }

        int getFreePlace () {return countPlace - countBuyTickets;}

        int getRevenue () {return countBuyTickets * costTickets;}

        //продажа билетов
        void sellTickets (int _countTickets)
        {
            if (_countTickets <= 0)
            {
                throw std::invalid_argument (
                    "Количество билетов должно быть больше 0"
                );
            }
            if (_countTickets > getFreePlace())
            {
                throw std::out_of_range (
                    "Недостаточно свободных мест"
                );
            }
            countBuyTickets = countBuyTickets + _countTickets;
        }

        void printInfo ()
        {
            std::cout << "Фильм: " << name << std::endl;
            std::cout << "Номер зала: " << numberRoom << std::endl;
            std::cout << "Время начала: " << timeStart << std::endl;
            std::cout << "Количество мест: " << countPlace << std::endl;
            std::cout << "Продано билетов: " << countBuyTickets << std::endl;
            std::cout << "Свободных мест: " << getFreePlace() << std::endl;
            std::cout << "Стоимость билета: " << costTickets << std::endl;
            std::cout << "Выручка: " << getRevenue() << std::endl;
        }
};

//поиск сеанса
int findSession (Cinema session[], int count, std::string nameFilm)
{
    for (int i = 0; i < count; i++)
    {
        if (session[i].getName() == nameFilm)
        {
            return i;
        }
    }
    return -1;
}

int loadFromFile (Cinema session[])
{
    std::ifstream fileIn ("data.txt");

    if (!fileIn.is_open())
    {
        throw std::runtime_error (
            "Не удалось открыть файл"
        );
    }

    int count = 0;
    std::string tempStr;

    while (std::getline(fileIn, tempStr))
    {
        if (tempStr.empty())
        {
            continue;
        }

        std::size_t pos = 0;

        pos = tempStr.find(';');
        if (pos == std::string::npos)
        {
            throw std::invalid_argument (
                "Недостаточно данных в строке"
            );
        }
        session[count].setName(tempStr.substr(0, pos));
        tempStr.erase(0, pos + 1);

        pos = tempStr.find(';');
        if (pos == std::string::npos)
        {
            throw std::invalid_argument (
                "Недостаточно данных в строке"
            );
        }
        session[count].setNumberRoom(std::stoi(tempStr.substr(0, pos)));
        tempStr.erase(0, pos + 1);

        pos = tempStr.find(';');
        if (pos == std::string::npos)
        {
            throw std::invalid_argument (
                "Недостаточно данных в строке"
            );
        }
        session[count].setTimeStart(tempStr.substr(0, pos));
        tempStr.erase(0, pos + 1);

        pos = tempStr.find(';');
        if (pos == std::string::npos)
        {
            throw std::invalid_argument (
                "Недостаточно данных в строке"
            );
        }
        session[count].setCountPlace(std::stoi(tempStr.substr(0, pos)));
        tempStr.erase(0, pos + 1);

        pos = tempStr.find(';');
        if (pos == std::string::npos)
        {
            throw std::invalid_argument (
                "Недостаточно данных в строке"
            );
        }
        session[count].setCountBuyTickets(std::stoi(tempStr.substr(0, pos)));
        tempStr.erase(0, pos + 1);

        if (tempStr.empty())
        {
            throw std::invalid_argument (
                "Недостаточно данных в строке"
            );
        }
        session[count].setCostTickets(std::stoi(tempStr));

        count++;
    }

    fileIn.close();

#ifdef DEBUG_MODE
    std::cout << "Загружено записей: " << count << std::endl;
#endif

    return count;
}

void saveToFile (Cinema session[], int count)
{
    std::ofstream fileOut ("data.txt");

    if (!fileOut.is_open())
    {
        throw std::runtime_error (
            "Не удалось открыть файл для записи"
        );
    }

    for (int i = 0; i < count; i++)
    {
        fileOut << session[i].getName() << ';' << session[i].getNumberRoom() << ';' << session[i].getTimeStart() << ';' << session[i].getCountPlace() << ';' << session[i].getCountBuyTickets() << ';' << session[i].getCostTickets() << std::endl;
    }

    fileOut.close();

#ifdef DEBUG_MODE
    std::cout << "Записано записей: " << count << std::endl;
#endif
}

int addSession (Cinema session[], int count, int maxCount)
{
    if (count >= maxCount)
    {
        throw std::out_of_range (
            "Достигнут максимум сеансов"
        );
    }

    std::string tempStr;
    int tempInt = 0;

    std::cout << "Введите название фильма: ";
    std::getline(std::cin, tempStr);
    session[count].setName(tempStr);

    std::cout << "Введите номер зала: ";
    std::cin >> tempInt;
    session[count].setNumberRoom(tempInt);
    std::cin.ignore();

    std::cout << "Введите время начала: ";
    std::getline(std::cin, tempStr);
    session[count].setTimeStart(tempStr);

    std::cout << "Введите количество мест: ";
    std::cin >> tempInt;
    session[count].setCountPlace(tempInt);

    std::cout << "Введите количество проданных билетов: ";
    std::cin >> tempInt;
    session[count].setCountBuyTickets(tempInt);

    std::cout << "Введите стоимость билета: ";
    std::cin >> tempInt;
    session[count].setCostTickets(tempInt);
    std::cin.ignore();

    count++;

    std::cout << "Сеанс добавлен" << std::endl;

    return count;
}

int deleteSession (Cinema session[], int count)
{
    if (count == 0)
    {
        throw std::out_of_range (
            "Список сеансов пуст"
        );
    }

    std::string nameFilm;
    std::cout << "Введите название фильма для удаления: ";
    std::getline(std::cin, nameFilm);

    int number = findSession(session, count, nameFilm);

    if (number == -1)
    {
        std::cout << "Сеанс не найден" << std::endl;
        return count;
    }

    session[number].printInfo();
    std::cout << "Удалить этот сеанс? (1 - да, 0 - нет): ";

    int confirm = 0;
    std::cin >> confirm;
    std::cin.ignore();

    if (confirm != 1)
    {
        std::cout << "Удаление отменено" << std::endl;
        return count;
    }

    for (int i = number; i < count - 1; i++)
    {
        session[i] = session[i + 1];
    }

    count--;

    session[count].setName("удалено");
    session[count].setNumberRoom(1);
    session[count].setTimeStart("00:00");
    session[count].setCountPlace(1);
    session[count].setCountBuyTickets(0);
    session[count].setCostTickets(1);

    std::cout << "Сеанс удалён" << std::endl;

    return count;
}

int main ()
{
    const int MAX_COUNT = 100;
    Cinema session[MAX_COUNT];
    int count = 0;

    try
    {
        count = loadFromFile(session);

        int choice = -1;

        while (choice != 0)
        {
            std::cout << "\n1 - поиск сеанса по фильму" << std::endl;
            std::cout << "2 - сеансы со свободными местами" << std::endl;
            std::cout << "3 - продажа билетов" << std::endl;
            std::cout << "4 - выручка сеанса" << std::endl;
            std::cout << "5 - добавить сеанс" << std::endl;
            std::cout << "6 - удалить сеанс" << std::endl;
            std::cout << "7 - сохранить в файл" << std::endl;
            std::cout << "0 - выход" << std::endl;
            std::cout << "Выбор: ";

            std::cin >> choice;
            std::cin.ignore();

            std::string nameFilm;
            int number = -1;

            if (choice == 1)
            {
                std::cout << "Введите название фильма: ";
                std::getline(std::cin, nameFilm);

                number = findSession(session, count, nameFilm);

                if (number == -1)
                {
                    std::cout << "Сеанс не найден" << std::endl;
                } else
                {
                    session[number].printInfo();
                }
            }
            else if (choice == 2)
            {
                for (int i = 0; i < count; i++)
                {
                    if (session[i].getFreePlace() > 0)
                    {
                        std::cout << "\nФильм: " << session[i].getName() << "\nЗал: " << session[i].getNumberRoom() << "\nВремя: " << session[i].getTimeStart() << "\nСвободно: " << session[i].getFreePlace() << std::endl;
                    }
                }
            }
            else if (choice == 3)
            {
                std::cout << "Введите название фильма: ";
                std::getline(std::cin, nameFilm);

                number = findSession(session, count, nameFilm);

                if (number == -1)
                {
                    std::cout << "Сеанс не найден" << std::endl;
                } else
                {
                    std::cout << "Сколько билетов продать: ";
                    int ticketCount = 0;
                    std::cin >> ticketCount;
                    std::cin.ignore();

                    session[number].sellTickets(ticketCount);
                    std::cout << "Билеты проданы" << std::endl;
                }
            }
            else if (choice == 4)
            {
                std::cout << "Введите название фильма: ";
                std::getline(std::cin, nameFilm);

                number = findSession(session, count, nameFilm);

                if (number == -1)
                {
                    std::cout << "Сеанс не найден" << std::endl;
                } else
                {
                    std::cout << "Выручка сеанса: " << session[number].getRevenue() << std::endl;
                }
            }
            else if (choice == 5)
            {
                count = addSession(session, count, MAX_COUNT);
            }
            else if (choice == 6)
            {
                count = deleteSession(session, count);
            }
            else if (choice == 7)
            {
                saveToFile(session, count);
            }
        }
    }
    catch (const std::invalid_argument e)
    {
        std::cout << "Ошибка: " << e.what() << std::endl;
    }
    catch (const std::out_of_range e)
    {
        std::cout << "Ошибка: " << e.what() << std::endl;
    }
    catch (const std::exception e)
    {
        std::cout << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}