#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>

using namespace std;

// Спільний інтерфейс для завантажувачів
class Downloader
{
public:
    virtual ~Downloader() = default;
    virtual string download(const string &url) = 0;
};

// Клас простого завантажувача, який імітує завантаження файлу з мережі
class SimpleDownloader : public Downloader
{
public:
    string download(const string &url) override
    {
        // Реалізація складного/повільного завантаження з мережі
        cout << "[Мережа] Завантаження файлу з: " << url << "..." << endl;
        return "Вміст файлу [" + url + "]";
    }
};

// Клас-замісник, який додає кешування до завантажувача
class ProxyCacheDownloader : public Downloader
{
private:
    shared_ptr<SimpleDownloader> realDownloader;
    unordered_map<string, string> cache;

public:
    ProxyCacheDownloader() : realDownloader(make_shared<SimpleDownloader>()) {}

    string download(const string &url) override
    {
        // Перевіряємо, чи є файл у кеші
        auto it = cache.find(url);
        if (it != cache.end())
        {
            cout << "[Кеш] Повернення збереженого файлу: " << url << endl;
            return it->second;
        }

        // Якщо в кеші немає, делегуємо завантаження реальному об'єкту
        string downloadedData = realDownloader->download(url);

        // Зберігаємо результат у кеш для майбутніх звернень
        cache[url] = downloadedData;

        return downloadedData;
    }
};

// Клієнтський код
void renderPage(Downloader &downloader, const string &pageUrl)
{
    cout << "Рендеринг сторінки. Запит ресурсів..." << endl;
    string data = downloader.download(pageUrl);
    cout << "Результат: " << data << "\n"
         << endl;
}

int main()
{
    ProxyCacheDownloader smartDownloader;

    // Перше звернення - дані беруться з "мережі"
    renderPage(smartDownloader, "http://example.com/image.png");

    // Друге звернення - дані беруться з кешу
    renderPage(smartDownloader, "http://example.com/image.png");

    // Третє звернення - дані беруться знову з "мережі"
    renderPage(smartDownloader, "http://example.com/style.css");

    return 0;
}
