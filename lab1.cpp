#include <iostream>
#include <string>
#include <memory>

using namespace std;

// Базовий інтерфейс для всіх сховищ
class IStorage
{
public:
    virtual ~IStorage() = default;

    // Методи для роботи з файлами
    virtual void uploadFile(const string &userId, const string &fileName, const string &fileData) = 0;
    virtual string downloadFile(const string &userId, const string &fileName) = 0;
};

// Реалізація локального сховища
class LocalStorage : public IStorage
{
private:
    LocalStorage() {}
    ~LocalStorage() = default;

public:
    LocalStorage(const LocalStorage &) = delete;
    LocalStorage &operator=(const LocalStorage &) = delete;

    static LocalStorage &getInstance()
    {
        static LocalStorage instance;
        return instance;
    }

    void uploadFile(const string &userId, const string &fileName, const string &fileData) override
    {
        // Логіка збереження на локальний диск
    }

    string downloadFile(const string &userId, const string &fileName) override
    {
        // Логіка читання з локального диска
        return "local_file_content_placeholder";
    }
};

// Реалізація сховища Amazon S3
class AmazonS3Storage : public IStorage
{
private:
    AmazonS3Storage() {}
    ~AmazonS3Storage() = default;

public:
    AmazonS3Storage(const AmazonS3Storage &) = delete;
    AmazonS3Storage &operator=(const AmazonS3Storage &) = delete;

    static AmazonS3Storage &getInstance()
    {
        static AmazonS3Storage instance;
        return instance;
    }

    void uploadFile(const string &userId, const string &fileName, const string &fileData) override
    {
        // Логіка відправки даних на AWS S3
    }

    string downloadFile(const string &userId, const string &fileName) override
    {
        return "s3_file_content_placeholder";
    }
};

// Клас користувача
class User
{
private:
    string id;
    IStorage *storage;

public:
    User(string userId, IStorage *selectedStorage) : id(userId), storage(selectedStorage) {}

    void setStorage(IStorage *newStorage)
    {
        storage = newStorage;
    }

    void saveFile(const string &fileName, const string &data)
    {
        if (storage)
        {
            storage->uploadFile(id, fileName, data);
        }
    }

    string getFile(const string &fileName)
    {
        if (storage)
        {
            return storage->downloadFile(id, fileName);
        }
        return "";
    }
};

int main()
{
    IStorage *local = &LocalStorage::getInstance();
    IStorage *s3 = &AmazonS3Storage::getInstance();

    User user1("user_001", local);
    User user2("user_002", s3);

    user1.saveFile("document.txt", "Hello Local Storage!");
    user2.saveFile("avatar.png", "Image Data");

    return 0;
}
