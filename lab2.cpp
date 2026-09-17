#include <iostream>
#include <string>
#include <memory>

using namespace std;

// Інтерфейс соціальної мережі
class ISocialNetwork
{
public:
    virtual ~ISocialNetwork() = default;

    // Методи для роботи з мережею
    virtual void logIn() = 0;
    virtual void publishMessage(const string &message) = 0;
    virtual void logOut() = 0;
};

// Клас для підключення до Facebook
class FacebookConnector : public ISocialNetwork
{
private:
    string login;
    string password;

public:
    FacebookConnector(const string &login, const string &password)
        : login(login), password(password) {}

    void logIn() override
    {
        // Логіка авторизації у Facebook
    }

    void publishMessage(const string &message) override
    {
        // Логіка публікації посту у Facebook
        cout << "[Facebook] Користувач " << login << " опублікував: " << message << endl;
    }

    void logOut() override
    {
        // Логіка виходу з акаунту Facebook
    }
};

// Клас для підключення до LinkedIn
class LinkedInConnector : public ISocialNetwork
{
private:
    string email;
    string password;

public:
    LinkedInConnector(const string &email, const string &password)
        : email(email), password(password) {}

    void logIn() override
    {
        // Логіка авторизації у LinkedIn
    }

    void publishMessage(const string &message) override
    {
        // Логіка публікації статті у LinkedIn
        cout << "[LinkedIn] Користувач " << email << " опублікував: " << message << endl;
    }

    void logOut() override
    {
        // Логіка виходу з акаунту LinkedIn
    }
};

// Клас, який використовує фабричний метод для публікації повідомлень у соціальних мережах
class SocialNetworkPoster
{
public:
    virtual ~SocialNetworkPoster() = default;

    // Фабричний метод, який повинен бути реалізований у підкласах для створення конкретних об'єктів соціальних мереж
    virtual unique_ptr<ISocialNetwork> createNetwork() const = 0;

    // Метод для публікації повідомлення у соціальній мережі
    void post(const string &message) const
    {
        // Використовуємо фабричний метод для створення об'єкта соціальної мережі
        unique_ptr<ISocialNetwork> network = createNetwork();

        network->logIn();
        network->publishMessage(message);
        network->logOut();
    }
};

// Підклас для публікації повідомлень у Facebook
class FacebookPoster : public SocialNetworkPoster
{
private:
    string login;
    string password;

public:
    FacebookPoster(const string &login, const string &password)
        : login(login), password(password) {}

    // Створюємо об'єкт FacebookConnector для публікації повідомлень у Facebook
    unique_ptr<ISocialNetwork> createNetwork() const override
    {
        return make_unique<FacebookConnector>(login, password);
    }
};

class LinkedInPoster : public SocialNetworkPoster
{
private:
    string email;
    string password;

public:
    LinkedInPoster(const string &email, const string &password)
        : email(email), password(password) {}

    // Створюємо об'єкт LinkedInConnector для публікації повідомлень у LinkedIn
    unique_ptr<ISocialNetwork> createNetwork() const override
    {
        return make_unique<LinkedInConnector>(email, password);
    }
};

// Головна функція програми
int main()
{
    // Створюємо творця для Facebook із необхідними параметрами (login, password)
    unique_ptr<SocialNetworkPoster> fbPoster = make_unique<FacebookPoster>("user_fb_login", "secret_pass123");

    // Публікуємо повідомлення у Facebook
    fbPoster->post("Привіт, Facebook! Це моє перше повідомлення.");

    // Створюємо творця для LinkedIn із необхідними параметрами (email, password)
    unique_ptr<SocialNetworkPoster> liPoster = make_unique<LinkedInPoster>("user@email.com", "secure_pass456");

    // Публікуємо повідомлення у LinkedIn
    liPoster->post("Привіт, LinkedIn! Шукаю нові професійні можливості.");

    return 0;
}
