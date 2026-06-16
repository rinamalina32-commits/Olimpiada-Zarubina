#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdlib>

// Структура для хранения номиналов и количества купюр
struct Bill {
    int nominal;
    int count;
};

// Функция сравнения для сортировки по убыванию номиналов
bool compareDesc(const Bill& a, const Bill& b) {
    return a.nominal > b.nominal;
}

// Функция сравнения для сортировки по возрастанию номиналов
bool compareAsc(const Bill& a, const Bill& b) {
    return a.nominal < b.nominal;
}

// Основная функция решения задачи
std::vector<Bill> findDispense(const std::vector<Bill>& wallet, int amount, const std::string& strategy) {
    std::vector<Bill> result;
    std::vector<Bill> temp = wallet; // Создаем копию для сортировки
    
    // Выбираем стратегию сортировки
    if (strategy == "MAX") {
        std::sort(temp.begin(), temp.end(), compareDesc);
    } else if (strategy == "MIN") {
        std::sort(temp.begin(), temp.end(), compareAsc);
    }
    
    // Жадный алгоритм выдачи
    for (const auto& bill : temp) {
        if (amount == 0) break;
        
        int take = std::min(amount / bill.nominal, bill.count);
        if (take > 0) {
            result.push_back({bill.nominal, take});
            amount -= bill.nominal * take;
        }
    }
    
    // Если сумма не собрана полностью
    if (amount != 0) {
        return {};
    }
    
    return result;
}

// Функция парсинга входных данных из JSON
std::vector<Bill> parseInput(const std::string& json) {
    std::vector<Bill> wallet;
    // Простая реализация парсинга JSON
    // Предполагается валидный формат входных данных
    size_t start = json.find("[[");
    size_t end = json.find("]]");
    std::string data = json.substr(start + 2, end - start - 2);
    
    size_t pos = 0;
    while ((pos = data.find('[', pos)) != std::string::npos) {
        size_t comma = data.find(',', pos);
        size_t close = data.find(']', pos);
        int nominal = std::atoi(data.substr(pos + 1, comma - pos - 1).c_str());
        int count = std::atoi(data.substr(comma + 1, close - comma - 1).c_str());
        wallet.push_back({nominal, count});
        pos = close + 1;
    }
    
    return wallet;
}

int main() {
    // Чтение входных данных
    std::ifstream input("input.json");
    std::string jsonData((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    
    // Извлечение параметров из JSON
    size_t walletStart = jsonData.find("\"wallet\":") + 9;
    size_t walletEnd = jsonData.find("\"amount\"", walletStart);
    std::string walletStr = jsonData.substr(walletStart, walletEnd - walletStart - 2);
    
    size_t amountStart = jsonData.find("\"amount\":") + 9;
    size_t amountEnd = jsonData.find(",", amountStart);
    int amount = std::atoi(jsonData.substr(amountStart, amountEnd - amountStart).c_str());
    
    size_t strategyStart = jsonData.find("\"strategy\":") + 12;
    size_t strategyEnd = jsonData.find("}", strategyStart);
    std::string strategy = jsonData.substr(strategyStart, strategyEnd - strategyStart - 1);
    
    // Парсинг кошелька
    std::vector<Bill> wallet = parseInput(walletStr);
    
    // Поиск решения
    std::vector<Bill> result = findDispense(wallet, amount, strategy);
    
    // Формирование выходного JSON
    std