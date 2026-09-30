#include <iostream>
#include <string>
#include <memory>

using namespace std;

// Цільовий клієнтський інтерфейс, який заборонено змінювати
class Notification
{
public:
    virtual ~Notification() = default;
    virtual void send(const string &title, const string &message) = 0;
};

// Клас, що відповідає цільовому інтерфейсу
class EmailNotification : public Notification
{
private:
    string adminEmail;

public:
    EmailNotification(const string &email) : adminEmail(email) {}

    void send(const string &title, const string &message) override
    {
        // Логіка відправки email
        cout << "Sent email with title '" << title << "' to '" << adminEmail << "' that says '" << message << "'." << endl;
    }
};

// Сервіс Slack
class SlackApi
{
private:
    string login;
    string apiKey;

public:
    SlackApi(const string &login, const string &apiKey)
        : login(login), apiKey(apiKey) {}

    void authenticate()
    {
        // Логіка авторизації в Slack
    }

    void sendMessage(const string &chatId, const string &text)
    {
        // Логіка відправки повідомлення в конкретний чат
        cout << "Sent Slack message to chat '" << chatId << "': " << text << endl;
    }
};

// Сервіс SMS
class SmsService
{
public:
    void sendSms(const string &phone, const string &sender, const string &text)
    {
        // Логіка відправки SMS
        cout << "Sent SMS to '" << phone << "' from '" << sender << "' with text: " << text << endl;
    }
};

// 4. Адаптери, які роблять сторонні сервіси сумісними з інтерфейсом Notification
// Адаптер для Slack
class SlackNotificationAdapter : public Notification
{
private:
    shared_ptr<SlackApi> slackApi;
    string chatId;

public:
    // Конструктор приймає налаштований сервіс та chatId
    SlackNotificationAdapter(shared_ptr<SlackApi> api, const string &chatId)
        : slackApi(api), chatId(chatId) {}

    void send(const string &title, const string &message) override
    {
        slackApi->authenticate();
        // Об'єднання параметрів у формат, який очікує Slack
        string formattedText = title + " - " + message;
        slackApi->sendMessage(chatId, formattedText);
    }
};

// Адаптер для SMS
class SmsNotificationAdapter : public Notification
{
private:
    shared_ptr<SmsService> smsService;
    string phone;
    string sender;

public:
    // Конструктор приймає сервіс, телефон та ім'я відправника
    SmsNotificationAdapter(shared_ptr<SmsService> service, const string &phone, const string &sender)
        : smsService(service), phone(phone), sender(sender) {}

    void send(const string &title, const string &message) override
    {
        // Об'єднання параметрів у формат, який очікує SMS
        string formattedText = title + ": " + message;
        smsService->sendSms(phone, sender, formattedText);
    }
};

// Клієнтський код
int main()
{
    // Відправка через Email
    unique_ptr<Notification> email = make_unique<EmailNotification>("admin@example.com");
    email->send("Системна помилка", "Сервер не відповідає.");

    // Відправка через Slack за допомогою Адаптера
    shared_ptr<SlackApi> slackService = make_shared<SlackApi>("slack_login", "super_secret_api_key");
    unique_ptr<Notification> slackNotification = make_unique<SlackNotificationAdapter>(slackService, "dev_ops_chat_id");
    slackNotification->send("Попередження", "Високе навантаження на процесор.");

    // Відправка через SMS за допомогою Адаптера
    shared_ptr<SmsService> smsService = make_shared<SmsService>();
    unique_ptr<Notification> smsNotification = make_unique<SmsNotificationAdapter>(smsService, "+380123456789", "MySystemAlert");
    smsNotification->send("Критично", "База даних впала!");

    return 0;
}
