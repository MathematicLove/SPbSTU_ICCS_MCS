#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <bitset>
#include <algorithm>

using namespace std;

// Состояния строки кэша
enum CacheState { //RT-MESI протокол
    INVALID = 0,
    EXLUSIVE,
    SHARED,
    MODIFIED,
    READ_T // Добавлено состояние "Read-Through"
};

//объявление структуры CacheLine, которая представляет собой строку кэша с тэгом, данными, состоянием, счетчиком политики и временем последнего доступа.
struct CacheLine { // 4 бита = 3 бита - тэг + 1 младший бит контейнер (либо 0 либо 1)
    std::string tag;
    std::string data; // инфа
    CacheState state; // состояние одно из RT/M/E/S/I
    int policy_counter; // далее будет использована для политики MRU
    int last_access_time; // для политики замещения MRU
};

//объявление структуры MainMemoryCell, которая представляет ячейку основной памяти с содержимым и адресом.
struct MainMemoryCell {
    std::string content;
    int address; // 4 бита = 3 тэг + 1 бит контейнер
};

//объявление констант, определяющих размер основной памяти, размер строки кэша, размер кэша, ассоциативность кэша и количество процессоров в системе.
const int main_memory_size = 16;
const int cache_line_size = 4;
const int cache_size = 16;
const int associativity = 2; // для контейнеров 0 & 1
const int num_processors = 4;

std::map<std::string, MainMemoryCell> main_memory; //объявление переменной main_memory, которая представляет собой карту, связывающую строки адресов с ячейками основной памяти.
//псевдоним
std::vector<std::vector<CacheLine>> cache(num_processors, std::vector<CacheLine>(cache_size / num_processors));//объявление переменной cache, которая представляет собой вектор векторов строк кэша для каждого процессора. Это инициализируется вектором строк кэша для каждого процессора, которые затем инициализируются пустыми строками кэша.

int global_tick_counter = 0; //объявление переменной global_tick_counter, которая представляет глобальный счетчик тактов машинного времени.

// объявление функции read_from_cache, которая осуществляет чтение данных из кэша по указанному тегу для заданного процессора.
CacheLine* read_from_cache(int processor_id, std::string tag) {
    for (auto& line : cache[processor_id]) {
        if (line.tag == tag && line.state != INVALID) {
            line.last_access_time = global_tick_counter;
            return &line; // адресс ячейки
        }
    }
    return nullptr;
}



// объявление функции write_to_cache, которая осуществляет запись данных в кэш по указанному тегу для заданного процессора.
void write_to_cache(int processor_id, std::string tag, std::string data) {
        // Определение контейнера
    char container_bit = tag.back();
    //tag = tag.substr(0, 4);
    //tag += container_bit; // бит контейнера

    // Поиск строки кэша для записи
    int i=0,mmax =-1;
    for (auto& line : cache[processor_id]) {
        i++;
        if((container_bit == '1' && i>1) ||(container_bit == '0' && i<2))
        if (line.state == INVALID) {
            line.tag = tag;
            line.data = data;
            line.state = MODIFIED;
            line.policy_counter = 0;
            line.last_access_time = global_tick_counter; //ищет первую пустую ячейку
            return;
        }
    }
    
    // Применение политики замещения (MRU)
    i=0;
    auto mit =cache[processor_id].begin();
    for(auto it = cache[processor_id].begin();it!=cache[processor_id].end();it++){
        i++;
        if((container_bit == '1' && i>=2) ||(container_bit == '0' && i<2)){
            if (it->last_access_time > mmax) {
                mit = it;
                mmax = it->last_access_time;
            }
        }
    }
    mit->tag = tag;
    mit->data = data;
    mit->state = MODIFIED;
    mit->policy_counter = 0;
    mit->last_access_time = global_tick_counter;
}

void user_write_to_cache(int processor_id){
    //cout << "Введите информацию которую хотите положить в кэш: " << endl;
    string data;
    
    cout << "Введите адрес: " << endl;
    string tag;
    cin >> tag;
    for(auto mm : main_memory){
        if(mm.first == tag) {
            data = mm.second.content;
        }
    }
    write_to_cache(processor_id, tag,data);
}

void user_read_from_cache(int processor_id){
    int i;
    int j = 0;
    i = 0;
    for(auto& line: cache[processor_id]){
        cout << i << ". " << "tag =" << line.tag << endl;
        i++;
        
    }
   // cout << "Введите номер адреса который хотите вывести: " << endl;
    //cin >> i;
    
    for(auto& line: cache[processor_id]){
            //line.policy_counter = global_tick_counter;
            cout << "Line " << j << ": Tag = " << line.tag << ", Data = " << line.data << ", State = ";
            switch (line.state) {
                case EXLUSIVE:
                    cout << "EXLUSIVE";
                case INVALID:
                    cout << "INVALID";
                    break;
                case SHARED:
                    cout << "SHARED";
                    break;
                case MODIFIED:
                    cout << "MODIFIED";
                    break;
                case READ_T:
                    cout << "READ_THROUGH";
                    break;
            }
            cout << endl;
        }
        cout << endl;
        i++;
    }


// Функция сброса состояния модели
void reset_model_state() { // все сбрасывает в 0
    for (auto& processor_cache : cache) {
        for (auto& line : processor_cache) {
            line.state = INVALID;
            line.policy_counter = 0;
            line.last_access_time = 0;
            CacheLine().data.clear();
            CacheLine().tag.clear();
            CacheLine().last_access_time*0;
        }
    }
    for (auto& cell : main_memory) {
        cell.second.content = "0";
    }
}

// объявление функции advance_ticks, которая продвигает глобальный счетчик тактов машинного времени на один такт.
void advance_ticks() {
    global_tick_counter++;
}

// объявление функции display_state, которая отображает текущее состояние кэшей и основной памяти на экране.
void display_state() {
    // Вывод состояния кэшей
    for (int i = 0; i < num_processors; ++i) {
        cout << "Cache for Processor " << i << ":" << endl;
        for (int j = 0; j < cache[i].size(); ++j) {
            // Интерпретация тега
            std::string tag = cache[i][j].tag.substr(0, 3); // Первые три бита как тег
            int container = cache[i][j].tag.back() - '0'; // Младший бит как контейнер

            // Вывод состояния
            cout << "Line " << j << ": Tag = " << tag << ", Container = " << container << ", Data = " << cache[i][j].data << ", State = ";
            switch (cache[i][j].state) {
                case EXLUSIVE:
                    cout << "EXLUSIVE";
                case INVALID:
                    cout << "INVALID";
                    break;
                case SHARED:
                    cout << "SHARED";
                    break;
                case MODIFIED:
                    cout << "MODIFIED";
                    break;
                case READ_T:
                    cout << "READ_THROUGH";
                    break;
            }
            cout << endl;
        }
        cout << endl;
    }


    // Вывод состояния основной памяти
    cout << "Main Memory:" << endl;
     for (const auto& cell : main_memory) {
         cout << "Address: " << cell.first << ", Content: " << cell.second.content << endl;
     }
 }

int main() {
    
    for (int i = 0; i < main_memory_size; ++i) {
        bitset<4> j = i;
        main_memory[j.to_string()] = MainMemoryCell{"00", i};
        
    }
    string data = "FE";
    string data1 = "BF";
    string data2 = "1D";
    string data3 = "C9";
    string tag = "0101"; // 4 бита для тега = 3 бита адрес а 1 младший бит
    string tag1 = "1010"; // Пример тега для другого контейнера
    string tag2 = "0001";
  
  write_to_cache(0, tag2, data);
  write_to_cache(0, tag2, data1);
    write_to_cache(0, tag, data2);
    write_to_cache(0, tag1, data3);
    
    int x = -1,pnum;
    while (x != 6){
        cout << "1 - Обзор кэша" << endl;
        cout << "2 - Чтение из памяти" << endl;
        cout << "3 - Общий сброс" << endl;
        cout << "4 - Продвижение по тактам" << endl;
        cout << "5 - На нескольких процессорах" << endl;
        cout << "6 - Выход из программы" << endl;
        cin >> x;
        advance_ticks();
        switch (x) {
            case 1: //чтение
                cout << "Введите номер процессора: " << endl;
                cin >> pnum;
                user_read_from_cache(pnum);
                break;
            case 2: //запись
                cout << "Введите номер процессора: " << endl;
                cin >> pnum;
                user_write_to_cache(pnum);
                break;
            case 3: //общий сброс
                reset_model_state();
                break;
            case 4: // продвижение по тактам вручную
                break;
            case 5: // на нескольких процессорах
                //??????
                break;
            case 6: // выход
                cout << "Выход из программы" << endl;
                break;
            default:
                break;
        }
    }
    display_state();

    // Этот цикл инициализирует содержимое основной памяти нулями для каждой ячейки.
    // Графический вывод + 5-ая функция ? + на счет контейнера (где используем последний младший бит?) + зачем mainMemory???? что должно выводится в
    // конкретном адресе ????
    return 0;
}

//1 - как записывать данные в кэш
//2 - какое приминение памяти в нашей модели (в ячейках)
// в каком виде должно происходить общение с моделью
// поменяли поле policy counter - сделали вместо last acces time !!!!!!!!
