#include <iostream>
#include <memory>

using namespace std;

// Клас-інтерфейс для стратегії доставки
class IDeliveryStrategy
{
public:
    virtual ~IDeliveryStrategy() = default;

    // Схематичний метод розрахунку (приймає суму замовлення та відстань)
    virtual double calculateCost(double orderTotal, double distanceKm) const = 0;
};

// Стратегія: Самовивіз
class PickupStrategy : public IDeliveryStrategy
{
public:
    double calculateCost(double orderTotal, double distanceKm) const override
    {
        // Логіка для самовивозу: вартість доставки завжди 0
        return 0.0;
    }
};

// Стратегія: Доставка зовнішньою службою
class ExternalDeliveryStrategy : public IDeliveryStrategy
{
public:
    double calculateCost(double orderTotal, double distanceKm) const override
    {
        // Логіка для зовнішньої доставки: базова вартість + відстань * тариф
        return 50.0 + (distanceKm * 15.0);
    }
};

// Стратегія: Доставка власною службою
class InternalDeliveryStrategy : public IDeliveryStrategy
{
public:
    double calculateCost(double orderTotal, double distanceKm) const override
    {
        // Логіка для власної доставки через кур'єрську службу: базова вартість + відстань * тариф
        return 30.0 + (distanceKm * 10.0);
    }
};

// Контекст, який використовує стратегію доставки
class OrderContext
{
private:
    unique_ptr<IDeliveryStrategy> strategy;

public:
    // Конструктор ініціалізує контекст стартовою стратегією
    OrderContext(unique_ptr<IDeliveryStrategy> initialStrategy)
        : strategy(move(initialStrategy)) {}

    // Дозволяє змінити стратегію на льоту
    void setStrategy(unique_ptr<IDeliveryStrategy> newStrategy)
    {
        strategy = move(newStrategy);
    }

    // Делегування розрахунку поточній стратегії
    double calculateDeliveryPrice(double orderTotal, double distanceKm) const
    {
        if (strategy)
        {
            return strategy->calculateCost(orderTotal, distanceKm);
        }
        return 0.0;
    }
};

// Клієнтський код
int main()
{
    double myOrderTotal = 500.0;
    double myDistance = 5.5;

    // За замовчуванням обираємо власну доставку
    OrderContext order(make_unique<InternalDeliveryStrategy>());
    cout << "Власна доставка: " << order.calculateDeliveryPrice(myOrderTotal, myDistance) << " грн" << endl;

    // Користувач перемикає на самовивіз
    order.setStrategy(make_unique<PickupStrategy>());
    cout << "Самовивіз: " << order.calculateDeliveryPrice(myOrderTotal, myDistance) << " грн" << endl;

    // Користувач перемикає на зовнішню доставку
    order.setStrategy(make_unique<ExternalDeliveryStrategy>());
    cout << "Зовнішня доставка: " << order.calculateDeliveryPrice(myOrderTotal, myDistance) << " грн" << endl;

    return 0;
}
