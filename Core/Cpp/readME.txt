/*!
* \mainpage ПО для STM32для КД-СКД-1

Принцип действия контроллера.

На любое действие (нажатие кнопки, считывание ключа, взлом и т.д.) формируется событие которое попадает в контейнер std::deque и формируется очередь событий которые нужно обработать.

Существует две очереди таких сообщений mStateQueueReader (сообщения формируемые прерываниями от считывателей) и mStateQueueOther (все другие сообщения). Это сделано потому что прерывание от считывателя может сработать когда идёт добавление сообщения от других устройств и возникнет конфликт приводящий к х.з. каким последствиям. 
Такая обработка действий сделана, что бы прерывания от считывателей не тормозили работу системы. Типа прерывание сработало, быстро отработало и событие ждёт своей очереди на обработку. 
События из сформированных очередей обрабатываются методом manager () класса TApplication и формируется очередь сообщений для отправки на концентратор.

Список устройств хранится в контейнере mListUnits элементами которого являются наследники базового абстрактного класса TUnit (патерн Фабрика).

Все команды поступающие от концентратора по RS485 обрабатываются классом TProtocol, который получает байты из порта, и если совпадает адрес или команда является безадресной, то при получении символов окончния посылки формирует ответ.
Заголовок ответа, CRC и байты окончания пакета формирует метод makeAnswer () класса TProtocol, а поле DATA формируется методом process () класса наследника абстрактного класса TComProtocol (реализация патерна Фабрика). Т.о. для реализации новой команды нужно создать класс наследованный от TComProtocol, в этом классе реализовать метод process () и в методе create () базового класса TComProtocol создать указатель на данный класс. Т.к. в качестве сформированного указателя используется std::share_prt, то можно не включать мозги когда нужно удалить этот указатель, т.к. он сдохнет сам когда он станет не нужен. 

PS.
Максимальное время передачи ответа на команду запроса диагностики 0x52 около 40 мСек.
Использовать CCMRAM не получилось, т.к. DMA с ней не работает, а я не смог отправить в RAM буфер в который пишутся по DMA данные. :( 

Совершенно не изучена проблема поиска обработчика exeption и поэтому я их обрабатываю сразу 
*/