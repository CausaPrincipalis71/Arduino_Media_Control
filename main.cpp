#include <QCoreApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>

class Mouse                                                     // Класс управления мышкой
{
    public:
        void up()                                               // Мышку вверх
        {
            updateMouse();                                      // Получаем координаты
            int newMouseY = mouseY - step;                      // Высчитываем координаты
            if(newMouseY < 0)                                   // Если они меньше нуля
                newMouseY = 0;                                  // То поставить их на ноль
            QString command = "xdotool mousemove ";             // Указание xdotool что делать
            command += QString::number(mouseX);                 // Координаты по x неизменные
            command += " ";
            command += QString::number(newMouseY);              // Координата по y уменьшается на шаг
            system(command.toStdString().c_str());              // Вызов программы xdotools
        }

        void down()                                             // Мышку вниз
        {
            updateMouse();                                      // Получаем координаты
            QString command = "xdotool mousemove ";             // Указание xdotool что делать
            command += QString::number(mouseX);                 // Координаты по x неизменные
            command += " ";
            command += QString::number(mouseY + step);          // Координата по y увеличивается на шаг
            system(command.toStdString().c_str());              // Вызов программы xdotools
        }

        void right()                                            // Мышку вправо
        {
            updateMouse();                                      // Получаем координаты
            QString command = "xdotool mousemove ";             // Указание xdotool что делать
            command += QString::number(mouseX + step);          // Координата по x увеличивается на шаг
            command += " ";
            command += QString::number(mouseY);                 // Координаты по y неизменны
            system(command.toStdString().c_str());              // Вызов xdotools
        }

        void left()                                             // Мышку влево
        {
            updateMouse();                                      // Получаем координаты
            int newMouseX = mouseX - step;                      // Высчитываем координаты
            if(newMouseX < 0)                                   // Если они меньше нуля
                newMouseX = 0;                                  // То поставить их на ноль
            QString command = "xdotool mousemove ";             // Указание xdotool что делать
            command += QString::number(newMouseX);              // Координата по x уменьшается на шаг
            command += " ";
            command += QString::number(mouseY);                 // Координаты по y неизменны
            system(command.toStdString().c_str());              // Вызов xdotools
        }

        void leftMouseClick()
        {
            system("xdotool click 1");                          // Вызов xdotools с командой одного нажатия мышью
        }

        void faster()                                           // Увеличения шага
        {
            step += 2;
        }

        void slower()                                           // Уменьшение
        {
            step -= 2;
        }

        /* Геттеры, не используются*/
        int getMouseX()
        {
            return mouseX;
        }
        int getMouseY()
        {
            return mouseY;
        }
    private:
        int mouseX, mouseY;                                     // Координаты

        int step = 25;                                          // Шаг в пикселях

        QString getMouseLocation()                              // Получение строки с положением мыши
        {
            QString mouse_location;                             // Строка, содержащяя данные
            FILE* sensors = popen("xdotool getmouselocation", "r"); // Открыть xdotool, с аргументом getmouselocation
            char * buffer = new char[1024];
            while(!feof(sensors))
            {
                fgets(buffer, 1024, sensors);                   // Чтение вывода
                mouse_location += buffer;                       // Запись в QString
            }
            delete[] buffer;
            return mouse_location;                              // Возврат QString
        }

        void updateMouse()                                      // Обновить координаты мышки
        {
            QString mouseLocation = this->getMouseLocation();                                                                   // Получаем строку с данными
            mouseLocation.remove(mouseLocation.indexOf('s'), mouseLocation.size() - mouseLocation.indexOf('s'));                // Убираем лишнюю информацию
            auto mouseCoordString = mouseLocation;                                                                              // Вторая строка
            mouseCoordString.remove(0, 2);                                                                                      // Убираем "x:"
            mouseCoordString.remove(mouseCoordString.indexOf('y'), mouseCoordString.size() - mouseCoordString.indexOf(':'));    // Убираем координаты по y

            mouseX = mouseCoordString.toInt();                                                                                  // Превращаем строку в int

            mouseCoordString = mouseLocation;                                                                                   // Повторно копируем строку
            mouseCoordString.remove(0, mouseCoordString.lastIndexOf(':') + 1);                                                  // Обрезка всего, кроме координат по y

            mouseY = mouseCoordString.toInt();                                                                                  // Превращаем строку в int
        }
};

class Keyboard
{
    public:
        void up()
        {
            system(QString("xdotool key --repeat " + QString::number(amount) + " Up").toStdString().c_str());
        }
        void down()
        {
            system(QString("xdotool key --repeat " + QString::number(amount) + " Down").toStdString().c_str());
        }

    private:
        int amount = 5;
};

class programm : public QObject
{
    Q_OBJECT
    public:
        programm()
        {
            Serial.setPort(QSerialPortInfo::availablePorts().first());                                      // Задание порту адреса (TTYUsb0)
        }

    public slots:
        void run()                                                                                          // Запуск
        {
            Serial.open(QIODevice::ReadWrite);                                                              // Открытие порта
            Serial.setBaudRate(QSerialPort::Baud115200);                                                    // Задание частоты 115200 бод
            QObject::connect(&Serial, &QSerialPort::readyRead, this, &programm::on_SerialReadAvailable);    // Соединение со слотом.
        }

        void on_SerialReadAvailable()                                                                       // Вызывается при приёме сигнала с последовательного порта
        {
            auto message = Serial.readAll();                                                                // Чтение данных
            /*
                  ОБОЗНАЧЕНИЕ КОМАНД
                  1 - МЫШЬ ВВЕРХ
                  2 - МЫШЬ ВНИЗ
                  3 - МЫШЬ ВПРАВО
                  4 - МЫШЬ ВЛЕВО
                  5 - ЛКМ
                  6 - СТРЕЛКА ВВЕРХ
                  7 - СТРЕЛКА ВНИЗ
                  8 - УМЕНЬШИТЬ СКОРОСТЬ МЫШИ
                  9 - УВЕЛИЧИТЬ СКОРОСТЬ МЫШИ
            */
            if(message.contains("1"))
            {
                mouse.up();
            }
            if(message.contains("2"))
            {
                mouse.down();
            }
            if(message.contains("3"))
            {
                mouse.right();
            }
            if(message.contains("4"))
            {
                mouse.left();
            }
            if(message.contains("5"))
            {
                mouse.leftMouseClick();
            }
            if(message.contains("6"))
            {
                keyboard.up();
            }
            if(message.contains("7"))
            {
                keyboard.down();
            }
            if(message.contains("8"))
            {
                mouse.slower();
            }
            if(message.contains("9"))
            {
                mouse.faster();
            }
        }
    private:
        QSerialPort Serial;
        Mouse mouse;
        Keyboard keyboard;
};

#include "main.moc"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    programm MainApp;

    QTimer::singleShot(0, &MainApp, SLOT(run()));

    return a.exec();
}
