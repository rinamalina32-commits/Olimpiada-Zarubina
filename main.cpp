#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdlib>

// Ñòðóêòóðà äëÿ õðàíåíèÿ íîìèíàëîâ è êîëè÷åñòâà êóïþð
struct Bill {
    int nominal;
    int count;
};

// Ôóíêöèÿ ñðàâíåíèÿ äëÿ ñîðòèðîâêè ïî óáûâàíèþ íîìèíàëîâ
bool compareDesc(const Bill& a, const Bill& b) {
    return a.nominal > b.nominal;
}

// Ôóíêöèÿ ñðàâíåíèÿ äëÿ ñîðòèðîâêè ïî âîçðàñòàíèþ íîìèíàëîâ
bool compareAsc(const Bill& a, const Bill& b) {
    return a.nominal < b.nominal;
}

// Îñíîâíàÿ ôóíêöèÿ ðåøåíèÿ çàäà÷è
std::vector<Bill> findDispense(const std::vector<Bill>& wallet, int amount, const std::string& strategy) {
    std::vector<Bill> result;
    std::vector<Bill> temp = wallet; // Ñîçäàåì êîïèþ äëÿ ñîðòèðîâêè
    
    // Âûáèðàåì ñòðàòåãèþ ñîðòèðîâêè
    if (strategy == "MAX") {
        std::sort(temp.begin(), temp.end(), compareDesc);
    } else if (strategy == "MIN") {
        std::sort(temp.begin(), temp.end(), compareAsc);
    }
    
    // Æàäíûé àëãîðèòì âûäà÷è
    for (const auto& bill : temp) {
        if (amount == 0) break;
        
        int take = std::min(amount / bill.nominal, bill.count);
        if (take > 0) {
            result.push_back({bill.nominal, take});
            amount -= bill.nominal * take;
        }
    }
    
    // Åñëè ñóììà íå ñîáðàíà ïîëíîñòüþ
    if (amount != 0) {
        return {};
    }
    
    return result;
}

// Ôóíêöèÿ ïàðñèíãà âõîäíûõ äàííûõ èç JSON
std::vector<Bill> parseInput(const std::string& json) {
    std::vector<Bill> wallet;
    // Ïðîñòàÿ ðåàëèçàöèÿ ïàðñèíãà JSON
    // Ïðåäïîëàãàåòñÿ âàëèäíûé ôîðìàò âõîäíûõ äàííûõ
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
    // ×òåíèå âõîäíûõ äàííûõ
    std::ifstream input("input.json");
    std::string jsonData((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    
    // Èçâëå÷åíèå ïàðàìåòðîâ èç JSON
    size_t walletStart = jsonData.find("\"wallet\":") + 9;
    size_t walletEnd = jsonData.find("\"amount\"", walletStart);
    std::string walletStr = jsonData.substr(walletStart, walletEnd - walletStart - 2);
    
    size_t amountStart = jsonData.find("\"amount\":") + 9;
    size_t amountEnd = jsonData.find(",", amountStart);
    int amount = std::atoi(jsonData.substr(amountStart, amountEnd - amountStart).c_str());
    
    size_t strategyStart = jsonData.find("\"strategy\":") + 12;
    size_t strategyEnd = jsonData.find("}", strategyStart);
    std::string strategy = jsonData.substr(strategyStart, strategyEnd - strategyStart - 1);
    
    // Ïàðñèíã êîøåëüêà
    std::vector<Bill> wallet = parseInput(walletStr);
    
    // Ïîèñê ðåøåíèÿ
    std::vector<Bill> result = findDispense(wallet, amount, strategy);
    
    // Ôîðìèðîâàíèå âûõîäíîãî JSON
    std
