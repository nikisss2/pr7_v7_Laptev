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

//поиск сеанса по названию
int findSession (Cinema* session, int count, std::string nameFilm)
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

void saveToFile (Cinema* session, int count)
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
        fileOut << session[i].getName() << ';'
                << session[i].getNumberRoom() << ';'
                << session[i].getTimeStart() << ';'
                << session[i].getCountPlace() << ';'
                << session[i].getCountBuyTickets() << ';'
                << session[i].getCostTickets() << std::endl;
    }

    fileOut.close();

#ifdef DEBUG_MODE
    std::cout << "Записано записей: " << count << std::endl;
#endif
}

int main ()
{
    Cinema* session = 0;
    int count = 0;
    int index = 0;

    try
    {
        std::fstream file ("data.txt", std::ios::in);

        if (!file.is_open())
        {
            throw std::runtime_error (
                "Не удалось открыть файл"
            );
        }

        std::string line;

        while (std::getline(file, line))
        {
            if (!line.empty())
            {
                count++;
            }
        }

        file.clear();
        file.seekg(0);

        session = new Cinema[count];
        std::string tempStr;

        while (std::getline(file, tempStr))
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
            session[index].setName(tempStr.substr(0, pos));
            tempStr.erase(0, pos + 1);

            pos = tempStr.find(';');
            if (pos == std::string::npos)
            {
                throw std::invalid_argument (
                    "Недостаточно данных в строке"
                );
            }
            session[index].setNumberRoom(std::stoi(tempStr.substr(0, pos)));
            tempStr.erase(0, pos + 1);

            pos = tempStr.find(';');
            if (pos == std::string::npos)
            {
                throw std::invalid_argument (
                    "Недостаточно данных в строке"
                );
            }
            session[index].setTimeStart(tempStr.substr(0, pos));
            tempStr.erase(0, pos + 1);

            pos = tempStr.find(';');
            if (pos == std::string::npos)
            {
                throw std::invalid_argument (
                    "Недостаточно данных в строке"
                );
            }
            session[index].setCountPlace(std::stoi(tempStr.substr(0, pos)));
            tempStr.erase(0, pos + 1);

            pos = tempStr.find(';');
            if (pos == std::string::npos)
            {
                throw std::invalid_argument (
                    "Недостаточно данных в строке"
                );
            }
            session[index].setCountBuyTickets(std::stoi(tempStr.substr(0, pos)));
            tempStr.erase(0, pos + 1);

            if (tempStr.empty())
            {
                throw std::invalid_argument (
                    "Недостаточно данных в строке"
                );
            }
            session[index].setCostTickets(std::stoi(tempStr));

            index++;
        }

        file.close();

#ifdef DEBUG_MODE
        std::cout << "Загружено записей: " << index << std::endl;
#endif

        int choice = -1;

        while (choice != 0)
        {
            std::cout << "\n1 - поиск сеанса по фильму" << std::endl;
            std::cout << "2 - сеансы со свободными местами" << std::endl;
            std::cout << "3 - продажа билетов" << std::endl;
            std::cout << "4 - выручка сеанса" << std::endl;
            std::cout << "5 - сохранить в файл" << std::endl;
            std::cout << "0 - выход" << std::endl;
            std::cout << "Выбор: ";

            std::cin >> choice;
            std::cin.ignore();

            std::string nameFilm;
            int number = 0;

            if (choice == 1)
            {
                std::cout << "Введите название фильма: ";
                std::getline(std::cin, nameFilm);

                number = findSession(session, index, nameFilm);

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
                for (int i = 0; i < index; i++)
                {
                    if (session[i].getFreePlace() > 0)
                    {
                        std::cout << "\nФильм: " << session[i].getName()<< " | Зал: " << session[i].getNumberRoom()<< " | Время: " << session[i].getTimeStart()<< " | Свободно: " << session[i].getFreePlace()
                                  << std::endl;
                    }
                }
            }
            else if (choice == 3)
            {
                std::cout << "Введите название фильма: ";
                std::getline(std::cin, nameFilm);

                number = findSession(session, index, nameFilm);

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

                number = findSession(session, index, nameFilm);

                if (number == -1)
                {
                    std::cout << "Сеанс не найден" << std::endl;
                } else
                {
                    std::cout << "Выручка сеанса: "
                              << session[number].getRevenue() << std::endl;
                }
            }
            else if (choice == 5)
            {
                saveToFile(session, index);
            }
        }

        delete[] session;
    }
    catch (const std::invalid_argument& e)
    {
        std::cout << "Ошибка: " << e.what() << std::endl;
        delete[] session;
    }
    catch (const std::out_of_range& e)
    {
        std::cout << "Ошибка: " << e.what() << std::endl;
        delete[] session;
    }
    catch (const std::exception& e)
    {
        std::cout << "Ошибка: " << e.what() << std::endl;
        delete[] session;
    }

    return 0;
}