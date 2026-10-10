#include <iostream>
#include <string>
#include <memory>

using namespace std;

// Структура для зберігання даних товару
struct Product
{
    int id;
    string name;
    string description;
    string image;
};

// Інтерфейс рендерера, який визначає методи для рендерингу різних частин сторінки
class IRenderer
{
public:
    virtual ~IRenderer() = default;

    // Методи для рендеру окремих частин сторінки
    virtual string renderTitle(const string &title) const = 0;
    virtual string renderText(const string &text) const = 0;
    virtual string renderImage(const string &url) const = 0;
    virtual string renderId(int id) const = 0;
};

// Клас рендерера для HTML
class HTMLRenderer : public IRenderer
{
public:
    string renderTitle(const string &title) const override { return "<h1>" + title + "</h1>"; }
    string renderText(const string &text) const override { return "<p>" + text + "</p>"; }
    string renderImage(const string &url) const override { return "<img src='" + url + "'/>"; }
    string renderId(int id) const override { return "<span>ID: " + to_string(id) + "</span>"; }
};

// Клас рендерера для JSON
class JsonRenderer : public IRenderer
{
public:
    string renderTitle(const string &title) const override { return "\"title\": \"" + title + "\""; }
    string renderText(const string &text) const override { return "\"text\": \"" + text + "\""; }
    string renderImage(const string &url) const override { return "\"image\": \"" + url + "\""; }
    string renderId(int id) const override { return "\"id\": " + to_string(id); }
};

// Клас рендерера для XML
class XmlRenderer : public IRenderer
{
public:
    string renderTitle(const string &title) const override { return "<title>" + title + "</title>"; }
    string renderText(const string &text) const override { return "<text>" + text + "</text>"; }
    string renderImage(const string &url) const override { return "<image>" + url + "</image>"; }
    string renderId(int id) const override { return "<id>" + to_string(id) + "</id>"; }
};

// Абстрактний клас сторінки, який використовує рендерер для відображення контенту
class Page
{
protected:
    // Міст до реалізації
    shared_ptr<IRenderer> renderer;

public:
    Page(shared_ptr<IRenderer> r) : renderer(r) {}
    virtual ~Page() = default;

    virtual void view() const = 0;

    // Метод для зміни рендерера на льоту
    void setRenderer(shared_ptr<IRenderer> r)
    {
        renderer = r;
    }
};

// Клас простої сторінки, який відображає заголовок та текст
class SimplePage : public Page
{
private:
    string title;
    string content;

public:
    SimplePage(shared_ptr<IRenderer> r, const string &t, const string &c)
        : Page(r), title(t), content(c) {}

    void view() const override
    {
        // Відображає сторінку, використовуючи рендерер
    }
};

// Клас сторінки товару, який відображає деталі продукту
class ProductPage : public Page
{
private:
    Product product;

public:
    ProductPage(shared_ptr<IRenderer> r, const Product &p)
        : Page(r), product(p) {}

    void view() const override
    {
        // Відображає сторінку товару, використовуючи рендерер
    }
};

// Клієнтський код
int main()
{
    auto htmlRenderer = make_shared<HTMLRenderer>();
    auto jsonRenderer = make_shared<JsonRenderer>();

    SimplePage aboutPage(htmlRenderer, "Про нас", "Текст про компанію");
    aboutPage.view();

    // Змінюємо рендерер на льоту
    aboutPage.setRenderer(jsonRenderer);
    aboutPage.view();

    Product p = {101, "Ноутбук", "Потужний ігровий ноутбук", "laptop.png"};
    auto xmlRenderer = make_shared<XmlRenderer>();

    ProductPage itemPage(xmlRenderer, p);
    itemPage.view();

    return 0;
}
