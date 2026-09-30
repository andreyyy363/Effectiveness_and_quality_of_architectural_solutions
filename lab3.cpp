#include <iostream>
#include <string>
#include <memory>

using namespace std;

// Інтерфейс Будівельника
class IQueryBuilder
{
public:
    virtual ~IQueryBuilder() = default;

    // Методи для покрокового конструювання запиту
    virtual IQueryBuilder &select(const string &fields) = 0;
    virtual IQueryBuilder &where(const string &condition) = 0;
    virtual IQueryBuilder &limit(int number) = 0;

    // Метод отримання готового результату
    virtual string getSQL() = 0;
};

// Будівельник для PostgreSQL
class PostgreSQLQueryBuilder : public IQueryBuilder
{
private:
    string query;

public:
    PostgreSQLQueryBuilder()
    {
        // Конструктор для ініціалізації проміжного результату
    }

    IQueryBuilder &select(const string &fields) override
    {
        // Логіка додавання SELECT та FROM для PostgreSQL
        return *this;
    }

    IQueryBuilder &where(const string &condition) override
    {
        // Логіка формування умов WHERE для PostgreSQL
        return *this;
    }

    IQueryBuilder &limit(int number) override
    {
        // Логіка додавання LIMIT для PostgreSQL
        return *this;
    }

    string getSQL() override
    {
        // Повернення готового рядка запиту
        return "SELECT ... FROM ... WHERE ... LIMIT ... (Згенеровано для PostgreSQL)";
    }
};

// Будівельник для MySQL
class MySQLQueryBuilder : public IQueryBuilder
{
private:
    string query; // Проміжний результат запиту

public:
    MySQLQueryBuilder()
    {
        // Конструктор для ініціалізації проміжного результату
    }

    IQueryBuilder &select(const string &fields) override
    {
        // Логіка додавання SELECT та FROM для MySQL
        return *this;
    }

    IQueryBuilder &where(const string &condition) override
    {
        // Логіка формування умов WHERE для MySQL
        return *this;
    }

    IQueryBuilder &limit(int number) override
    {
        // Логіка додавання LIMIT для MySQL
        return *this;
    }

    string getSQL() override
    {
        // Повернення готового рядка запиту
        return "SELECT ... FROM ... WHERE ... LIMIT ... (Згенеровано для MySQL)";
    }
};

// Клієнтський код
int main()
{
    // Звернення до PostgreSQL
    unique_ptr<IQueryBuilder> postgresBuilder = make_unique<PostgreSQLQueryBuilder>();

    // Покрокове створення запиту
    string pgQuery = postgresBuilder->select("id, name, email")
                         .where("age >= 18")
                         .limit(10)
                         .getSQL();

    cout << "PostgreSQL запит: " << pgQuery << endl;

    // Звернення до MySQL
    unique_ptr<IQueryBuilder> mysqlBuilder = make_unique<MySQLQueryBuilder>();

    // Покрокове створення запиту (тут ми можемо викликати лише потрібні кроки)
    string mySqlQuery = mysqlBuilder->select("title, description")
                            .limit(5)
                            .getSQL();

    cout << "MySQL запит: " << mySqlQuery << endl;

    return 0;
}
