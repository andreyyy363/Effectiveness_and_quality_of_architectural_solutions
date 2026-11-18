#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

class UIComponent;

// Інтерфейс Посередника (Mediator)
class IOrderMediator
{
public:
    virtual ~IOrderMediator() = default;
    virtual void notify(UIComponent *sender, const string &event) = 0;
};

// Базовий клас Компонента UI
class UIComponent
{
protected:
    IOrderMediator *mediator = nullptr;
    bool enabled = true;
    bool visible = true;

public:
    virtual ~UIComponent() = default;

    void setMediator(IOrderMediator *m)
    {
        mediator = m;
    }

    virtual void setEnabled(bool state) { enabled = state; }
    virtual void setVisible(bool state) { visible = state; }
    bool isEnabled() const { return enabled; }
    bool isVisible() const { return visible; }
};

// Клас конкретного компонента UI: DatePicker
class DatePicker : public UIComponent
{
private:
    string selectedDate;

public:
    void selectDate(const string &date)
    {
        selectedDate = date;
        if (mediator)
            mediator->notify(this, "date_changed");
    }

    string getDate() const { return selectedDate; }
};

// Клас конкретного компонента UI: TimeSlotSelector
class TimeSlotSelector : public UIComponent
{
private:
    vector<string> availableSlots;

public:
    void updateSlots(const vector<string> &slots)
    {
        availableSlots = slots;
        // Логіка оновлення списку проміжків часу у UI
    }
};

// Клас конкретного компонента UI: Checkbox
class Checkbox : public UIComponent
{
private:
    bool checked = false;

public:
    void setChecked(bool state)
    {
        checked = state;
        if (mediator)
            mediator->notify(this, "toggle");
    }

    bool isChecked() const { return checked; }
};

// Клас конкретного компонента UI: TextInput
class TextInput : public UIComponent
{
private:
    string value;
    bool required = false;

public:
    void setRequired(bool state) { required = state; }
    bool isRequired() const { return required; }

    void setValue(const string &text)
    {
        value = text;
        if (mediator)
            mediator->notify(this, "input");
    }
};

// Клас Посередника для сторінки оформлення замовлення квітів
class FlowerOrderCheckoutPage : public IOrderMediator
{
private:
    shared_ptr<DatePicker> deliveryDate;
    shared_ptr<TimeSlotSelector> deliveryTimeSlots;
    shared_ptr<Checkbox> otherRecipientCheckbox;
    shared_ptr<TextInput> recipientNameInput;
    shared_ptr<TextInput> recipientPhoneInput;
    shared_ptr<Checkbox> selfPickupCheckbox;

public:
    FlowerOrderCheckoutPage(
        shared_ptr<DatePicker> date,
        shared_ptr<TimeSlotSelector> slots,
        shared_ptr<Checkbox> otherRecipient,
        shared_ptr<TextInput> nameInput,
        shared_ptr<TextInput> phoneInput,
        shared_ptr<Checkbox> pickup) : deliveryDate(date),
                                       deliveryTimeSlots(slots),
                                       otherRecipientCheckbox(otherRecipient),
                                       recipientNameInput(nameInput),
                                       recipientPhoneInput(phoneInput),
                                       selfPickupCheckbox(pickup)
    {
        // Прив'язуємо всі компоненти до посередника
        deliveryDate->setMediator(this);
        deliveryTimeSlots->setMediator(this);
        otherRecipientCheckbox->setMediator(this);
        recipientNameInput->setMediator(this);
        recipientPhoneInput->setMediator(this);
        selfPickupCheckbox->setMediator(this);

        // Початковий стан UI: поля отримувача приховані
        recipientNameInput->setVisible(false);
        recipientPhoneInput->setVisible(false);
    }

    void notify(UIComponent *sender, const string &event) override
    {
        // 1. Зміна дати доставки
        if (sender == deliveryDate.get() && event == "date_changed")
        {
            deliveryTimeSlots->updateSlots({"09:00-12:00", "14:00-18:00"});
        }

        // 2. Чекбокс "отримувач інша особа"
        if (sender == otherRecipientCheckbox.get() && event == "toggle")
        {
            bool isOther = otherRecipientCheckbox->isChecked();
            recipientNameInput->setVisible(isOther);
            recipientPhoneInput->setVisible(isOther);
            recipientNameInput->setRequired(isOther);
            recipientPhoneInput->setRequired(isOther);
        }

        // 3. Чекбокс "самовивіз з магазину"
        if (sender == selfPickupCheckbox.get() && event == "toggle")
        {
            bool isPickup = selfPickupCheckbox->isChecked();
            bool deliveryFieldsActive = !isPickup;

            deliveryDate->setEnabled(deliveryFieldsActive);
            deliveryTimeSlots->setEnabled(deliveryFieldsActive);
            otherRecipientCheckbox->setEnabled(deliveryFieldsActive);
            recipientNameInput->setEnabled(deliveryFieldsActive);
            recipientPhoneInput->setEnabled(deliveryFieldsActive);
        }
    }
};

// Клієнтський код
int main()
{
    auto dateField = make_shared<DatePicker>();
    auto timeSlots = make_shared<TimeSlotSelector>();
    auto otherPersonChk = make_shared<Checkbox>();
    auto nameField = make_shared<TextInput>();
    auto phoneField = make_shared<TextInput>();
    auto pickupChk = make_shared<Checkbox>();

    FlowerOrderCheckoutPage checkoutMediator(dateField, timeSlots, otherPersonChk, nameField, phoneField, pickupChk);

    // Демонстрація взаємодії елементів через посередника
    dateField->selectDate("2026-11-20");
    otherPersonChk->setChecked(true);
    pickupChk->setChecked(true);

    return 0;
}
