#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

// Попереднє оголошення класів елементів
class Company;
class Department;
class Employee;

// Клас інтерфейсу Відвідувача (Visitor)
class IVisitor
{
public:
    virtual ~IVisitor() = default;
    virtual void visit(const Company &company) = 0;
    virtual void visit(const Department &department) = 0;
    virtual void visit(const Employee &employee) = 0;
};

// Клас інтерфейсу Елемента (Element)
class IOrgElement
{
public:
    virtual ~IOrgElement() = default;
    virtual void accept(IVisitor &visitor) const = 0;
};

// Конкретні класи елементів: Компанія, Департамент, Співробітник
class Employee : public IOrgElement
{
private:
    string position;
    double salary;

public:
    // Приймає назву позиції та розмір зарплати
    Employee(const string &position, double salary)
        : position(position), salary(salary) {}

    string getPosition() const { return position; }
    double getSalary() const { return salary; }

    void accept(IVisitor &visitor) const override
    {
        visitor.visit(*this);
    }
};

// Клас Департаменту, який містить співробітників
class Department : public IOrgElement
{
private:
    string name;
    vector<shared_ptr<Employee>> employees;

public:
    // Приймає масив співробітників
    Department(const string &name, const vector<shared_ptr<Employee>> &employees)
        : name(name), employees(employees) {}

    string getName() const { return name; }

    void accept(IVisitor &visitor) const override
    {
        visitor.visit(*this);
        for (const auto &emp : employees)
        {
            emp->accept(visitor);
        }
    }
};

// Клас Компанії, який містить департаменти
class Company : public IOrgElement
{
private:
    string title;
    vector<shared_ptr<Department>> departments;

public:
    // Приймає масив департаментів
    Company(const string &title, const vector<shared_ptr<Department>> &departments)
        : title(title), departments(departments) {}

    string getTitle() const { return title; }

    void accept(IVisitor &visitor) const override
    {
        visitor.visit(*this);
        for (const auto &dept : departments)
        {
            dept->accept(visitor);
        }
    }
};

// Клас Відвідувача для формування зарплатної відомості
class SalaryReportVisitor : public IVisitor
{
private:
    string reportBuffer;

public:
    void visit(const Company &company) override
    {
        // Схематичне формування заголовка компанії
        reportBuffer += "=== Зарплатна відомість компанії: " + company.getTitle() + " ===\n";
    }

    void visit(const Department &department) override
    {
        // Схематичне додавання інформації про департамент
        reportBuffer += "\n[Департамент: " + department.getName() + "]\n";
    }

    void visit(const Employee &employee) override
    {
        // Схематичне додавання позиції та зарплати
        reportBuffer += " - " + employee.getPosition() + ": " + to_string(employee.getSalary()) + " грн\n";
    }

    string getReport() const
    {
        return reportBuffer;
    }

    void clear()
    {
        reportBuffer.clear();
    }
};

// Клієнтський код
int main()
{
    auto dev1 = make_shared<Employee>("Senior C++ Developer", 120000.0);
    auto dev2 = make_shared<Employee>("Junior QA Engineer", 35000.0);
    auto hr1 = make_shared<Employee>("HR Manager", 45000.0);

    auto itDept = make_shared<Department>("IT Відділ", vector<shared_ptr<Employee>>{dev1, dev2});
    auto hrDept = make_shared<Department>("HR Відділ", vector<shared_ptr<Employee>>{hr1});

    Company myCompany("TechSolutions", {itDept, hrDept});

    SalaryReportVisitor payrollVisitor;

    // 1. Отримання репорту для всієї компанії
    myCompany.accept(payrollVisitor);
    cout << payrollVisitor.getReport() << endl;

    // Очищуємо буфер відвідувача перед новим запитом
    payrollVisitor.clear();

    // 2. Отримання репорту окремо для одного департаменту
    itDept->accept(payrollVisitor);
    cout << payrollVisitor.getReport() << endl;

    return 0;
}
