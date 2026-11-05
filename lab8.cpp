#include <iostream>
#include <string>
#include <map>

using namespace std;

// Схематична структура для відповіді
struct RestResponse
{
    int code;
    string status;
    string payload;
};

// Абстрактний базовий клас для оновлення REST-ресурсів
class AbstractRestUpdater
{
public:
    virtual ~AbstractRestUpdater() = default;

    // Шаблонний метод (Template Method), який визначає загальний алгоритм оновлення ресурсу
    RestResponse update(int id, const map<string, string> &data)
    {
        string entity = getEntity(id);

        if (!validateData(data))
        {
            onValidationFailed(); // Виклик хука при помилці валідації
            return {400, "Bad Request", "Validation failed"};
        }

        saveInformation(entity, data);
        return formatResponse();
    }

protected:
    // Обов'язкові абстрактні кроки, які мають реалізувати підкласи
    virtual string getEntity(int id) = 0;
    virtual void saveInformation(const string &entity, const map<string, string> &data) = 0;

    // Крок з типовою реалізацією валідації
    virtual bool validateData(const map<string, string> &data)
    {
        // Умовна базова валідація
        cout << "Виконання базової валідації вихідних даних..." << endl;
        return true;
    }

    // Крок з типовою реалізацією формування відповіді
    virtual RestResponse formatResponse()
    {
        return {200, "OK", ""};
    }

    // Хук, який викликається при помилці валідації
    virtual void onValidationFailed()
    {
        // За замовчуванням нічого не робить
    }
};

// Сутність Товар
class ProductUpdater : public AbstractRestUpdater
{
protected:
    string getEntity(int id) override
    {
        return "ProductEntity_" + to_string(id);
    }

    bool validateData(const map<string, string> &data) override
    {
        // Симуляція помилки валідації
        return false;
    }

    void saveInformation(const string &entity, const map<string, string> &data) override
    {
        cout << "Збереження даних товару..." << endl;
    }

    // Перевизначення хука: адміністратор має отримати сповіщення у месенджер
    void onValidationFailed() override
    {
        cout << "Відправка повідомлення в месенджер адміну: Помилка валідації товару!" << endl;
    }
};

// Сутність Користувач
class UserUpdater : public AbstractRestUpdater
{
protected:
    string getEntity(int id) override
    {
        return "UserEntity_" + to_string(id);
    }

    // Перевизначення валідації: заборонено змінювати email
    bool validateData(const map<string, string> &data) override
    {
        if (data.find("email") != data.end())
        {
            cout << "Зміна поля email заборонена!" << endl;
            return false;
        }
        return AbstractRestUpdater::validateData(data); // Виклик базової логіки
    }

    void saveInformation(const string &entity, const map<string, string> &data) override
    {
        cout << "Збереження даних користувача..." << endl;
    }
};

// Сутність Замовлення
class OrderUpdater : public AbstractRestUpdater
{
protected:
    string getEntity(int id) override
    {
        return "OrderEntity_" + to_string(id);
    }

    void saveInformation(const string &entity, const map<string, string> &data) override
    {
        cout << "Збереження даних замовлення..." << endl;
    }

    // Перевизначення формування відповіді: повертати JSON-подання
    RestResponse formatResponse() override
    {
        RestResponse baseResponse = AbstractRestUpdater::formatResponse();
        baseResponse.payload = "{ \"orderId\": 123, \"total\": 500.00 }";

        return baseResponse;
    }
};

// Клієнтський код
int main()
{
    map<string, string> requestData = {{"name", "NewName"}, {"email", "test@mail.com"}};

    cout << "Оновлення Товару" << endl;
    ProductUpdater productUpdate;
    productUpdate.update(1, requestData);

    cout << "\nОновлення Користувача" << endl;
    UserUpdater userUpdate;
    userUpdate.update(2, requestData);

    cout << "\nОновлення Замовлення" << endl;
    OrderUpdater orderUpdate;
    RestResponse orderRes = orderUpdate.update(3, {{"status", "completed"}});
    cout << "Відповідь: " << orderRes.code << " " << orderRes.status << " JSON: " << orderRes.payload << endl;

    return 0;
}
